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
#include <settings/ExcludeRangeSetting.h>


#include "GDDLSearchLayer.h"
#include "RatingsManager.h"

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

    bool init() override {
        if (!MenuLayer::init()) return false;

        GDDLSearchLayer::loadSettings(); // will only work if this is the first time the MenuLayer was loaded
        GDDLSearchLayer::stopSearch();
        GDDLSearchLayer::restoreValuesAfterSplit();
        GDDLSearchLayer::saveSettings();
        if (!RatingsManager::alreadyCached() && !RatingsManager::triedToCache) {
            RatingsManager::triedToCache = true;
            web::AsyncWebRequest()
            .fetch("https://gdladder.com/api/theList")
            .text()
            .then([](std::string const& response) {
                RatingsManager::cacheRatings(response);
                if(!RatingsManager::alreadyCached()) {
                    somethingWentWrong();
                }
            })
            .expect([](std::string const& error) {
                somethingWentWrong();
            });
        }
        return true;
    }

    static void somethingWentWrong() {
        FLAlertLayer::create("GDDL Integration", "Could not cache ratings from gdladder.com! Check your internet connection and restart the game.", "OK")->show();
    }

    void onQuit(cocos2d::CCObject* sender) {
        MenuLayer::onQuit(sender);
        RatingsManager::cacheList(true); // cache modified list on every exit
    }
};

$on_mod(Loaded) {
    Mod::get()->registerCustomSetting(
        "exclude-range",
        std::make_unique<ExcludeRangeSetting>("exclude-range", Mod::get()->getID(), 0, 0, false)
    );
}
