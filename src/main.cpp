/**
 * Include the Geode headers.
 */
// ReSharper disable once CppUnusedIncludeDirective
#include <Geode/Geode.hpp>
/**
 * Required to modify the MenuLayer class
 */
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/utils/web.hpp>

#include "layers/GDDLSearchLayer.h"
#include "RatingsManager.h"
#include "Utils.h"
#include "modified/GDDLRobtopLevelsLayer.h"

/**
 * Brings cocos2d and all Geode namespaces
 * to the current scope.
 */
using namespace geode::prelude;

/**
 * `$modify` lets you extend and modify GD's
 * classes; to hook a function in Geode,
 * simply $modify the class and write a new
 * function definition with the signature of
 * the one you want to hook.
 */
class $modify(MenuLayer) {
    /**
     * MenuLayer::onMoreGames is a GD function,
     * so we hook it by simply writing its name
     * inside a $modified MenuLayer class.
     *
     * Note that for all hooks, your signature
     * has to match exactly - `bool onMoreGames`
     * would not place a hook!
     */

    struct Fields {
        async::TaskHolder<web::WebResponse> cacheEventListener;
    };


    bool init() override {
        if (!MenuLayer::init()) return false;

        GDDLSearchLayer::loadSettings(); // will only work if this is the first time the MenuLayer was loaded
        GDDLSearchLayer::stopSearch();
        GDDLSearchLayer::restoreValuesAfterSplit();
        GDDLSearchLayer::saveSettings();
        if (!RatingsManager::readCache) {
            // populate from save
            RatingsManager::readCache = true;
            RatingsManager::populateFromSave();
        }
        if (RatingsManager::readCache && RatingsManager::cacheEmpty() && !RatingsManager::triedToDownloadCache) {
            RatingsManager::triedToDownloadCache = true;
            auto req = web::WebRequest();
            req.header("User-Agent", Utils::getUserAgent());
            // if you're reading this because you treat this as an example of how to use the gddl api
            // cache
            // for the love of god
            // please
            m_fields->cacheEventListener.spawn(req.get(RatingsManager::gddlSheetUrl), Utils::getCacheDownloadLambda());
        }
        return true;
    }

    void onQuit(cocos2d::CCObject* sender) {
        MenuLayer::onQuit(sender);
        RatingsManager::cacheList(true); // cache modified list on every exit
    }
};
