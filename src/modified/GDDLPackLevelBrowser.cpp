#include "GDDLPackLevelBrowser.h"

#include <Utils.h>
#include <Geode/binding/GJListLayer.hpp>
#include <layers/GDDLThemeBaseLayer.h>
#include <managers/PacksManager.h>

bool GDDLPackLevelBrowser::init(GJSearchObject* gjSearchObject) {
    if (!LevelBrowserLayer::init(gjSearchObject)) {
        return false;
    }
    log::info("[({})GDDLPackLevelBrowser::init] Called", fmt::ptr(this));
    m_fields->m_this = this;
    return true;
}

gd::string GDDLPackLevelBrowser::getSearchTitle() {
    if (m_fields->packID != 0) {
        log::info("[({})GDDLPackLevelBrowser::getSearchTitle] Called", fmt::ptr(this));
        const std::shared_ptr<PackInfo> packInfo = PacksManager::getOrRequestPackInfo(m_fields->packID, true).unwrap(); // should always be valid
        return packInfo->getName();
    }
    return LevelBrowserLayer::getSearchTitle();
}

void GDDLPackLevelBrowser::loadLevelsFinished(cocos2d::CCArray* p0, char const* p1, int p2) {
    LevelBrowserLayer::loadLevelsFinished(p0, p1, p2);
    if (m_fields->packID != 0) {
        log::info("[({})GDDLPackLevelBrowser::loadLevelsFinished] Called", fmt::ptr(this));
        updateAfterLoadLevelsFinished();
    }
}

void GDDLPackLevelBrowser::onNextPage(CCObject* sender) {
    if (m_fields->packID != 0) {
        log::info("[({})GDDLPackLevelBrowser::onNextPage] Called", fmt::ptr(this));
        const std::shared_ptr<PackInfo> packInfo = PacksManager::getOrRequestPackInfo(m_fields->packID, true).unwrap(); // should always be valid
        packInfo->requestPage(m_fields->currentPage + 1, this);
        hideOriginalTextures();
    } else {
        LevelBrowserLayer::onNextPage(sender);
    }
}

void GDDLPackLevelBrowser::onPrevPage(CCObject* sender) {
    if (m_fields->packID != 0) {
        log::info("[({})GDDLPackLevelBrowser::onPrevPage] Called", fmt::ptr(this));
        const std::shared_ptr<PackInfo> packInfo = PacksManager::getOrRequestPackInfo(m_fields->packID, true).unwrap(); // should always be valid
        packInfo->requestPage(m_fields->currentPage - 1, this);
        hideOriginalTextures();
    } else {
        LevelBrowserLayer::onPrevPage(sender);
    }
}

void GDDLPackLevelBrowser::onRefresh(CCObject* sender) {
    if (m_fields->packID != 0) {
        log::info("[({})GDDLPackLevelBrowser::onRefresh] Called", fmt::ptr(this));
        const std::shared_ptr<PackInfo> packInfo = PacksManager::getOrRequestPackInfo(m_fields->packID, true).unwrap(); // should always be valid
        packInfo->clearLevelList();
        // request again to trigger a web request
        log::info("[({})GDDLPackLevelBrowser::onRefresh] Requesting new packInfo", fmt::ptr(this));
        const auto err = PacksManager::getOrRequestPackInfo(m_fields->packID, true);
        // and now we wait, m_fields is always subscribed to changes
    } else {
        LevelBrowserLayer::onRefresh(sender);
    }
}

void GDDLPackLevelBrowser::setIDPopupClosed(SetIDPopup* popup, int value) {
    if (m_fields->packID != 0) {
        log::info("[({})GDDLPackLevelBrowser::setIDPopupClosed] Called", fmt::ptr(this));
        const std::shared_ptr<PackInfo> packInfo = PacksManager::getOrRequestPackInfo(m_fields->packID, true).unwrap(); // should always be valid
        log::info("[({})GDDLPackLevelBrowser::setIDPopupClosed] Requesting page {}", fmt::ptr(this), value - 1);
        packInfo->requestPage(value - 1, this);
        hideOriginalTextures();
    } else {
        LevelBrowserLayer::setIDPopupClosed(popup, value);
    }
}

