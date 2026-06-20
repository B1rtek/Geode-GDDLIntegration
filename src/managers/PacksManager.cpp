#include "PacksManager.h"

#include <Utils.h>

/**
* Cached json format:
{
  "packs": [
    {
      "ID": 1,
      "CategoryID": 3,
      "Name": "RobTop Pack",
      "Description": "something",
      "IconName": "something",
      "last-pack-refresh": 2137,
      "Meta": {
        "MedianTier": 40
      },
      "levels": [
        {
          "LevelID": 98322118,
          "EX": false
        }
      ]
    }
  ],
  "categories": [
    {
      "ID": 1,
      "Name": "Category Packs",
      "Description": "These are packs consisting of levels named after related things"
    }
  ],
  "last-packlist-refresh": 2137
}
 *
 */
void PacksManager::populateFromSave() {
    log::info("[PacksManager::populateFromSave] Called");
    if (!Utils::fileExists(packsCachePath)) {
        return;
    }
    log::info("[PacksManager::populateFromSave] File exists");
    std::ifstream f(packsCachePath);
    if (Utils::fileIsEmpty(f)) {
        return;
    }
    log::info("[PacksManager::populateFromSave] File is not empty");
    std::stringstream content;
    content << f.rdbuf();
    const auto maybeData = matjson::parse(content.str());
    if (maybeData.isErr()) {
        return;
    }
    log::info("[PacksManager::populateFromSave] Data is a valid JSON");
    const matjson::Value& data = maybeData.unwrap();
    lastRefreshTimestamp = data["last-packlist-refresh"].asInt().unwrapOr(0);
    const unsigned int currentTimestamp = Utils::getCurrentTimestamp();
    if (currentTimestamp - lastRefreshTimestamp < 86400 * 7) {
        // cache still valid (not older than 7 days), load it
        // categories
        const auto categoriesListMaybe = data["categories"].asArray();
        if (categoriesListMaybe.isErr()) {
            log::error("[PacksManager::populateFromSave] failed to read categories");
            return;
        }
        const std::vector<matjson::Value>& categoriesList = categoriesListMaybe.unwrap();
        for (const matjson::Value& entry : categoriesList) {
            const auto maybeCategoryInfo = getPackCategoryInfoFromJson(entry);
            if (maybeCategoryInfo.isErr()) {
                log::warn("[PacksManager::populateFromSave] encountered invalid category entry");
            } else {
                const PackCategoryInfo categoryInfo = maybeCategoryInfo.unwrap();
                packCategoryMap[categoryInfo.getId()] = categoryInfo;
            }
        }
        // packs
        const auto packsListMaybe = data["packs"].asArray();
        if (packsListMaybe.isErr()) {
            log::error("[PacksManager::populateFromSave] failed to read packs");
            return;
        }
        const std::vector<matjson::Value>& packsList = packsListMaybe.unwrap();
        for (const matjson::Value& entry : packsList) {
            const auto maybePackInfo = getPackInfoFromJson(entry);
            if (maybePackInfo.isErr()) {
                log::warn("[PacksManager::populateFromSave] encountered invalid pack entry");
            } else {
                const std::shared_ptr<PackInfo> packInfo = maybePackInfo.unwrap();
                packsMap[packInfo->getId()] = packInfo;
                if (entry.contains("levels") && entry["levels"].isArray()) {
                    const auto maybeWithLevels = getPackLevelsFromJson(entry["levels"].asArray().unwrap(), packInfo->getId());
                    if (maybeWithLevels.isOk()) {
                        packsMap[packInfo->getId()] = maybeWithLevels.unwrap();
                    }
                }
            }
        }
    }
    log::info("[PacksManager::populateFromSave] Finished successfully");
}

