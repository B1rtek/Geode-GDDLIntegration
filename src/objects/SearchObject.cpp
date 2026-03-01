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

GJSearchObject* SearchObject::createGJSearchObjectFromIndex(const unsigned long long firstIndex) const {
    std::string requestString;
    const unsigned lastIndex = std::min(firstIndex + inGameResultsPageSize, results.size());
    for (unsigned i = firstIndex; i < lastIndex; i++) {
        requestString += std::to_string(results[i]) + ',';
    }
    if (!requestString.empty()) {
        requestString.pop_back();
    }
    requestString += "&gameVersion=22";
    log::info("GJRequest: {}", requestString);
    return GJSearchObject::create(SearchType::Type19, requestString);
}

std::function<void(web::WebResponse)> SearchObject::getSearchLambda() {
    return [](web::WebResponse res) {

    };
}

void SearchObject::openLevelBrowser(GJSearchObject* gjSearchObject) {
    const auto listLayer = static_cast<GDDLLevelBrowserLayer*>(GDDLLevelBrowserLayer::create(gjSearchObject));
    listLayer->assignSearchObject(this);
    const auto listLayerScene = CCScene::create();
    listLayerScene->addChild(listLayer);
    const auto transition = CCTransitionFade::create(0.5, listLayerScene);
    CCDirector::sharedDirector()->pushScene(transition);
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

void SearchObject::performInitialSearch() {
    const std::string searchParameters = createSearchParametersString();
    if (searchParameters != lastSearchParameters) {
        // query has changed, we can clear the list of results
        results.clear();
        lastSearchParameters = searchParameters;
    }
    if (results.size() < 10 && apiResultsProcessedCount < totalApiResultsCount) {
        // fetch more
        const std::string apiSearchQuery = createFullSearchQuery(searchParameters);
        auto req = web::WebRequest();
        req.header("User-Agent", Utils::getUserAgent());
        searchTaskHolder.spawn(req.get(apiSearchQuery), getSearchLambda());
    } else {
        // display what we have
        GJSearchObject* gjSearcjObject = createGJSearchObjectFromIndex(0);
        openLevelBrowser(gjSearcjObject);
    }
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
