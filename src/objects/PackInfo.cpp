#include "PackInfo.h"

#include <Utils.h>
#include <Geode/ui/Notification.hpp>
#include <modified/GDDLPackLevelBrowser.h>

void PackInfo::forwardToLevelBrowser(GJSearchObject* gjSearchObject, GDDLPackLevelBrowser* callingLayer,
    const int actualPageNumber) {
    if (callingLayer != nullptr) {
        callingLayer->handleSearchObject(gjSearchObject, actualPageNumber);
        return;
    }
    const auto levelBrowserLayer = static_cast<GDDLPackLevelBrowser*>(GDDLPackLevelBrowser::create(gjSearchObject));
    levelBrowserLayer->assignPackID(id);
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
                   const int medianTier, const int lastSaveTimestamp = 0): id(id), categoryId(categoryId), name(name), description(description), iconPath(iconPath), medianTier(medianTier), lastSaveTimestamp(lastSaveTimestamp) {
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

void PackInfo::clearLevelList() {
    levels.clear();
    extraLevels.clear();
}

void PackInfo::addLevel(const int levelID, const bool isExtra) {
    levels.push_back(levelID);
    if (isExtra) {
        extraLevels.insert(levelID);
    }
}

void PackInfo::replaceLevels(std::vector<int> levels, std::set<int> extraLevels) {
    this->levels = levels;
    this->extraLevels = extraLevels;
}

void PackInfo::updateLastSaveTimestamp() {
    lastSaveTimestamp = Utils::getCurrentTimestamp();
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

std::set<int> PackInfo::getExtraMap() const {
    return extraLevels;
}

bool PackInfo::isExtra(const int levelId) const {
    return extraLevels.contains(levelId);
}

int PackInfo::getLastSaveTimestamp() const {
    return lastSaveTimestamp;
}
