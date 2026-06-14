#include "GDDLPacksLayer.h"

#include <Utils.h>
#include <Geode/ui/Layout.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <nodes/PackListItem.h>
#include <nodes/searchcontrols/CheckboxInputControl.h>
#include <objects/searchsettings/BoolSearchSetting.h>

bool GDDLPacksLayer::init() {
    if (!GDDLThemeListLayer::init(false)) return false;

    const auto winSize = CCDirector::sharedDirector()->getWinSize();
    // base UI already created by GDDLThemeListLayer

    // next/prev page arrows, positions are really weird because math doesn't work for some reason here
    const auto nextPageMenu = CCMenu::create();
    nextPageMenu->setContentSize({50.0f, 50.0f});
    nextPageMenu->setPosition({120.0f + listSize.x, winSize.height / 2 - 25.0f});
    this->addChild(nextPageMenu);
    const auto nextButtonSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    nextButtonSprite->setFlipX(true);
    const auto nextPageButton = CCMenuItemSpriteExtra::create(nextButtonSprite, this, menu_selector(GDDLPacksLayer::onNextPage));
    nextPageButton->setPosition({25.0f, 25.0f});
    nextPageMenu->addChild(nextPageButton);

    const auto prevPageMenu = CCMenu::create();
    prevPageMenu->setContentSize({50.0f, 50.0f});
    prevPageMenu->setPosition({44.0f, winSize.height / 2 - 25.0f});
    this->addChild(prevPageMenu);
    const auto prevButtonSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    const auto prevPageButton = CCMenuItemSpriteExtra::create(prevButtonSprite, this, menu_selector(GDDLPacksLayer::onPrevPage));
    prevPageButton->setPosition({25.0f, 25.0f});
    prevPageMenu->addChild(prevPageButton);

    // title label
    titleLabel = CCLabelBMFont::create("Packs", "bigFont.fnt");
    titleLabel->setPosition({winSize.width / 2, winSize.height / 2 + listSize.y / 2 + 30.0f});
    this->addChild(titleLabel);

    // request packs
    auto req = web::WebRequest();
    req.header("User-Agent", Utils::getUserAgent());
    packsTaskHolder.spawn(req.get(packsRequestApiUrl), getPacksDownloadLambda());

    return true;
}

void GDDLPacksLayer::updateList() {
    scrollList->m_contentLayer->removeAllChildren();
    for (const auto& packInfo : packInfos[page]) {
        scrollList->m_contentLayer->addChild(PackListItem::create(356.0f, packInfo));
    }
    scrollList->m_contentLayer->setLayout(ScrollLayer::createDefaultListLayout());
    scrollList->scrollToTop();
    titleLabel->setString(packCategoryInfos[page].getName().c_str());
}

std::function<void(web::WebResponse)> GDDLPacksLayer::getPacksDownloadLambda() {
    return [this](web::WebResponse res) {
        if (res.code() != 200) {
            // const auto jsonResponse = res.json().unwrapOr(matjson::Value());
            // const std::string errorMessage = "GDDL: Search failed - " + Utils::getErrorFromMessageAndResponse(jsonResponse, res);
            // Notification::create(errorMessage, NotificationIcon::Error, 2)->show();
            // const std::string rawResponse = jsonResponse.contains("message") ? jsonResponse.dump(0) : res.string().unwrapOr("Response was not a valid string");
            // log::error("SearchObject::getSearchLambda: [{}] {}, raw response: {}", res.code(), errorMessage, rawResponse);
            return;
        }
        const auto jsonResponse = res.json().unwrapOr(matjson::Value());
        if (!jsonResponse.contains("packs") || !jsonResponse["packs"].isArray() ||
            !jsonResponse.contains("categories") || !jsonResponse["categories"].isArray()) {
            // TODO error
            log::info("invalid outer json");
            return;
        }
        for (const auto packInfoObject : jsonResponse["packs"].asArray().unwrap()) {
            const Result<std::shared_ptr<PackInfo>> maybePackInfo = PackInfo::createFromJson(packInfoObject);
            if (maybePackInfo.isErr()) {
                // TODO error
                log::info("invalid inner pack json: {}", packInfoObject.dump());
                return;
            }
            const std::shared_ptr<PackInfo> packInfo = maybePackInfo.unwrap();
            packInfos[packInfo->getCategoryId()].push_back(packInfo);
        }
        for (const auto packCategoryInfoObject : jsonResponse["categories"].asArray().unwrap()) {
            const Result<PackCategoryInfo> maybePackCategoryInfo = PackCategoryInfo::createFromJson(packCategoryInfoObject);
            if (maybePackCategoryInfo.isErr()) {
                // TODO error
                log::info("invalid inner category json: {}", packCategoryInfoObject.dump());
                return;
            }
            const PackCategoryInfo& packCategoryInfo = maybePackCategoryInfo.unwrap();
            highestPage = std::max(highestPage, packCategoryInfo.getId());
            packCategoryInfos[packCategoryInfo.getId()] = packCategoryInfo;
        }
        updateList();
    };
}

void GDDLPacksLayer::onNextPage(CCObject* sender) {
    ++page;
    if (page > highestPage) page = 1;
    updateList();
}

void GDDLPacksLayer::onPrevPage(CCObject* sender) {
    --page;
    if (page < 1) page = highestPage;
    updateList();
}

void GDDLPacksLayer::onBack(CCObject* sender) {
    backActions();
}

void GDDLPacksLayer::keyBackClicked() {
    backActions();
}

void GDDLPacksLayer::backActions() {
    CCDirector::get()->popScene();
}

GDDLPacksLayer* GDDLPacksLayer::create() {
    const auto ret = new GDDLPacksLayer();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }
    delete ret;
    return nullptr;
}

GDDLPacksLayer* GDDLPacksLayer::scene() {
    const auto scene = CCScene::create();
    const auto layer = create();
    scene->addChild(layer);
    CCDirector::sharedDirector()->pushScene(CCTransitionFade::create(0.5f, scene));
    return layer;
}
