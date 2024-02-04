#include <Geode/Bindings.hpp>
#include <Geode/modify/LevelBrowserLayer.hpp>


#include "RatingsManager.h"

class $modify(GDDLBrowserLayer, LevelBrowserLayer) {
    int currentPage = 1;

    bool init(GJSearchObject* p0) {
        if(!RatingsManager::isSearchingForTier() || p0->m_searchType != SearchType::Type19) {
            return LevelBrowserLayer::init(p0);
        }
        return LevelBrowserLayer::init(RatingsManager::getSearchPage(m_fields->currentPage));
    }

    void loadLevelsFinished(cocos2d::CCArray* p0, char const* p1, int p2) {
        LevelBrowserLayer::loadLevelsFinished(p0, p1, p2);
        if(!RatingsManager::isSearchingForTier() || m_searchObject->m_searchType != SearchType::Type19) return;
        m_leftArrow->setVisible(m_fields->currentPage > 0);
        m_rightArrow->setVisible(true);
    }

    void onNextPage(CCObject* sender) {
        LevelBrowserLayer::onNextPage(sender);
        if(!RatingsManager::isSearchingForTier() || m_searchObject->m_searchType != SearchType::Type19) return;
        m_fields->currentPage++;
        loadPage(RatingsManager::getSearchPage(m_fields->currentPage));
    }

    void onPrevPage(CCObject* sender) {
        LevelBrowserLayer::onPrevPage(sender);
        if(!RatingsManager::isSearchingForTier() || m_searchObject->m_searchType != SearchType::Type19) return;
        m_fields->currentPage = std::max(1, m_fields->currentPage-1);
        loadPage(RatingsManager::getSearchPage(m_fields->currentPage));
    }
};