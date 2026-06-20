#include "PackListItem.h"

#include <Utils.h>
#include <Values.h>
#include <Geode/loader/Mod.hpp>
#include <Geode/ui/LazySprite.hpp>
#include <managers/PacksManager.h>
#include <modified/GDDLPackLevelBrowser.h>

bool PackListItem::init(const float width, const int packID) {
    if (!CCNode::init()) return false;
    log::info("[({})PackListItem::init] Creating PackListItem for pack {}", fmt::ptr(this), packID);

    this->setContentSize({width, itemHeight});
    this->packID = packID;
    const std::shared_ptr<PackInfo> packInfo = PacksManager::getOrRequestPackInfo(packID, false).unwrap(); // should never be Err

    // icon
    const auto icon = LazySprite::create({20.0f, 20.0f});
    icon->setLoadCallback([icon](Result<> res) {
        if (!res) {
            icon->initWithFile(Mod::get()->expandSpriteName("tier_unrated.png").data());
            icon->setScale(0.275f);
        }
    });
    icon->loadFromUrl(Values::packIconsBaseUrl + packInfo->getIconPath());
    icon->setPosition({20.0f, itemHeight / 2});
    this->addChild(icon);

    // title
    const auto title = CCLabelBMFont::create(packInfo->getName().c_str(), "bigFont.fnt");
    title->setAnchorPoint({0.0f, 0.5f});
    title->setScale(0.6f);
    title->setPosition({50.0f, itemHeight / 2});
    this->addChild(title);

    // TODO add completion status

    // view button
    const auto buttonMenu = CCMenu::create();
    buttonMenu->setContentSize({itemHeight * 2, itemHeight});
    buttonMenu->setPosition({width - itemHeight * 2, 0});
    this->addChild(buttonMenu);
    const auto viewButtonSprite = ButtonSprite::create("View", "bigFont.fnt", "GJ_button_06.png");
    viewButtonSprite->setScale(0.6f);
    const auto viewButton = CCMenuItemSpriteExtra::create(viewButtonSprite, this, menu_selector(PackListItem::onView));
    viewButton->setPosition({itemHeight, itemHeight / 2});
    buttonMenu->addChild(viewButton);

    log::info("[({})PackListItem::init] Created PackListItem", fmt::ptr(this), packID);
    return true;
}

void PackListItem::onView(CCObject* sender) {
    log::info("[({})PackListItem::onView] Called", fmt::ptr(this));
    PacksManager::subscribeToObservers(this);
    log::info("[({})PackListItem::onView] Subscribed to PacksManager", fmt::ptr(this));
    const auto maybePackInfo = PacksManager::getOrRequestPackInfo(packID, true);
    if (maybePackInfo.isOk()) {
        log::info("[({})PackListItem::onView] Received pack {} from cache", fmt::ptr(this), packID);
        PacksManager::unsubscribeFromObservers(this);
        log::info("[({})PackListItem::onView] Unsubscribed from PackManager", fmt::ptr(this));
        GJSearchObject* gjSearchObject = Utils::createGJSearchObjectFromIndex(0, maybePackInfo.unwrap()->getLevels());
        forwardToLevelBrowser(gjSearchObject);
    }
    // well now we wait I guess
    // TODO some kind of a loading circle I guess
}

void PackListItem::forwardToLevelBrowser(GJSearchObject* gjSearchObject) {
    log::info("[({})PackListItem::forwardToLevelBrowser] Forwarding", fmt::ptr(this));
    const auto levelBrowserLayer = static_cast<GDDLPackLevelBrowser*>(GDDLPackLevelBrowser::create(gjSearchObject));
    levelBrowserLayer->assignPackID(packID);
    const auto listLayerScene = CCScene::create();
    listLayerScene->addChild(levelBrowserLayer);
    const auto transition = CCTransitionFade::create(0.5, listLayerScene);
    CCDirector::sharedDirector()->pushScene(transition);
    log::info("[({})PackListItem::forwardToLevelBrowser] Pushed scene", fmt::ptr(this));
}

PackListItem* PackListItem::create(const float width, const int packID) {
    const auto ret = new PackListItem();
    if (ret->init(width, packID)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void PackListItem::updateData() {
    log::info("[({})PackListItem::updateData] Called", fmt::ptr(this));
    PacksManager::unsubscribeFromObservers(this);
    log::info("[({})PackListItem::updateData] Unsubscribed from PacksManager", fmt::ptr(this));
    GJSearchObject* gjSearchObject = Utils::createGJSearchObjectFromIndex(0, PacksManager::getOrRequestPackInfo(packID, true).unwrap()->getLevels());
    forwardToLevelBrowser(gjSearchObject);
}
