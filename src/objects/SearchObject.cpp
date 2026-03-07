#include "SearchObject.h"

#include <Utils.h>
#include <modified/GDDLLevelBrowserLayer.h>

std::string SearchObject::createSearchParametersString() {
    std::string request;
    for (const auto& setting : searchableSettings) {
        request += setting->getSearchQueryFragment();
    }
    return request;
}

std::string SearchObject::createFullSearchQuery(const std::string& queryParameters) {
    std::string request = searchEndpoint + "?page=" + std::to_string(apiPagesFetched) + "&limit=25" + queryParameters;
    log::info("Search request: {}", request);
    return request;
}

GJSearchObject* SearchObject::createGJSearchObjectFromIndex(const unsigned long long firstIndex, std::vector<int> filteredResults) const {
    std::string requestString;
    const unsigned lastIndex = std::min(firstIndex + inGameResultsPageSize, static_cast<unsigned long long>(filteredResults.size()));
    for (unsigned i = firstIndex; i < lastIndex; i++) {
        requestString += std::to_string(filteredResults[i]) + ',';
    }
    if (!requestString.empty()) {
        requestString.pop_back();
    }
    requestString += "&gameVersion=22";
    return GJSearchObject::create(SearchType::Type19, requestString);
}

void SearchObject::getSearchResultsForPage(const int pageNumber, GDDLLevelBrowserLayer* callingLayer, ILoadingCircleHaver* loadingCircleHaver) {
    const std::vector<int> filteredResults = filterResults(results, completedSetting->getSettingValue(), uncompletedSetting->getSettingValue());
    if (isPageReady(pageNumber, filteredResults)) {
        // display what we have
        const int actualPageNumber = std::min(pageNumber, getPageCountOf(filteredResults) - 1);
        GJSearchObject* gjSearchObject = createGJSearchObjectFromIndex(actualPageNumber * inGameResultsPageSize, filteredResults);
        // the "ready" page might not necessarily be the one requested as there might be less than the requested amount of pages
        forwardToLevelBrowser(gjSearchObject, callingLayer, actualPageNumber, loadingCircleHaver);
    } else if (searching) {
        // fetch more (if search wasn't canceled)
        const std::string apiSearchQuery = createFullSearchQuery(lastSearchParameters);
        auto req = web::WebRequest();
        req.header("User-Agent", Utils::getUserAgent());
        searchTaskHolder.spawn(req.get(apiSearchQuery), getSearchLambda(pageNumber, callingLayer, loadingCircleHaver));
    }
}

Result<std::vector<int>> SearchObject::parseApiResponse(const std::string& response) {
    std::vector<int> listOfIds;
    if (response.empty()) {
        return Err("Server returned empty response");
    }
    const auto maybeResponseJson = matjson::parse(response);
    if (maybeResponseJson.isErr()) {
        return Err("Server returned invalid JSON");
    }
    const matjson::Value& responseJson = maybeResponseJson.unwrap();
    if (!responseJson.contains("levels") || !responseJson["levels"].isArray() || !responseJson.contains("total") || !responseJson["total"].isNumber()) {
        return Err("Server returned invalid response");
    }
    totalApiResultsCount = std::max(totalApiResultsCount, static_cast<long long>(responseJson["total"].asInt().unwrap()));
    for (auto level : responseJson["levels"].asArray().unwrap()) {
        if (!level.contains("ID") || !level["ID"].isNumber()) {
            return Err("Server returned invalid response: missing ID key in a level object");
        }
        const int levelID = level["ID"].asInt().unwrap();
        // levels with ID lower than that are official demons and therefore cannot be displayed
        if (levelID > 3) {
            listOfIds.push_back(levelID);
        }
        // optional step: if rating is present, update cache
        if (level.contains("Rating") && level["Rating"].isExactlyDouble()) {
            const float rating = level["Rating"].asDouble().unwrap();
            RatingsManager::updateCacheFromSearch(levelID, rating);
        }
        ++apiResultsProcessedCount;
    }
    return Ok(listOfIds);
}

std::vector<int> SearchObject::filterResults(std::vector<int> parsedResponse, const bool includeCompleted,
    const bool includeUncompleted) {
    if (includeCompleted == includeUncompleted) {
        // just return everything
        return parsedResponse;
    }
    // get the list of completed in parsed response
    std::vector<int> completedFromParsed;
    GameLevelManager *levelManager = GameLevelManager::sharedState();
    CCArray *completedLevels = levelManager->getCompletedLevels(false);
    for (const auto level : CCArrayExt<GJGameLevel*>(completedLevels)) {
        const bool levelCompleted = level->m_normalPercent == 100;
        if (levelCompleted && std::ranges::find(parsedResponse, static_cast<int>(level->m_levelID)) != parsedResponse.end()) {
            completedFromParsed.push_back(level->m_levelID);
        }
    }
    // filter
    if (includeCompleted) {
        std::vector<int> orderedCompletedFromParsed;
        for (const auto& id : parsedResponse) {
            if (std::ranges::find(completedFromParsed, id) != completedFromParsed.end()) {
                orderedCompletedFromParsed.push_back(id);
            }
        }
        return orderedCompletedFromParsed;
    }
    for (const auto& id : completedFromParsed) {
        std::erase(parsedResponse, id);
    }
    return parsedResponse;
}

