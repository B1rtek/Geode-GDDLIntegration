// ReSharper disable CppHidingFunction
// ReSharper disable CppParameterMayBeConst
#include "GDDLLevelBrowserLayer.h"

#include <Geode/Bindings.hpp>
#include "Geode/modify/LevelBrowserLayer.hpp"

#include "layers/GDDLSearchLayer.h"

void GDDLLevelBrowserLayer::loadLevelsFinished(cocos2d::CCArray * p0, char const *p1, int p2) {
    LevelBrowserLayer::loadLevelsFinished(p0, p1, p2);
    if (m_fields->searchObject != nullptr) {
        updateAfterInitialLoad();
    }
}

void GDDLLevelBrowserLayer::onNextPage(CCObject * sender) {
    LevelBrowserLayer::onNextPage(sender);
    if (m_fields->searchObject != nullptr) {
        m_fields->currentPage = std::min(m_fields->currentPage + 1, m_fields->searchObject->getTotalApiResultsPageCount() - 1);
        m_fields->searchObject->requestSearchPage(m_fields->currentPage, this);
    }
}

void GDDLLevelBrowserLayer::onPrevPage(CCObject * sender) {
    LevelBrowserLayer::onPrevPage(sender);
    if (m_fields->searchObject != nullptr) {
        m_fields->currentPage = std::max(0, m_fields->currentPage - 1);
        m_fields->searchObject->requestSearchPage(m_fields->currentPage, this);
    }
}

void GDDLLevelBrowserLayer::onGoToPage(CCObject* sender) {// left here in case this is doable in the future
    LevelBrowserLayer::onGoToPage(sender);
    if (m_fields->searchObject != nullptr) {
        const int pageTarget = std::stoi(m_pageText->getString());
        m_fields->searchObject->requestSearchPage(pageTarget, this);
    }
}

void GDDLLevelBrowserLayer::setCorrectLabelsText() {
    const int firstLevel = m_fields->currentPage * 10 + 1;
    const int lastLevel = std::min(firstLevel + 9, m_fields->searchObject->getTotalApiResultsPageCount());
    m_countText->setString(
            fmt::format("{} to {} of max. {}", firstLevel, lastLevel, m_fields->searchObject->getTotalApiResultsPageCount())
                    .c_str());
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
    m_rightArrow->setVisible(m_fields->currentPage < m_fields->searchObject->getTotalApiResultsPageCount() - 1);
    setCorrectLabelsText();
}
