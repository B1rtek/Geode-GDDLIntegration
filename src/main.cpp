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
#include <settings/ButtonPositionSetting.h>
#include <settings/ExcludeRangeSetting.h>
#include <settings/UseOldTierLabelSetting.h>


#include "GDDLSearchLayer.h"
#include "RatingsManager.h"
#include "Utils.h"

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
        EventListener<web::WebTask> cacheEventListener;
    };


    bool init() override {
        if (!MenuLayer::init()) return false;

        GDDLSearchLayer::loadSettings(); // will only work if this is the first time the MenuLayer was loaded
        GDDLSearchLayer::stopSearch();
        GDDLSearchLayer::restoreValuesAfterSplit();
        GDDLSearchLayer::saveSettings();
        if (!RatingsManager::alreadyCached() && !RatingsManager::triedToCache) { // TODO triedToCache is never written to
            Utils::bindCacheDownloadCallback(m_fields->cacheEventListener);
            auto req = web::WebRequest();
            // if you're reading this because you treat this as an example of how to use the gddl api
            // cache
            // for the love of god
            // please
            m_fields->cacheEventListener.setFilter(req.get("https://docs.google.com/spreadsheets/d/1qKlWKpDkOpU1ZF6V6xGfutDY2NvcA8MNPnsv6GBkKPQ/gviz/tq?tqx=out:csv&sheet=GDDL"));
        }
        return true;
    }

    void onQuit(cocos2d::CCObject* sender) {
        MenuLayer::onQuit(sender);
        RatingsManager::cacheList(true); // cache modified list on every exit
    }
};

$on_mod(Loaded) {
    Mod::get()->registerCustomSetting(
        "button-position",
        std::make_unique<ButtonPositionSetting>("button-position", Mod::get()->getID(), DEFAULT)
    );
    Mod::get()->registerCustomSetting(
        "use-old-tier-label",
        std::make_unique<UseOldTierLabelSetting>("use-old-tier-label", Mod::get()->getID(), false, 0)
    );
    Mod::get()->registerCustomSetting(
        "exclude-range",
        std::make_unique<ExcludeRangeSetting>("exclude-range", Mod::get()->getID(), 0, 0, false)
    );
}