// called only on game quit
void PacksManager::dumpToSave() {
    log::info("[PacksManager::dumpToSave] Called");
    if (packCategoryMap.empty()) {
        // if we have nothing to save, do not save anything, do not overwrite potentially correct data
        return;
    }
    matjson::Value packsCache;
    packsCache["last-packlist-refresh"] = lastRefreshTimestamp;
    // categories
    std::vector<matjson::Value> categoriesList;
    for (auto [id, categoryInfo]: packCategoryMap) {
        matjson::Value categoryInfoJson;
        categoryInfoJson["ID"] = categoryInfo.getId();
        categoryInfoJson["Name"] = categoryInfo.getName();
        categoryInfoJson["Description"] = categoryInfo.getDescription();
        categoriesList.push_back(categoryInfoJson);
    }
    packsCache["categories"] = categoriesList;
    // packs
    std::vector<matjson::Value> packsList;
    for (auto [id, packInfo] : packsMap) {
        matjson::Value packInfoJson;
        packInfoJson["ID"] = packInfo->getId();
        packInfoJson["CategoryID"] = packInfo->getCategoryId();
        packInfoJson["Name"] = packInfo->getName();
        packInfoJson["Description"] = packInfo->getDescription();
        packInfoJson["IconName"] = packInfo->getIconPath();
        packInfoJson["last-pack-refresh"] = packInfo->getLastSaveTimestamp();
        matjson::Value meta;
        meta["MedianTier"] = packInfo->getMedianTier();;
        packInfoJson["Meta"] = meta;
        std::vector<matjson::Value> levelList;
        for (const auto levelID : packInfo->getLevels()) {
            matjson::Value levelInfoJson;
            levelInfoJson["LevelID"] = levelID;
            levelInfoJson["EX"] = packInfo->isExtra(levelID);
            levelList.push_back(levelInfoJson);
        }
        packInfoJson["levels"] = levelList;
        packsList.push_back(packInfoJson);
    }
    packsCache["packs"] = packsList;
    // save it all
    std::fstream packsCacheFile;
    packsCacheFile.open(packsCachePath, std::ios::out);
    packsCacheFile << packsCache.dump();
    packsCacheFile.close();
    log::info("[PacksManager::dumpToSave] Finished");
}

Result<std::shared_ptr<PackInfo>> PacksManager::getOrRequestPackInfo(int packID, bool withLevels) {
    log::info("[PacksManager::getOrRequestPackInfo] Called with packID={}, withLevels={}", packID, withLevels);
    if (packsMap.contains(packID)) {
        log::info("[PacksManager::getOrRequestPackInfo] Pack in cache");
        if (!withLevels || !packsMap[packID]->getLevels().empty()) {
            log::info("[PacksManager::getOrRequestPackInfo] Pack satisfies the criteria");
            return Ok(packsMap[packID]);
        }
    }
    log::info("[PacksManager::getOrRequestPackInfo] Pack not cached or empty");
    // we don't have it, prepare and send the request
    // TODO for now only handles downloading levels, not updating pack definition if it's outdated
    auto req = web::WebRequest();
    req.header("User-Agent", Utils::getUserAgent());
    packLevelsTaskHolder.spawn(req.get(getPackLevelsDownloadUrl(packID)), getPackLevelsDownloadLambda(packID));
    log::info("[PacksManager::getOrRequestPackInfo] Spawned packLevelsTaskHolder");
    return Err("Pack not saved");
}

Result<PackCategoryInfo> PacksManager::getOrRequestPackCategoryInfo(int categoryID) {
    log::info("[PacksManager::getOrRequestPackCategoryInfo] Called with categoryID={}", categoryID);
    if (!readCache) {
        log::info("[PacksManager::getOrRequestPackCategoryInfo] Reading cache");
        populateFromSave();
        readCache = true;
    }
    if (packCategoryMap.contains(categoryID)) {
        log::info("[PacksManager::getOrRequestPackCategoryInfo] PackCategory in cache");
        return Ok(packCategoryMap[categoryID]);
    }
    log::info("[PacksManager::getOrRequestPackCategoryInfo] PackCategory not cached");
    // we don't have it, prepare and send the request
    auto req = web::WebRequest();
    req.header("User-Agent", Utils::getUserAgent());
    packsTaskHolder.spawn(req.get(packsRequestApiUrl), getPacksDownloadLambda(false));
    log::info("[PacksManager::getOrRequestPackCategoryInfo] Spawned packsTaskHolder");
    return Err("Category not saved");
}

void PacksManager::requestPackListRefresh() {
    log::info("[PacksManager::requestPackListRefresh] Called");
    auto req = web::WebRequest();
    req.header("User-Agent", Utils::getUserAgent());
    packsTaskHolder.spawn(req.get(packsRequestApiUrl), getPacksDownloadLambda(true));
    log::info("[PacksManager::requestPackListRefresh] Spawned packsTaskHolder");
}