void GDDLPackLevelBrowser::onEnter() {
    LevelBrowserLayer::onEnter();
    if (m_fields->packID != 0) {
        log::info("[({})GDDLPackLevelBrowser::onEnter] Called", fmt::ptr(this));
        createPackUI();
        updateAfterLoadLevelsFinished();
    }
}

void GDDLPackLevelBrowser::onInfo(CCObject* sender) {
    if (m_fields->packID != 0) {
        const std::shared_ptr<PackInfo> packInfo = PacksManager::getOrRequestPackInfo(m_fields->packID, true).unwrap(); // should always be valid
        const std::string description = "<cb>" + packInfo->getDescription() + "</c>\n\n<cp>Demons</c> which are <cy>not required</c> for pack completion are marked in <cr>red</c>. The progress bar counts only <co>progress towards required completions</c> until you <cg>complete the pack</c>, afterwards the progress bar will <cj>turn blue</c> and count <co>progress of the whole pack</c>.";
        FLAlertLayer::create(packInfo->getName().c_str(), description, "OK")->show();
    } else {
        LevelBrowserLayer::onInfo(sender);
    }
}

void GDDLPackLevelBrowser::handleSearchObject(GJSearchObject* gjSearchObject, const int actualPageNumber) {
    log::info("[({})GDDLPackLevelBrowser::handleSearchObject] Called", fmt::ptr(this));
    m_fields->currentPage = actualPageNumber;
    log::info("[({})GDDLPackLevelBrowser::handleSearchObject] Calling loadPage()", fmt::ptr(this));
    loadPage(gjSearchObject);
    setCorrectLabelsText();
}

void GDDLPackLevelBrowser::assignPackID(const int packID) {
    log::info("[({})GDDLPackLevelBrowser::assignPackID] Called with packID={}", fmt::ptr(this), packID);
    this->m_fields->packID = packID;
    updateAfterLoadLevelsFinished();
}

