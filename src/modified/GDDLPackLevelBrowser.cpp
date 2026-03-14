#include "GDDLPackLevelBrowser.h"

#include <Utils.h>
#include <Geode/binding/GJListLayer.hpp>

gd::string GDDLPackLevelBrowser::getSearchTitle() {
    if (m_fields->packInfo != nullptr) {
        return this->m_fields->packInfo->getName();
    }
    return LevelBrowserLayer::getSearchTitle();
}

void GDDLPackLevelBrowser::loadLevelsFinished(cocos2d::CCArray* p0, char const* p1, int p2) {
    LevelBrowserLayer::loadLevelsFinished(p0, p1, p2);
    if (m_fields->packInfo != nullptr) {
        updateAfterLoadLevelsFinished();
    }
}

void GDDLPackLevelBrowser::onNextPage(CCObject* sender) {
    if (m_fields->packInfo != nullptr) {
        m_fields->packInfo->requestPage(m_fields->currentPage + 1, this);
    } else {
        LevelBrowserLayer::onNextPage(sender);
    }
}

void GDDLPackLevelBrowser::onPrevPage(CCObject* sender) {
    if (m_fields->packInfo != nullptr) {
        m_fields->packInfo->requestPage(m_fields->currentPage - 1, this);
    } else {
        LevelBrowserLayer::onPrevPage(sender);
    }
}

void GDDLPackLevelBrowser::setIDPopupClosed(SetIDPopup* popup, int value) {
    if (m_fields->packInfo != nullptr) {
        m_fields->packInfo->requestPage(value - 1, this);
    } else {
        LevelBrowserLayer::setIDPopupClosed(popup, value);
    }
}

void GDDLPackLevelBrowser::onEnterTransitionDidFinish() {
    LevelBrowserLayer::onEnterTransitionDidFinish();
    if (m_fields->packInfo != nullptr) {
        createPackUI();
        updateAfterLoadLevelsFinished();
    }
}

void GDDLPackLevelBrowser::handleSearchObject(GJSearchObject* gjSearchObject, const int actualPageNumber) {
    m_fields->currentPage = actualPageNumber;
    loadPage(gjSearchObject);
    setCorrectLabelsText();
}

void GDDLPackLevelBrowser::assignPackInfo(PackInfo* packInfo) {
    this->m_fields->packInfo = packInfo;
    updateAfterLoadLevelsFinished();
}

void GDDLPackLevelBrowser::createPackUI() {
    if (m_fields->firstOpen) {
        m_fields->firstOpen = false;
        // cursed things (fortunately they only happen once)
        const auto listChildren = m_list->getChildren();
        for (const auto child : CCArrayExt<CCNode*>(listChildren)) {
            if (auto maybeLabel = typeinfo_cast<CCLabelBMFont*>(child)) {
                maybeLabel->setString(m_fields->packInfo->getName().c_str());
                // scale down so it doesn't take up the whole screen because it only scales after the whole thing loads for some reason
                maybeLabel->limitLabelWidth(280.0f, 0.8f, 0.2f);
            }
        }
        // sprite adding things
        for (int i = 0; i < 2; i++) {
            const auto packIcon = LazySprite::create({20.0f, 20.0f});
            packIcon->setLoadCallback([packIcon](Result<> res) {
                if (!res) {
                    packIcon->initWithFile(Mod::get()->expandSpriteName("tier_unrated.png").data());
                    packIcon->setScale(0.275f);
                }
            });
            packIcon->loadFromUrl(Values::packIconsBaseUrl + m_fields->packInfo->getIconPath());
            packIcon->setPosition({m_list->getPositionX() - 4.0f + i * (m_list->getContentWidth() + 8.0f), m_list->getPositionY() + m_list->getContentHeight() + 17.0f});
            packIcon->setScale(1.25f);
            packIcon->setZOrder(11);
            this->addChild(packIcon);
        }
        // progress bar
        m_fields->progressBar = ProgressBar::create(ProgressBarStyle::Solid);
        m_fields->progressBar->showProgressLabel(true);
        m_fields->progressBar->setScale(0.9f);
        m_fields->progressBar->setPrecision(2);
        m_fields->progressBar->setPosition({m_list->getPositionX() + m_list->getContentWidth() / 2 - m_fields->progressBar->getScaledContentWidth() / 2, m_list->getPositionY() - 15.0f});
        m_fields->progressBar->setZOrder(11);
        this->addChild(m_fields->progressBar);
    }
}

void GDDLPackLevelBrowser::updatePackUI() {
    // progressbar
    if (m_fields->progressBar != nullptr) {
        const auto [progress, baseCompleted] = m_fields->packInfo->getCompletionStatus();
        m_fields->progressBar->updateProgress(progress);
        m_fields->progressBar->setFillColor(baseCompleted ? ccc3(0, 255, 255) : ccc3(0, 255, 0));
    }
    // mark levels as extra, not crashing here would be nice
    const auto boomListView = typeinfo_cast<BoomListView*>(m_list->m_listView);
    if (boomListView) {
        const auto tableView = typeinfo_cast<TableView*>(boomListView->m_tableView);
        if (tableView) {
            for (const auto levelCell : CCArrayExt<LevelCell*>(m_list->m_listView->m_tableView->m_cellArray)) {
                if (m_fields->packInfo->isExtra(levelCell->m_level->m_levelID)) {
                    // mark as extra by setting the name to red I guess? idk
                    const auto maybeLevelNameLabel = levelCell->getChildByIDRecursive("level-name");
                    const auto levelNameLabel = typeinfo_cast<CCLabelBMFont*>(maybeLevelNameLabel);
                    if (levelNameLabel) {
                        levelNameLabel->setColor(ccc3(255, 0, 0));
                    }
                }
            }
        }
    }
}

void GDDLPackLevelBrowser::updateAfterLoadLevelsFinished() {
    m_leftArrow->setVisible(m_fields->currentPage > 0);
    m_rightArrow->setVisible(m_fields->packInfo->shouldShowRightArrow(m_fields->currentPage));
    setCorrectLabelsText();
    updatePackUI();
}

void GDDLPackLevelBrowser::setCorrectLabelsText() {
    // # of results text
    m_countText->setString(m_fields->packInfo->getPageCountText(m_fields->currentPage).c_str());
    // page button
    m_pageBtn->setVisible(true);
    m_pageText->setString(std::to_string(m_fields->currentPage + 1).c_str());
    // title text handled by getSearchTitle()
}