std::function<void(web::WebResponse)> PacksManager::getPacksDownloadLambda(bool refresh) {
    return [refresh](web::WebResponse res) {
        log::info("[(res@{})PacksManager::getPacksDownloadLambda] Callback with refresh={}", fmt::ptr(std::addressof(res)), refresh);
        if (res.code() != 200) {
            // const auto jsonResponse = res.json().unwrapOr(matjson::Value());
            // const std::string errorMessage = "GDDL: Search failed - " + Utils::getErrorFromMessageAndResponse(jsonResponse, res);
            // Notification::create(errorMessage, NotificationIcon::Error, 2)->show();
            // const std::string rawResponse = jsonResponse.contains("message") ? jsonResponse.dump(0) : res.string().unwrapOr("Response was not a valid string");
            // log::error("SearchObject::getSearchLambda: [{}] {}, raw response: {}", res.code(), errorMessage, rawResponse);
            return;
        }
        const auto jsonResponse = res.json().unwrapOr(matjson::Value());
        if (!jsonResponse.contains("packs") || !jsonResponse["packs"].isArray() ||
            !jsonResponse.contains("categories") || !jsonResponse["categories"].isArray()) {
            // TODO error
            log::info("[(res@{})PacksManager::getPacksDownloadLambda] invalid outer json", fmt::ptr(std::addressof(res)));
            return;
        }
        for (const auto& packCategoryInfoObject : jsonResponse["categories"].asArray().unwrap()) {
            const Result<PackCategoryInfo> maybePackCategoryInfo = getPackCategoryInfoFromJson(packCategoryInfoObject);
            if (maybePackCategoryInfo.isErr()) {
                // TODO error
                log::info("[(res@{})PacksManager::getPacksDownloadLambda] invalid inner category json: {}", fmt::ptr(std::addressof(res)), packCategoryInfoObject.dump());
                return;
            }
            const PackCategoryInfo& packCategoryInfo = maybePackCategoryInfo.unwrap();
            packCategoryMap[packCategoryInfo.getId()] = packCategoryInfo;
        }
        for (const auto& packInfoObject : jsonResponse["packs"].asArray().unwrap()) {
            const Result<std::shared_ptr<PackInfo>> maybePackInfo = getPackInfoFromJson(packInfoObject);
            if (maybePackInfo.isErr()) {
                // TODO error
                log::info("[(res@{})PacksManager::getPacksDownloadLambda] invalid inner pack json: {}", fmt::ptr(std::addressof(res)), packInfoObject.dump());
                return;
            }
            const std::shared_ptr<PackInfo> packInfo = maybePackInfo.unwrap();
            if (refresh && packsMap.contains(packInfo->getId())) {
                const auto oldPackInfo = packsMap[packInfo->getId()];
                packInfo->replaceLevels(oldPackInfo->getLevels(), oldPackInfo->getExtraMap());
            }
            packsMap[packInfo->getId()] = packInfo;
        }
        lastRefreshTimestamp = Utils::getCurrentTimestamp();
        log::info("[(res@{})PacksManager::getPacksDownloadLambda] Success, notifying observers", fmt::ptr(std::addressof(res)));
        notifyObservers();
    };
}

std::function<void(web::WebResponse)> PacksManager::getPackLevelsDownloadLambda(const int packID) {
    return [packID](web::WebResponse res) {
        log::info("[(res@{})PacksManager::getPackLevelsDownloadLambda] Callback with packID={}", fmt::ptr(std::addressof(res)), packID);
        if (res.code() != 200) {
            // TODO error
            log::info("[(res@{})PacksManager::getPackLevelsDownloadLambda] HTTP code {}", fmt::ptr(std::addressof(res)), res.code());
            return;
        }
        const auto jsonResponse = res.json().unwrapOr(matjson::Value());
        if (!jsonResponse.isArray()) {
            // TODO error
            log::info("[(res@{})PacksManager::getPackLevelsDownloadLambda] Did not receive a JSON array", fmt::ptr(std::addressof(res)));
            return;
        }
        const auto maybeUpdatedPack = getPackLevelsFromJson(jsonResponse, packID);
        if (maybeUpdatedPack.isErr()) {
            // TODO error
            log::info("[(res@{})PacksManager::getPackLevelsDownloadLambda] Could not create PackInfo from received JSON", fmt::ptr(std::addressof(res)));
            return;
        }
        const std::shared_ptr<PackInfo> packInfo = maybeUpdatedPack.unwrap();
        packInfo->updateLastSaveTimestamp();
        packsMap[packID] = packInfo;
        log::info("[(res@{})PacksManager::getPackLevelsDownloadLambda] Success, notifying observers", fmt::ptr(std::addressof(res)));
        notifyObservers();
    };
}

std::string PacksManager::getPackLevelsDownloadUrl(const int packID) {
    return packLevelsDownloadApiUrlBase + std::to_string(packID) + "/levels";
}

