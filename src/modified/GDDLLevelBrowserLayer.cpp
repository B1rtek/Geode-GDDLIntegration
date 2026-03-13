// ReSharper disable CppHidingFunction
// ReSharper disable CppParameterMayBeConst
#include "GDDLLevelBrowserLayer.h"

#include <Geode/Bindings.hpp>
#include "Geode/modify/LevelBrowserLayer.hpp"


void GDDLLevelBrowserLayer::loadLevelsFinished(cocos2d::CCArray * p0, char const *p1, int p2) {
    LevelBrowserLayer::loadLevelsFinished(p0, p1, p2);
    if (m_fields->searchObject != nullptr) {
        updateAfterInitialLoad();
    }
}

void GDDLLevelBrowserLayer::onNextPage(CCObject * sender) {
    if (m_fields->searchObject != nullptr) {
        m_fields->currentPage = std::min(m_fields->currentPage + 1, m_fields->searchObject->getTotalApiResultsPageCount());
        m_fields->searchObject->requestSearchPage(m_fields->currentPage, this);
    } else {
        LevelBrowserLayer::onNextPage(sender);
    }
}

void GDDLLevelBrowserLayer::onPrevPage(CCObject * sender) {
    if (m_fields->searchObject != nullptr) {
        m_fields->currentPage = std::max(0, m_fields->currentPage - 1);
        m_fields->searchObject->requestSearchPage(m_fields->currentPage, this);
    } else {
        LevelBrowserLayer::onPrevPage(sender);
    }
}

void GDDLLevelBrowserLayer::setIDPopupClosed(SetIDPopup* popup, int value) {
    if (m_fields->searchObject != nullptr) {
        m_fields->searchObject->requestSearchPage(value - 1, this);
    } else {
        LevelBrowserLayer::setIDPopupClosed(popup, value);
    }
}

// crash workaround, code moved to GDDLSearchLayerV2::onEnter()
// void GDDLLevelBrowserLayer::keyBackClicked() {
//     if (m_fields->searchObject != nullptr) {
//         backActions();
//     }
//     LevelBrowserLayer::keyBackClicked();
// }
//
// void GDDLLevelBrowserLayer::onBack(cocos2d::CCObject* sender) {
//     if (m_fields->searchObject != nullptr) {
//         backActions();
//     }
//     LevelBrowserLayer::onBack(sender);
// }
//
// void GDDLLevelBrowserLayer::backActions() {
//     m_fields->searchObject->cancelSearch();
// }

void GDDLLevelBrowserLayer::setCorrectLabelsText() {
    // # of results text
    m_countText->setString(m_fields->searchObject->getPageCountText(m_fields->currentPage).c_str());
    // page button
    m_pageBtn->setVisible(true);
    m_pageText->setString(std::to_string(m_fields->currentPage + 1).c_str());
}

void GDDLLevelBrowserLayer::handleSearchObject(GJSearchObject * searchObject, int pageToLoad) {
    m_fields->currentPage = pageToLoad;
    loadPage(searchObject);
    setCorrectLabelsText();
}

void GDDLLevelBrowserLayer::assignSearchObject(SearchObject* searchObject) {
    m_fields->searchObject = searchObject;
    updateAfterInitialLoad();
}

void GDDLLevelBrowserLayer::updateAfterInitialLoad() {
    m_leftArrow->setVisible(m_fields->currentPage > 0);
    m_rightArrow->setVisible(m_fields->searchObject->shouldShowRightArrow(m_fields->currentPage));
    setCorrectLabelsText();
}