bool SearchObject::isPageReady(const int pageNumber, const std::vector<int>& filteredResults) const {
    if (totalApiResultsCount <= apiResultsProcessedCount && apiPagesFetched > 0) {
        // already fetched everything there is to fetch
        return true;
    }
    return filteredResults.size() >= (pageNumber + 1) * inGameResultsPageSize;
}

std::function<void(web::WebResponse)> SearchObject::getSearchLambda(int requestedPage, GDDLLevelBrowserLayer* callingLayer, ILoadingCircleHaver* loadingCircleHaver) {
    return [this, requestedPage, callingLayer, loadingCircleHaver](web::WebResponse res) {
        if (res.code() != 200) {
            const auto jsonResponse = res.json().unwrapOr(matjson::Value());
            const std::string errorMessage = "GDDL: Search failed - " + Utils::getErrorFromMessageAndResponse(jsonResponse, res);
            Notification::create(errorMessage, NotificationIcon::Error, 2)->show();
            const std::string rawResponse = jsonResponse.contains("message") ? jsonResponse.dump(0) : res.string().unwrapOr("Response was not a valid string");
            log::error("SearchObject::getSearchLambda: [{}] {}, raw response: {}", res.code(), errorMessage, rawResponse);
            endSearch(loadingCircleHaver);
            return;
        }
        Result<std::vector<int>> parsedResponse = parseApiResponse(res.string().unwrapOrDefault());
        if (parsedResponse.isErr()) {
            const std::string errorMessage = "GDDL: Search failed - " + parsedResponse.unwrapErr();
            Notification::create(errorMessage, NotificationIcon::Error, 2)->show();
            log::error("SearchObject::getSearchLambda: {}", parsedResponse.unwrapErr());
            endSearch(loadingCircleHaver);
            return;
        }
        for (const auto id : parsedResponse.unwrap()) {
            results.push_back(id);
        }
        ++apiPagesFetched;
        // const std::vector<int> filteredResults = filterResults(parsedResponse.unwrap(), completedSetting->getSettingValue(), uncompletedSetting->getSettingValue());
        getSearchResultsForPage(requestedPage, callingLayer, loadingCircleHaver);
    };
}

void SearchObject::forwardToLevelBrowser(GJSearchObject* gjSearchObject, GDDLLevelBrowserLayer* callingLayer, int actualPageNumber, ILoadingCircleHaver*
                                         loadingCircleHaver) {
    if (!searching) return; // search can be canceled externally by the browser layer which might no longer exist
    endSearch(loadingCircleHaver);
    if (callingLayer != nullptr) {
        callingLayer->handleSearchObject(gjSearchObject, actualPageNumber);
        return;
    }
    const auto listLayer = static_cast<GDDLLevelBrowserLayer*>(GDDLLevelBrowserLayer::create(gjSearchObject));
    listLayer->assignSearchObject(this);
    const auto listLayerScene = CCScene::create();
    listLayerScene->addChild(listLayer);
    const auto transition = CCTransitionFade::create(0.5, listLayerScene);
    CCDirector::sharedDirector()->pushScene(transition);
}

void SearchObject::endSearch(ILoadingCircleHaver* loadingCircleHaver) {
    if (loadingCircleHaver != nullptr) {
        loadingCircleHaver->hideLoadingCircle();
    }
    searching = false;
}

void SearchObject::loadSettings() {
    for (const auto& setting : settings) {
        setting->loadSetting();
    }
}

void SearchObject::saveSettings() {
    for (const auto& setting : settings) {
        setting->saveSetting();
    }
}

void SearchObject::resetToDefaults() {
    for (const auto& setting : settings) {
        setting->resetToDefault();
    }
}

void SearchObject::performInitialSearch(ILoadingCircleHaver* loadingCircleHaver) {
    if (searching) return;
    searching = true;
    const std::string searchParameters = createSearchParametersString();
    if (searchParameters != lastSearchParameters) {
        // query has changed, we can clear the list of results
        results.clear();
        lastSearchParameters = searchParameters;
        apiPagesFetched = 0;
        totalApiResultsCount = 0;
        apiResultsProcessedCount = 0;
    }
    getSearchResultsForPage(0, nullptr, loadingCircleHaver);
}

void SearchObject::requestSearchPage(int pageNumber, GDDLLevelBrowserLayer* callingLayer) {
    if (searching) return;
    searching = true;
    getSearchResultsForPage(pageNumber, callingLayer, nullptr);
}

void SearchObject::cancelSearch() {
    searchTaskHolder.cancel();
    searching = false;
}

int SearchObject::getTotalApiResultsCount() {
    return this->totalApiResultsCount;
}

int SearchObject::getTotalApiResultsPageCount() {
    return this->totalApiResultsCount % inGameResultsPageSize == 0 ? this->totalApiResultsCount / inGameResultsPageSize : this->totalApiResultsCount / inGameResultsPageSize + 1;
}

