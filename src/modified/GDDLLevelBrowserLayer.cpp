// ReSharper disable CppHidingFunction
// ReSharper disable CppParameterMayBeConst
#include "GDDLLevelBrowserLayer.h"

#include <Geode/Bindings.hpp>
#include "Geode/modify/LevelBrowserLayer.hpp"

#include "layers/GDDLSearchLayer.h"

bool GDDLLevelBrowserLayer::init(GJSearchObject * p0) {
    if (!GDDLSearchLayer::isSearching() || p0->m_searchType != SearchType::Type19) {
        return LevelBrowserLayer::init(p0);
    }
    return LevelBrowserLayer::init(p0);
}

void GDDLLevelBrowserLayer::loadLevelsFinished(cocos2d::CCArray * p0, char const *p1, int p2) {
    LevelBrowserLayer::loadLevelsFinished(p0, p1, p2);
    if (!GDDLSearchLayer::isSearching() || m_searchObject->m_searchType != SearchType::Type19)
        return;
    m_leftArrow->setVisible(m_fields->currentPage > 0);
    m_rightArrow->setVisible(m_fields->currentPage < GDDLSearchLayer::getSearchResultsPageCount());
    setCorrectLabelsText();
}

void GDDLLevelBrowserLayer::onNextPage(CCObject * sender) {
    LevelBrowserLayer::onNextPage(sender);
    if (!GDDLSearchLayer::isSearching() || m_searchObject->m_searchType != SearchType::Type19)
        return;
    m_fields->currentPage = std::min(m_fields->currentPage + 1, GDDLSearchLayer::getSearchResultsPageCount());
    GDDLSearchLayer::requestSearchPage(m_fields->currentPage, this);
}

void GDDLLevelBrowserLayer::onPrevPage(CCObject * sender) {
    LevelBrowserLayer::onPrevPage(sender);
    if (!GDDLSearchLayer::isSearching() || m_searchObject->m_searchType != SearchType::Type19)
        return;
    m_fields->currentPage = std::max(0, m_fields->currentPage - 1);
    GDDLSearchLayer::requestSearchPage(m_fields->currentPage, this);
}

// void GDDLLevelBrowserLayer::onGoToPage(CCObject* sender) {// left here in case this is doable in the future
//     LevelBrowserLayer::onGoToPage(sender);
//     if(!RatingsManager::isSearchingForTier() || m_searchObject->m_searchType != SearchType::Type19) return;
//     const int pageTarget = std::stoi(m_pageText->getString());
//     m_fields->currentPage = std::min(std::max(1, pageTarget), RatingsManager::getSearchResultsPageCount());
//     loadPage(RatingsManager::getSearchPage(m_fields->currentPage));
//     setCorrectLabelsText();
// }

void GDDLLevelBrowserLayer::setCorrectLabelsText() {
    const int firstLevel = m_fields->currentPage * 10 + 1;
    const int lastLevel = std::min(firstLevel + 9, GDDLSearchLayer::getSearchResultsCount());
    m_countText->setString(
            fmt::format("{} to {} of max. {}", firstLevel, lastLevel, GDDLSearchLayer::getSearchResultsCount())
                    .c_str());
    m_pageBtn->setVisible(false);
}

void GDDLLevelBrowserLayer::handleSearchObject(GJSearchObject * searchObject, int resultsCount) {
    // check doesn't apply to the first page for REASONS
    // DON'T GO TOO FAR OR ELSE
    if (resultsCount <= 0 && m_fields->currentPage != 1) {
        delete searchObject;
        searchObject = GDDLSearchLayer::getSearchObjectForPage(--m_fields->currentPage);
    }
    // THIS IS NOT A JOKE
    loadPage(searchObject);
    setCorrectLabelsText();
}

void GDDLLevelBrowserLayer::assignSearchObject(SearchObject* searchObject) {
    m_fields->searchObject = searchObject;
}
