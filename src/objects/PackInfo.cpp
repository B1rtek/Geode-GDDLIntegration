#include "PackInfo.h"

#include <Utils.h>
#include <Geode/ui/Notification.hpp>
#include <modified/GDDLPackLevelBrowser.h>

std::function<void(web::WebResponse)> PackInfo::getPackDownloadLambda() {
    return [this](web::WebResponse res) {
        if (res.code() != 200) {
            // TODO error
            return;
        }
        const auto jsonResponse = res.json().unwrapOr(matjson::Value());
        if (!jsonResponse.isArray()) {
            // TODO error
            return;
        }
        for (const auto levelObject : jsonResponse.asArray().unwrap()) {
            if (!levelObject.isObject() || !levelObject.contains("LevelID") || !levelObject["LevelID"].isNumber()) {
                // TODO error
                return;
            }
            const int levelId = levelObject["LevelID"].asInt().unwrap();
            levels.push_back(levelId);
            // extra levels
            if (levelObject.contains("EX") && levelObject["EX"].isBool() && levelObject["EX"].asBool().unwrap()) {
                extraLevels.insert(levelId);
            }
        }
        // everything went well, forward to level browser
        GJSearchObject* gjSearchObject = Utils::createGJSearchObjectFromIndex(0, levels);
        forwardToLevelBrowser(gjSearchObject, nullptr, 0);
    };
}

std::string PackInfo::getPackDownloadUrl(int packId) {
    return packDownloadApiUrlBase + std::to_string(packId) + "/levels";
}

void PackInfo::forwardToLevelBrowser(GJSearchObject* gjSearchObject, GDDLPackLevelBrowser* callingLayer,
    const int actualPageNumber) {
    if (callingLayer != nullptr) {
        callingLayer->handleSearchObject(gjSearchObject, actualPageNumber);
        return;
    }
    const auto levelBrowserLayer = static_cast<GDDLPackLevelBrowser*>(GDDLPackLevelBrowser::create(gjSearchObject));
    levelBrowserLayer->assignPackInfo(this);
    const auto listLayerScene = CCScene::create();
    listLayerScene->addChild(levelBrowserLayer);
    const auto transition = CCTransitionFade::create(0.5, listLayerScene);
    CCDirector::sharedDirector()->pushScene(transition);
}

std::pair<int, int> PackInfo::calculateCompletionStats() const {
    int completedBase = 0, completedTotal = 0;
    GameLevelManager *levelManager = GameLevelManager::sharedState();
    CCArray *completedLevels = levelManager->getCompletedLevels(false);
    for (const auto level : CCArrayExt<GJGameLevel*>(completedLevels)) {
        const bool levelCompleted = level->m_normalPercent == 100;
        if (levelCompleted && std::ranges::find(levels, static_cast<int>(level->m_levelID)) != levels.end()) {
            ++completedTotal;
            if (!extraLevels.contains(static_cast<int>(level->m_levelID))) {
                ++completedBase;
            }
        }
    }
    return {completedBase, completedTotal};
}

PackInfo::PackInfo(const int id, const int categoryId, const std::string& name, const std::string& description, const std::string& iconPath,
                   const int medianTier): id(id), categoryId(categoryId), name(name), description(description), iconPath(iconPath), medianTier(medianTier) {
}

Result<std::shared_ptr<PackInfo>> PackInfo::createFromJson(const matjson::Value& json) {
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
    return Ok(std::make_shared<PackInfo>(json["ID"].asInt().unwrap(), json["CategoryID"].asInt().unwrap(), json["Name"].asString().unwrap(), json["Description"].asString().unwrap(), iconName, medianTier));
}

void PackInfo::downloadAndOpenPack() {
    if (levels.empty()) {
        auto req = web::WebRequest();
        req.header("User-Agent", Utils::getUserAgent());
        packDownloadTaskHolder.spawn(req.get(getPackDownloadUrl(id)), getPackDownloadLambda());
    } else {
        requestPage(0, nullptr);
    }
}

void PackInfo::requestPage(int pageNumber, GDDLPackLevelBrowser* callingLayer) {
    const int actualPageNumber = std::min(pageNumber, static_cast<int>(levels.size() % 10 == 0 ? levels.size() / 10 : levels.size() / 10 + 1));
    GJSearchObject* gjSearchObject = Utils::createGJSearchObjectFromIndex(actualPageNumber * 10, levels);
    forwardToLevelBrowser(gjSearchObject, callingLayer, actualPageNumber);
}

std::string PackInfo::getPageCountText(const int pageNumber) {
    const int firstLevel = pageNumber * 10 + 1;
    const int lastLevel = std::min(firstLevel + 9, static_cast<int>(levels.size()));
    return fmt::format("{} to {} of {}", firstLevel, lastLevel, levels.size());
}

bool PackInfo::shouldShowRightArrow(const int pageNumber) {
    const int pageCount = Utils::getPageCountOf(levels);
    return pageNumber < pageCount - 1;
}

std::pair<float, bool> PackInfo::getCompletionStatus() const {
    const auto [completedBase, completedTotal] = calculateCompletionStats();
    if (completedBase == levels.size() - extraLevels.size()) {
        return {100.0f * static_cast<float>(completedTotal) / static_cast<float>(levels.size()), true};
    }
    return {100.0f * static_cast<float>(completedBase) / static_cast<float>(levels.size() - extraLevels.size()), false};
}

std::pair<std::pair<int, int>, bool> PackInfo::getCompletedFraction() const {
    const auto [completedBase, completedTotal] = calculateCompletionStats();
    if (completedBase == levels.size() - extraLevels.size()) {
        return {{completedTotal, levels.size()}, true};
    }
    return {{completedBase, levels.size() - extraLevels.size()}, false};
}

int PackInfo::getId() const {
    return id;
}

int PackInfo::getCategoryId() const {
    return categoryId;
}

std::string PackInfo::getName() const {
    return name;
}

std::string PackInfo::getDescription() const {
    return description;
}

std::string PackInfo::getIconPath() const {
    return iconPath;
}

int PackInfo::getMedianTier() const {
    return medianTier;
}

std::vector<int> PackInfo::getLevels() const {
    return levels;
}

bool PackInfo::isExtra(const int levelId) const {
    return extraLevels.contains(levelId);
}