int SearchObject::getPageCountOf(const std::vector<int>& vec) {
    return vec.size() % inGameResultsPageSize == 0 ? vec.size() / inGameResultsPageSize : vec.size() / inGameResultsPageSize + 1;
}

std::string SearchObject::getPageCountText(const int pageNumber) {
    const int firstLevel = pageNumber * 10 + 1;
    if (totalApiResultsCount <= apiResultsProcessedCount && apiPagesFetched > 0) {
        // we have everything already, we can answer
        const std::vector<int> filteredResults = filterResults(results, completedSetting->getSettingValue(), uncompletedSetting->getSettingValue());
        const int lastLevel = std::min(firstLevel + 9, static_cast<int>(filteredResults.size()));
        return fmt::format("{} to {} of {}", firstLevel, lastLevel, filteredResults.size());
    }
    // we can only answer based on the max estimate
    const int lastLevel = std::min(firstLevel + 9, static_cast<int>(totalApiResultsCount));
    return fmt::format("{} to {} of max. {}", firstLevel, lastLevel, totalApiResultsCount);
}

bool SearchObject::shouldShowRightArrow(const int pageNumber) {
    if (totalApiResultsCount <= apiResultsProcessedCount && apiPagesFetched > 0) {
        // we have everything already, we can answer based on filtered
        const std::vector<int> filteredResults = filterResults(results, completedSetting->getSettingValue(), uncompletedSetting->getSettingValue());
        const int pageCount = getPageCountOf(filteredResults);
        return pageNumber < pageCount - 1;
    }
    // we can only answer based on the max estimate
    const int pageCount = getTotalApiResultsPageCount();
    return pageNumber < pageCount - 1;
}

bool SearchObject::isSearching() {
    return this->searching;
}

std::shared_ptr<EnumSearchSetting> SearchObject::getSortSetting() {
    return this->sortSetting;
}

std::shared_ptr<EnumSearchSetting> SearchObject::getSortDirectionSetting() {
    return this->sortDirectionSetting;
}

std::shared_ptr<TextSearchSetting> SearchObject::getLevelNameSetting() {
    return this->levelNameSetting;
}

std::shared_ptr<TextSearchSetting> SearchObject::getCreatorNameSetting() {
    return this->creatorNameSetting;
}

std::shared_ptr<TextSearchSetting> SearchObject::getSongNameSetting() {
    return this->songNameSetting;
}

std::shared_ptr<RangeSearchSetting<int>> SearchObject::getRatingsSetting() {
    return this->ratingsSetting;
}

std::shared_ptr<RangeSearchSetting<float>> SearchObject::getEnjoymentsSetting() {
    return this->enjoymentsSetting;
}

std::shared_ptr<RangeSearchSetting<int>> SearchObject::getEnjoymentsCountSetting() {
    return this->enjoymentsCountSetting;
}

std::shared_ptr<RangeSearchSetting<float>> SearchObject::getDeviationsSetting() {
    return this->deviationsSetting;
}

std::shared_ptr<RangeSearchSetting<int>> SearchObject::getRatingsCountSetting() {
    return this->ratingsCountSetting;
}

std::shared_ptr<EnumSearchSetting> SearchObject::getDifficultySetting() {
    return this->difficultySetting;
}

std::shared_ptr<RangeSearchSetting<int>> SearchObject::getIdsRangeSetting() {
    return this->idsRangeSetting;
}

std::shared_ptr<EnumSearchSetting> SearchObject::getLengthSetting() {
    return this->lengthSetting;
}

std::shared_ptr<EnumSearchSetting> SearchObject::getTwoPlayerSetting() {
    return this->twoPlayerSetting;
}

std::shared_ptr<BoolSearchSetting> SearchObject::getIsInPackSetting() {
    return this->isInPackSetting;
}

std::shared_ptr<EnumSearchSetting> SearchObject::getTopSkillsetSetting() {
    return this->topSkillsetSetting;
}

std::shared_ptr<EnumSearchSetting> SearchObject::getHasSkillsetSetting() {
    return this->hasSkillsetSetting;
}

std::shared_ptr<BoolSearchSetting> SearchObject::getCompletedSetting() {
    return this->completedSetting;
}

std::shared_ptr<BoolSearchSetting> SearchObject::getUncompletedSetting() {
    return this->uncompletedSetting;
}

std::shared_ptr<BoolSearchSetting> SearchObject::getExcludeRatedSetting() {
    return this->excludeRatedSetting;
}

std::shared_ptr<BoolSearchSetting> SearchObject::getExcludeRatedEnjoymentSetting() {
    return this->excludeRatedEnjoymentSetting;
}

std::shared_ptr<BoolSearchSetting> SearchObject::getExcludeUnratedSetting() {
    return this->excludeUnratedSetting;
}

std::shared_ptr<BoolSearchSetting> SearchObject::getExcludeUnratedEnjoymentSetting() {
    return this->excludeUnratedEnjoymentSetting;
}
