// ReSharper disable CppHidingFunction
// ReSharper disable CppParameterMayBeConst
#include <Geode/Bindings.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>

#include "GDDLSearchLayer.h"

class $modify(GDDLBrowserLayer, LevelBrowserLayer) {
    int currentPage = 1;

    bool init(GJSearchObject * p0) {
        if (!GDDLSearchLayer::isSearching() || p0->m_searchType != SearchType::Type19) {
            return LevelBrowserLayer::init(p0);
        }
        return LevelBrowserLayer::init(p0);
    }

    void loadLevelsFinished(cocos2d::CCArray * p0, char const *p1, int p2) override {
        LevelBrowserLayer::loadLevelsFinished(p0, p1, p2);
        if (!GDDLSearchLayer::isSearching() || m_searchObject->m_searchType != SearchType::Type19)
            return;
        m_leftArrow->setVisible(currentPage > 1);
        m_rightArrow->setVisible(currentPage < GDDLSearchLayer::getSearchResultsPageCount());
        setCorrectLabelsText();
    }

    void onNextPage(CCObject * sender) {
        LevelBrowserLayer::onNextPage(sender);
        if (!GDDLSearchLayer::isSearching() || m_searchObject->m_searchType != SearchType::Type19)
            return;
        currentPage = std::min(currentPage + 1, GDDLSearchLayer::getSearchResultsPageCount());
        GDDLSearchLayer::requestSearchPage(currentPage, this);
    }

    void onPrevPage(CCObject * sender) {
        LevelBrowserLayer::onPrevPage(sender);
        if (!GDDLSearchLayer::isSearching() || m_searchObject->m_searchType != SearchType::Type19)
            return;
        currentPage = std::max(1, currentPage - 1);
        GDDLSearchLayer::requestSearchPage(currentPage, this);
    }

    // void onGoToPage(CCObject* sender) {// left here in case this is doable in the future
    //     LevelBrowserLayer::onGoToPage(sender);
    //     if(!RatingsManager::isSearchingForTier() || m_searchObject->m_searchType != SearchType::Type19) return;
    //     const int pageTarget = std::stoi(m_pageText->getString());
    //     currentPage = std::min(std::max(1, pageTarget), RatingsManager::getSearchResultsPageCount());
    //     loadPage(RatingsManager::getSearchPage(currentPage));
    //     setCorrectLabelsText();
    // }

    void setCorrectLabelsText() {
        const int firstLevel = (currentPage - 1) * 10 + 1;
        const int lastLevel = std::min(firstLevel + 9, GDDLSearchLayer::getSearchResultsCount());
        m_countText->setString(
                fmt::format("{} to {} of max. {}", firstLevel, lastLevel, GDDLSearchLayer::getSearchResultsCount())
                        .c_str());
        m_pageBtn->setVisible(false);
    }

    void handleSearchObject(GJSearchObject * searchObject, int resultsCount) {
        // check doesn't apply to the first page for REASONS
        // DON'T GO TOO FAR OR ELSE
        if (resultsCount <= 0 && currentPage != 1) {
            delete searchObject;
            searchObject = GDDLSearchLayer::getSearchObjectForPage(--currentPage);
        }
        // THIS IS NOT A JOKE
        loadPage(searchObject);
        setCorrectLabelsText();
    }
};
