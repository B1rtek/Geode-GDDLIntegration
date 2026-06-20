#include "PackListItem.h"

#include <Utils.h>
#include <Values.h>
#include <Geode/loader/Mod.hpp>
#include <Geode/ui/LazySprite.hpp>
#include <managers/PacksManager.h>
#include <modified/GDDLPackLevelBrowser.h>

bool PackListItem::init(const float width, const int packID) {
    if (!CCNode::init()) return false;

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

    return true;
}

void PackListItem::onView(CCObject* sender) {
    PacksManager::subscribeToObservers(this);
    const auto maybePackInfo = PacksManager::getOrRequestPackInfo(packID, true);
    if (maybePackInfo.isOk()) {
        PacksManager::unsubscribeFromObservers(this);
        GJSearchObject* gjSearchObject = Utils::createGJSearchObjectFromIndex(0, maybePackInfo.unwrap()->getLevels());
        forwardToLevelBrowser(gjSearchObject, nullptr, 0);
    }
    // well now we wait I guess
    // TODO some kind of a loading circle I guess
}

void PackListItem::forwardToLevelBrowser(GJSearchObject* gjSearchObject, GDDLPackLevelBrowser* callingLayer,
    int actualPageNumber) {
    if (callingLayer != nullptr) {
        callingLayer->handleSearchObject(gjSearchObject, actualPageNumber);
        return;
    }
    const auto levelBrowserLayer = static_cast<GDDLPackLevelBrowser*>(GDDLPackLevelBrowser::create(gjSearchObject));
    levelBrowserLayer->assignPackInfo(PacksManager::getOrRequestPackInfo(packID, true).unwrap().get());
    const auto listLayerScene = CCScene::create();
    listLayerScene->addChild(levelBrowserLayer);
    const auto transition = CCTransitionFade::create(0.5, listLayerScene);
    CCDirector::sharedDirector()->pushScene(transition);
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
    PacksManager::unsubscribeFromObservers(this);
    GJSearchObject* gjSearchObject = Utils::createGJSearchObjectFromIndex(0, PacksManager::getOrRequestPackInfo(packID, true).unwrap()->getLevels());
    forwardToLevelBrowser(gjSearchObject, nullptr, 0);
}
