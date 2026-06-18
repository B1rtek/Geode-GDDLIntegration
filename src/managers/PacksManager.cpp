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
    if (!Utils::fileExists(packsCachePath)) {
        return;
    }
    std::ifstream f(packsCachePath);
    if (Utils::fileIsEmpty(f)) {
        return;
    }
    std::stringstream content;
    content << f.rdbuf();
    const auto maybeData = matjson::parse(content.str());
    if (maybeData.isErr()) {
        return;
    }
    const matjson::Value& data = maybeData.unwrap();
    lastRefreshTimestamp = data["last-packlist-refresh"].asInt().unwrapOr(0);
    const unsigned int currentTimestamp = Utils::getCurrentTimestamp();
    if (currentTimestamp - lastRefreshTimestamp < 86400 * 7) {
        // cache still valid (not older than 7 days), load it
        // categories
        const auto categoriesListMaybe = data["categories"].asArray();
        if (categoriesListMaybe.isErr()) {
            log::error("PacksManager::populateFromSave: failed to read categories");
            return;
        }
        const std::vector<matjson::Value>& categoriesList = categoriesListMaybe.unwrap();
        for (const matjson::Value& entry : categoriesList) {
            const auto maybeCategoryInfo = getPackCategoryInfoFromJson(entry);
            if (maybeCategoryInfo.isErr()) {
                log::warn("PacksManager::populateFromSave: encountered invalid category entry");
            } else {
                const PackCategoryInfo categoryInfo = maybeCategoryInfo.unwrap();
                packCategoryMap[categoryInfo.getId()] = categoryInfo;
            }
        }
        // packs
        const auto packsListMaybe = data["packs"].asArray();
        if (packsListMaybe.isErr()) {
            log::error("PacksManager::populateFromSave: failed to read packs");
            return;
        }
        const std::vector<matjson::Value>& packsList = packsListMaybe.unwrap();
        for (const matjson::Value& entry : packsList) {
            const auto maybePackInfo = getPackInfoFromJson(entry);
            if (maybePackInfo.isErr()) {
                log::warn("PacksManager::populateFromSave: encountered invalid pack entry");
            } else {
                const std::shared_ptr<PackInfo> packInfo = maybePackInfo.unwrap();
                packsMap[packInfo->getId()] = packInfo;
            }
        }
    }
}

// called only on game quit
void PacksManager::dumpToSave() {
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
}

Result<std::shared_ptr<PackInfo>> PacksManager::getOrRequestPackInfo(int packID) {
    if (packsMap.contains(packID)) {
        return Ok(packsMap[packID]);
    }
    // we don't have it, prepare and send the request
    return Err("Pack not saved");
}

Result<PackCategoryInfo> PacksManager::getOrRequestPackCategoryInfo(int categoryID) {
    if (!readCache) {
        populateFromSave();
        readCache = true;
    }
    if (packCategoryMap.contains(categoryID)) {
        return Ok(packCategoryMap[categoryID]);
    }
    // we don't have it, prepare and send the request
    auto req = web::WebRequest();
    req.header("User-Agent", Utils::getUserAgent());
    packsTaskHolder.spawn(req.get(packsRequestApiUrl), getPacksDownloadLambda());
    return Err("Category not saved");
}

std::function<void(web::WebResponse)> PacksManager::getPacksDownloadLambda() {
    return [](web::WebResponse res) {
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
            log::info("invalid outer json");
            return;
        }
        for (const auto& packCategoryInfoObject : jsonResponse["categories"].asArray().unwrap()) {
            const Result<PackCategoryInfo> maybePackCategoryInfo = getPackCategoryInfoFromJson(packCategoryInfoObject);
            if (maybePackCategoryInfo.isErr()) {
                // TODO error
                log::info("invalid inner category json: {}", packCategoryInfoObject.dump());
                return;
            }
            const PackCategoryInfo& packCategoryInfo = maybePackCategoryInfo.unwrap();
            packCategoryMap[packCategoryInfo.getId()] = packCategoryInfo;
        }
        for (const auto& packInfoObject : jsonResponse["packs"].asArray().unwrap()) {
            const Result<std::shared_ptr<PackInfo>> maybePackInfo = getPackInfoFromJson(packInfoObject);
            if (maybePackInfo.isErr()) {
                // TODO error
                log::info("invalid inner pack json: {}", packInfoObject.dump());
                return;
            }
            const std::shared_ptr<PackInfo> packInfo = maybePackInfo.unwrap();
            packsMap[packInfo->getId()] = packInfo;
        }
        lastRefreshTimestamp = Utils::getCurrentTimestamp();
        notifyObservers();
    };
}

// for both reading cache and parsing api response
Result<std::shared_ptr<PackInfo>> PacksManager::getPackInfoFromJson(const matjson::Value& json) {
    if (!json.isObject() ||
        !json.contains("ID") || !json["ID"].isNumber() ||
        !json.contains("CategoryID") || !json["CategoryID"].isNumber() ||
        !json.contains("Name") || !json["Name"].isString() ||
        !json.contains("Description") || !json["Description"].isString()) {
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
    return Ok(std::make_shared<PackInfo>(json["ID"].asInt().unwrap(), json["CategoryID"].asInt().unwrap(), json["Name"].asString().unwrap(), json["Description"].asString().unwrap(), iconName, medianTier, lastSaveTimestamp));
}

// for both reading cache and parsing api response
Result<PackCategoryInfo> PacksManager::getPackCategoryInfoFromJson(const matjson::Value& json) {
    if (!json.contains("ID") || !json["ID"].isNumber() ||
        !json.contains("Name") || !json["Name"].isString() ||
        !json.contains("Description") || !json["Description"].isString()) {
        return Err("Invalid category JSON");
    }
    return Ok(PackCategoryInfo(json["ID"].asInt().unwrap(), json["Name"].asString().unwrap(), json["Description"].asString().unwrap()));
}

int PacksManager::getCategoryCount() {
    return packCategoryMap.size();
}

std::vector<std::shared_ptr<PackInfo>> PacksManager::getPacksFromCategory(const int categoryID) {
    std::vector<std::shared_ptr<PackInfo>> packsToReturn;
    for (const auto [id, packInfo] : packsMap) {
        if (packInfo->getCategoryId() == categoryID) {
            packsToReturn.push_back(packInfo);
        }
    }
    return packsToReturn;
}

void PacksManager::subscribeToObservers(IApiResponseObserver* newSubscriber) {
    packUpdateObservers.insert(newSubscriber);
}

void PacksManager::unsubscribeFromObservers(IApiResponseObserver* unsubscribing) {
    packUpdateObservers.erase(unsubscribing);
}

void PacksManager::notifyObservers() {
    for (const auto observer: packUpdateObservers) {
        observer->updateData();
    }
}