// for both reading cache and parsing api response
Result<std::shared_ptr<PackInfo>> PacksManager::getPackInfoFromJson(const matjson::Value& json) {
    log::info("[PacksManager::getPackInfoFromJson] Called");
    if (!json.isObject() ||
        !json.contains("ID") || !json["ID"].isNumber() ||
        !json.contains("CategoryID") || !json["CategoryID"].isNumber() ||
        !json.contains("Name") || !json["Name"].isString() ||
        !json.contains("Description") || !json["Description"].isString()) {
        log::info("[PacksManager::getPackInfoFromJson] Invalid PackInfo JSON");
        return Err("Invalid PackInfo JSON");
        }
    std::string iconName = "tier_unrated.png";
    if (json.contains("IconName") && json["IconName"].isString()) {
        iconName = json["IconName"].asString().unwrap();
    }
    int medianTier = -1;
    if (json.contains("Meta") && json["Meta"].isObject() && json["Meta"].contains("MedianTier") && json["Meta"]["MedianTier"].isNumber()) {
        medianTier = json["Meta"]["MedianTier"].asInt().unwrap();
    }
    int lastSaveTimestamp = Utils::getCurrentTimestamp();
    if (json.contains("last-pack-refresh") && json["last-pack-refresh"].isNumber()) {
        lastSaveTimestamp = json["last-pack-refresh"].asInt().unwrap();
    }
    log::info("[PacksManager::getPackInfoFromJson] Returning Ok");
    return Ok(std::make_shared<PackInfo>(json["ID"].asInt().unwrap(), json["CategoryID"].asInt().unwrap(), json["Name"].asString().unwrap(), json["Description"].asString().unwrap(), iconName, medianTier, lastSaveTimestamp));
}

// for both reading cache and parsing api response
Result<PackCategoryInfo> PacksManager::getPackCategoryInfoFromJson(const matjson::Value& json) {
    log::info("[PacksManager::getPackCategoryInfoFromJson] Called");
    if (!json.contains("ID") || !json["ID"].isNumber() ||
        !json.contains("Name") || !json["Name"].isString() ||
        !json.contains("Description") || !json["Description"].isString()) {
        log::info("[PacksManager::getPackCategoryInfoFromJson] Invalid category JSON");
        return Err("Invalid category JSON");
    }
    log::info("[PacksManager::getPackCategoryInfoFromJson] Returning Ok");
    return Ok(PackCategoryInfo(json["ID"].asInt().unwrap(), json["Name"].asString().unwrap(), json["Description"].asString().unwrap()));
}

Result<std::shared_ptr<PackInfo>> PacksManager::getPackLevelsFromJson(const matjson::Value& json, int packID) {
    log::info("[PacksManager::getPackLevelsFromJson] Called with packID={}", packID);
    if (!json.isArray()) {
        log::info("[PacksManager::getPackLevelsFromJson] JSON is not an array");
        return Err("JSON is not an array");
    }
    const std::shared_ptr<PackInfo> updatedPackInfo = packsMap[packID];
    log::info("[PacksManager::getPackLevelsFromJson] Clearing PackInfo levels");
    updatedPackInfo->clearLevelList();
    for (const auto& levelObject : json.asArray().unwrap()) {
        if (levelObject.isObject() && levelObject.contains("LevelID") && levelObject["LevelID"].isNumber() &&
            levelObject.contains("EX") && levelObject["EX"].isBool()) {
            updatedPackInfo->addLevel(levelObject["LevelID"].asInt().unwrap(), levelObject["EX"].asBool().unwrap());
        } else {
            // invalid json
            log::info("[PacksManager::getPackLevelsFromJson] Invalid level JSON object");
            return Err("Invalid level JSON object");
        }
    }
    log::info("[PacksManager::getPackLevelsFromJson] Returning Ok");
    return Ok(updatedPackInfo);
}

int PacksManager::getCategoryCount() {
    log::info("[PacksManager::getCategoryCount] Called");
    return packCategoryMap.size();
}

std::vector<std::shared_ptr<PackInfo>> PacksManager::getPacksFromCategory(const int categoryID) {
    log::info("[PacksManager::getPacksFromCategory] Called");
    std::vector<std::shared_ptr<PackInfo>> packsToReturn;
    for (const auto [id, packInfo] : packsMap) {
        if (packInfo->getCategoryId() == categoryID) {
            packsToReturn.push_back(packInfo);
        }
    }
    return packsToReturn;
}

void PacksManager::subscribeToObservers(IApiResponseObserver* newSubscriber) {
    log::info("[PacksManager::subscribeToObservers] Called with newSubscriber={}", fmt::ptr(newSubscriber));
    packUpdateObservers.insert(newSubscriber);
    log::info("[PacksManager::subscribeToObservers] {} subscribed", fmt::ptr(newSubscriber));
}

void PacksManager::unsubscribeFromObservers(IApiResponseObserver* unsubscribing) {
    log::info("[PacksManager::unsubscribeFromObservers] Called with unsubscribing={}", fmt::ptr(unsubscribing));
    packUpdateObservers.erase(unsubscribing);
    log::info("[PacksManager::unsubscribeFromObservers] {} unsubscribed", fmt::ptr(unsubscribing));
}

void PacksManager::notifyObservers() {
    log::info("[PacksManager::notifyObservers] Called");
    const auto observersCopy = packUpdateObservers; // like this we won't crash when observers are removed in the updateData() call, probably not the best idea
    for (const auto observer: observersCopy) {
        log::info("[PacksManager::notifyObservers] Notifying {}", fmt::ptr(observer));
        observer->updateData();
    }
}