void GDDLPackLevelBrowser::createPackUI() {
    log::info("[({})GDDLPackLevelBrowser::createPackUI] Called", fmt::ptr(this));
    if (m_fields->firstOpen) {
        log::info("[({})GDDLPackLevelBrowser::createPackUI] First open, creating UI", fmt::ptr(this));
        const std::shared_ptr<PackInfo> packInfo = PacksManager::getOrRequestPackInfo(m_fields->packID, true).unwrap(); // should always be valid
        m_fields->firstOpen = false;
        // cursed things (fortunately they only happen once)
        log::info("[({})GDDLPackLevelBrowser::createPackUI] Updating list title", fmt::ptr(this));
        const auto listChildren = m_list->getChildren();
        for (const auto child : CCArrayExt<CCNode*>(listChildren)) {
            if (auto maybeLabel = typeinfo_cast<CCLabelBMFont*>(child)) {
                maybeLabel->setString(packInfo->getName().c_str());
                maybeLabel->setZOrder(11);
                // scale down so it doesn't take up the whole screen because it only scales after the whole thing loads for some reason
                maybeLabel->limitLabelWidth(280.0f, 0.8f, 0.2f);
            }
        }
        // sprite adding things
        log::info("[({})GDDLPackLevelBrowser::createPackUI] Adding pack sprites", fmt::ptr(this));
        for (int i = 0; i < 2; i++) {
            const auto packIcon = LazySprite::create({20.0f, 20.0f});
            packIcon->setLoadCallback([packIcon](Result<> res) {
                if (!res) {
                    packIcon->initWithFile(Mod::get()->expandSpriteName("tier_unrated.png").data());
                    packIcon->setScale(0.275f);
                }
            });
            packIcon->loadFromUrl(Values::packIconsBaseUrl + packInfo->getIconPath());
            packIcon->setPosition({m_list->getPositionX() - 4.0f + i * (m_list->getContentWidth() + 8.0f), m_list->getPositionY() + m_list->getContentHeight() + 17.0f});
            packIcon->setScale(1.25f);
            packIcon->setZOrder(11);
            this->addChild(packIcon);
        }
        // progress bar
        log::info("[({})GDDLPackLevelBrowser::createPackUI] Creating progress bar", fmt::ptr(this));
        m_fields->progressBar = ProgressBar::create(ProgressBarStyle::Solid);
        m_fields->progressBar->showProgressLabel(true);
        m_fields->progressBar->setScale(0.9f);
        m_fields->progressBar->setPrecision(2);
        m_fields->progressBar->setPosition({m_list->getPositionX() + m_list->getContentWidth() / 2 - m_fields->progressBar->getScaledContentWidth() / 2, m_list->getPositionY() - 15.0f});
        m_fields->progressBar->setZOrder(11);
        this->addChild(m_fields->progressBar);

        // now the really cursed stuff happens
        // hiding the original frame and bg
        log::info("[({})GDDLPackLevelBrowser::createPackUI] Hiding original textures", fmt::ptr(this));
        hideOriginalTextures();
        // placing the better frame
        log::info("[({})GDDLPackLevelBrowser::createPackUI] Placing GDDL theme frame", fmt::ptr(this));
        // sides
        const std::vector<CCPoint> sidePositions = {
            {m_list->getPositionX() - 7.5f, m_list->getContentHeight() / 2 + m_list->getPositionY()}, // left
            {m_list->getContentWidth() + m_list->getPositionX() + 7.5f, m_list->getContentHeight() / 2 + m_list->getPositionY()}, // right
        };
        for (int i = 0; i < sidePositions.size(); i++) {
            const auto sideSprite = CCSprite::create(Mod::get()->expandSpriteName("border_thin.png").data());
            sideSprite->setPosition(sidePositions[i]);
            sideSprite->setRotation((i - 1) * 180.0f + 90.0f);
            sideSprite->setScaleX(m_list->getContentHeight() / sideSprite->getContentWidth());
            sideSprite->setZOrder(10);
            this->addChild(sideSprite);
        }
        // top and bottom
        const auto topSprite = CCSprite::create(Mod::get()->expandSpriteName("border_upper_thick.png").data());
        topSprite->setPosition({m_list->getContentWidth() / 2 + m_list->getPositionX(), m_list->getContentHeight() + m_list->getPositionY() + topSprite->getContentHeight() / 2 - 10.0f});
        topSprite->setZOrder(-1);
        this->addChild(topSprite);
        const auto bottomSprite = CCSprite::create(Mod::get()->expandSpriteName("border_bottom_thick.png").data());
        bottomSprite->setPosition({m_list->getContentWidth() / 2 + m_list->getPositionX(), m_list->getPositionY() - bottomSprite->getContentHeight() / 2 + 20.0f});
        bottomSprite->setZOrder(10);
        this->addChild(bottomSprite);

        // gddl theme :tm: background
        log::info("[({})GDDLPackLevelBrowser::createPackUI] Adding GDDL theme background", fmt::ptr(this));
        GDDLThemeBaseLayer::createBackground(this);
    }
}

void GDDLPackLevelBrowser::updatePackUI() {
    log::info("[({})GDDLPackLevelBrowser::updatePackUI] Called", fmt::ptr(this));
    const std::shared_ptr<PackInfo> packInfo = PacksManager::getOrRequestPackInfo(m_fields->packID, true).unwrap(); // should always be valid
    // progressbar
    if (m_fields->progressBar != nullptr) {
        log::info("[({})GDDLPackLevelBrowser::updatePackUI] Updating progress bar", fmt::ptr(this));
        const auto [completedFraction, baseCompleted] = packInfo->getCompletedFraction();
        const float progress = 100.0f * static_cast<float>(completedFraction.first) / static_cast<float>(completedFraction.second);
        m_fields->progressBar->updateProgress(progress);
        m_fields->progressBar->setFillColor(baseCompleted ? ccc3(0, 255, 255) : ccc3(0, 255, 0));
        const std::string progressString = std::to_string(completedFraction.first) + "/" + std::to_string(completedFraction.second);
        m_fields->progressBar->getProgressLabel()->setString(progressString.c_str());
    }
    // mark levels as extra, not crashing here would be nice
    log::info("[({})GDDLPackLevelBrowser::updatePackUI] Marking levels as extra", fmt::ptr(this));
    const auto boomListView = typeinfo_cast<BoomListView*>(m_list->m_listView);
    if (boomListView) {
        const auto tableView = typeinfo_cast<TableView*>(boomListView->m_tableView);
        if (tableView) {
            for (const auto levelCell : CCArrayExt<LevelCell*>(m_list->m_listView->m_tableView->m_cellArray)) {
                if (packInfo->isExtra(levelCell->m_level->m_levelID)) {
                    // mark as extra by setting the name to red I guess? idk
                    const auto maybeLevelNameLabel = levelCell->getChildByIDRecursive("level-name");
                    const auto levelNameLabel = typeinfo_cast<CCLabelBMFont*>(maybeLevelNameLabel);
                    if (levelNameLabel) {
                        levelNameLabel->setColor(ccc3(255, 100, 100));
                    }
                }
            }
        }
    }
    // hide the original frame because it keeps reappearing
    log::info("[({})GDDLPackLevelBrowser::updatePackUI] Hiding original textures", fmt::ptr(this));
    hideOriginalTextures();
}

