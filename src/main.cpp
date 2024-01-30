/**
 * Include the Geode headers.
 */
#include <Geode/Geode.hpp>
/**
 * Required to modify the MenuLayer class
 */
#include <Geode/modify/MenuLayer.hpp>
#include <Geode/utils/web.hpp>


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

    bool init() {
        if (!MenuLayer::init()) return false;

        if (!RatingsManager::alreadyCached()) {
            web::AsyncWebRequest()
            .fetch("https://gdladder.com/api/theList")
            .text()
            .then([](std::string const& response) {
                RatingsManager::cacheRatings(response);
            })
            .expect([](std::string const& error) {
                FLAlertLayer::create("GDDL Integration", "Could not cache ratings from gdladder.com! Check your internet connection and restart the game.", "OK")->show();
            });
        }
        return true;
    }
};
