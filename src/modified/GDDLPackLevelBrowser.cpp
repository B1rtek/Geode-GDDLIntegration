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
    Modify<GDDLPackLevelBrowser, LevelBrowserLayer>::loadLevelsFinished(p0, p1, p2);
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
        Modify<GDDLPackLevelBrowser, LevelBrowserLayer>::setIDPopupClosed(popup, value);
    }
}

void GDDLPackLevelBrowser::keyBackClicked() {
    if (m_fields->packInfo != nullptr) {
        backActions();
    }
    Modify<GDDLPackLevelBrowser, LevelBrowserLayer>::keyBackClicked();
}

void GDDLPackLevelBrowser::onEnterTransitionDidFinish() {
    Modify<GDDLPackLevelBrowser, LevelBrowserLayer>::onEnterTransitionDidFinish();
    if (m_fields->packInfo != nullptr) {
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
            packIcon->setZOrder(3);
            this->addChild(packIcon);
        }
        // ok things
        updateAfterLoadLevelsFinished();
    }

}

void GDDLPackLevelBrowser::onBack(cocos2d::CCObject* sender) {
    if (m_fields->packInfo != nullptr) {
        backActions();
    }
    Modify<GDDLPackLevelBrowser, LevelBrowserLayer>::onBack(sender);
}

void GDDLPackLevelBrowser::backActions() {
    // empty for now
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

void GDDLPackLevelBrowser::updateAfterLoadLevelsFinished() {
    m_leftArrow->setVisible(m_fields->currentPage > 0);
    m_rightArrow->setVisible(m_fields->packInfo->shouldShowRightArrow(m_fields->currentPage));
    setCorrectLabelsText();
}

void GDDLPackLevelBrowser::setCorrectLabelsText() {
    // # of results text
    m_countText->setString(m_fields->packInfo->getPageCountText(m_fields->currentPage).c_str());
    // page button
    m_pageBtn->setVisible(true);
    m_pageText->setString(std::to_string(m_fields->currentPage + 1).c_str());
    // title text handled by getSearchTitle()
}