void GDDLPackLevelBrowser::hideOriginalTextures() {
    log::info("[({})GDDLPackLevelBrowser::hideOriginalTextures] Called", fmt::ptr(this));
    const std::vector<std::string> borderIDs = {"left-border", "right-border", "bottom-border", "top-border"};
    for (const auto id : borderIDs) {
        const auto node = m_list->getChildByIDRecursive(id);
        if (node != nullptr) {
            node->setVisible(false);
        }
    }
    const std::vector<std::string> bgIDs = {"background", "left-corner", "right-corner"};
    for (const auto id : bgIDs) {
        const auto node = this->getChildByIDRecursive(id);
        if (node != nullptr) {
            node->setVisible(false);
        }
    }
    const std::vector<std::string> otherModIDs = {"cvolton.betterinfo/last-button", "cvolton.betterinfo/star-button", "cvolton.betterinfo/filter-button"};
    for (const auto id : otherModIDs) {
        const auto node = this->getChildByIDRecursive(id);
        if (node != nullptr) {
            node->setVisible(false);
        }
    }
}

void GDDLPackLevelBrowser::updateAfterLoadLevelsFinished() {
    log::info("[({})GDDLPackLevelBrowser::updateAfterLoadLevelsFinished] Called, requesting PackInfo", fmt::ptr(this));
    const std::shared_ptr<PackInfo> packInfo = PacksManager::getOrRequestPackInfo(m_fields->packID, true).unwrap(); // should always be valid
    log::info("[({})GDDLPackLevelBrowser::updateAfterLoadLevelsFinished] Updating UI", fmt::ptr(this));
    m_leftArrow->setVisible(m_fields->currentPage > 0);
    m_rightArrow->setVisible(packInfo->shouldShowRightArrow(m_fields->currentPage));
    setCorrectLabelsText();
    updatePackUI();
}

void GDDLPackLevelBrowser::updateAfterRefresh() {
    log::info("[({})GDDLPackLevelBrowser::updateAfterRefresh] Called, requesting PackInfo", fmt::ptr(this));
    const std::shared_ptr<PackInfo> packInfo = PacksManager::getOrRequestPackInfo(m_fields->packID, true).unwrap(); // should always be valid
    log::info("[({})GDDLPackLevelBrowser::updateAfterRefresh] Requesting page 0", fmt::ptr(this));
    packInfo->requestPage(0, this);
    hideOriginalTextures();
}

void GDDLPackLevelBrowser::setCorrectLabelsText() {
    log::info("[({})GDDLPackLevelBrowser::setCorrectLabelsText] Called", fmt::ptr(this));
    const std::shared_ptr<PackInfo> packInfo = PacksManager::getOrRequestPackInfo(m_fields->packID, true).unwrap(); // should always be valid
    // # of results text
    m_countText->setString(packInfo->getPageCountText(m_fields->currentPage).c_str());
    // page button
    m_pageBtn->setVisible(true);
    m_pageText->setString(std::to_string(m_fields->currentPage + 1).c_str());
    // title text handled by getSearchTitle()
}
