#include "GDDLPackLevelBrowser.h"

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
            if (const auto maybeLabel = typeinfo_cast<CCLabelBMFont*>(child)) {
                maybeLabel->setString(m_fields->packInfo->getName().c_str());
            }
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
