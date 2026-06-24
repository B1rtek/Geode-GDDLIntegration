#include "PackListItem.h"

#include <Utils.h>
#include <Values.h>
#include <Geode/loader/Mod.hpp>
#include <Geode/ui/LazySprite.hpp>
#include <managers/PacksManager.h>
#include <modified/GDDLPackLevelBrowser.h>

bool PackListItem::init(const float width, const int packID, bool dark) {
    if (!CCNode::init()) return false;
    log::info("[({})PackListItem::init] Creating PackListItem for pack {}", fmt::ptr(this), packID);

    this->setContentSize({width, itemHeight});
    this->packID = packID;
    const std::shared_ptr<PackInfo> packInfo = PacksManager::getOrRequestPackInfo(packID, false).unwrap(); // should never be Err

    // bg
    const auto bg = CCLayerColor::create(dark ? ccc4(0, 0, 0, 50) : ccc4(80, 80, 80, 50), width, itemHeight);
    bg->setAnchorPoint({0.0f, 0.0f});
    bg->setPosition({0.0f, 0.0f});
    bg->setZOrder(-1);
    this->addChild(bg);

    // icon
    const auto icon = LazySprite::create({20.0f, 20.0f});
    icon->setLoadCallback([icon](Result<> res) {
        if (!res) {
            icon->initWithFile(Mod::get()->expandSpriteName("tier_unrated.png").data());
            icon->setScale(0.275f);
        } else {
            icon->setScale(32.0f / icon->getContentWidth());
        }
    });
    icon->loadFromUrl(Values::packIconsBaseUrl + packInfo->getIconPath());
    icon->setPosition({20.0f, itemHeight / 2.0f});
    icon->setScale(32.0f / icon->getContentWidth());
    this->addChild(icon);

    // title
    auto title = CCLabelBMFont::create(packInfo->getName().c_str(), "bigFont.fnt");
    title->setAnchorPoint({0.0f, 0.5f});
    title->setScale(0.6f);
    title->setPosition({50.0f, 5.0f * itemHeight / 7.0f});
    Utils::scaleLabelToWidth(title, 210.0f);
    this->addChild(title);

    // median tier
    const auto buttonSprite = Utils::getSpriteFromTier(packInfo->getMedianTier());
    buttonSprite->setAnchorPoint({0.5f, 0.5f});
    buttonSprite->setScale(0.125f);
    buttonSprite->setPosition({title->getPositionX() + title->getScaledContentWidth() + 15.0f, title->getPositionY() - 1.5f});
    this->addChild(buttonSprite);

    // progressbar
    this->progressBar = ProgressBar::create(ProgressBarStyle::Solid);
    this->progressBar->showProgressLabel(true);
    this->progressBar->setScale(232.5f / this->progressBar->getContentWidth());
    this->progressBar->setAnchorPoint({0.0f, 0.5f});
    this->progressBar->setPosition({50.0f, 2.0f * itemHeight / 7.0f});
    this->addChild(this->progressBar);
    // progressbar data
    updateProgressbar();


    // view button
    const auto buttonMenu = CCMenu::create();
    buttonMenu->setContentSize({itemHeight * 1.5f, itemHeight});
    buttonMenu->setPosition({width - itemHeight * 1.5f, 0});
    this->addChild(buttonMenu);
    const auto viewButtonSprite = ButtonSprite::create("View", "bigFont.fnt", "GJ_button_06.png");
    viewButtonSprite->setScale(0.6f);
    const auto viewButton = CCMenuItemSpriteExtra::create(viewButtonSprite, this, menu_selector(PackListItem::onView));
    viewButton->setPosition({itemHeight * 0.75f, itemHeight / 2});
    buttonMenu->addChild(viewButton);

    log::info("[({})PackListItem::init] Created PackListItem", fmt::ptr(this), packID);
    return true;
}

void PackListItem::updateProgressbar() {
    const std::shared_ptr<PackInfo> packInfo = PacksManager::getOrRequestPackInfo(packID, false).unwrap(); // should never be Err
    if (packInfo->getLevels().empty()) {
        this->progressBar->updateProgress(100.0f);
        this->progressBar->setFillColor(ccc3(255, 84, 50));
        this->progressBar->getProgressLabel()->setString("Not yet downloaded");
    } else {
        const auto [completedFraction, baseCompleted] = packInfo->getCompletedFraction();
        const float progress = 100.0f * static_cast<float>(completedFraction.first) / static_cast<float>(completedFraction.second);
        this->progressBar->updateProgress(progress);
        this->progressBar->setFillColor(baseCompleted ? ccc3(0, 255, 255) : ccc3(0, 255, 0));
        const std::string progressString = std::to_string(completedFraction.first) + "/" + std::to_string(completedFraction.second);
        this->progressBar->getProgressLabel()->setString(progressString.c_str());
    }
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

PackListItem* PackListItem::create(const float width, const int packID, bool dark) {
    const auto ret = new PackListItem();
    if (ret->init(width, packID, dark)) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

void PackListItem::draw() {
    CCNode::draw();
    constexpr float lineWidth = 0.3f;
    ccDrawSolidRect({0.0f, lineWidth/2.0f}, {this->getContentWidth(), 0.0f}, ccc4f(0.0f, 0.0f, 0.0f, 0.5f));
    ccDrawSolidRect({0.0f, this->getContentHeight()}, {this->getContentWidth(), this->getContentHeight()-lineWidth/2.0f}, ccc4f(0.0f, 0.0f, 0.0f, 0.5f));
}

void PackListItem::updateData() {
    log::info("[({})PackListItem::updateData] Called", fmt::ptr(this));
    PacksManager::unsubscribeFromObservers(this);
    log::info("[({})PackListItem::updateData] Unsubscribed from PacksManager", fmt::ptr(this));
    const auto maybePackInfo = PacksManager::getOrRequestPackInfo(packID, true);
    if (maybePackInfo.isOk()) {
        updateProgressbar();
        log::info("[({})PackListItem::updateData] Forwarding to level browser", fmt::ptr(this));
        GJSearchObject* gjSearchObject = Utils::createGJSearchObjectFromIndex(0, maybePackInfo.unwrap()->getLevels());
        forwardToLevelBrowser(gjSearchObject);
    } else {
        log::info("[({})PackListItem::updateData] Reached end with invalid PackInfo", fmt::ptr(this));
    }
}
