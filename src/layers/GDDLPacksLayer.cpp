#include "GDDLPacksLayer.h"

#include <Utils.h>
#include <Geode/ui/Layout.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <nodes/PackListItem.h>
#include <nodes/searchcontrols/CheckboxInputControl.h>
#include <objects/searchsettings/BoolSearchSetting.h>

bool GDDLPacksLayer::init() {
    if (!CCLayer::init()) return false;

    // bg
    const auto winSize = CCDirector::sharedDirector()->getWinSize();
    const auto bg = CCSprite::create("GJ_gradientBG.png");
    bg->setAnchorPoint({0.0f, 0.0f});
    bg->setPosition({-5.0f, -5.0f});
    bg->setZOrder(-3);
    bg->setScaleX((winSize.width + 10.0f) / bg->getContentWidth());
    bg->setScaleY((winSize.height + 10.0f) / bg->getContentHeight());
    this->addChild(bg);

    // TODO cornerpieces

    // definitely not taken from Geode's ModsLayer
    const auto backMenu = CCMenu::create();
    backMenu->setID("back-menu");
    backMenu->setContentSize({100.f, 40.f});
    backMenu->setAnchorPoint({ .0f, .5f });

    const auto backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    const auto backBtn = CCMenuItemSpriteExtra::create(
        backSpr, this, menu_selector(GDDLPacksLayer::onBack)
    );
    backBtn->setID("back-button");
    backMenu->addChild(backBtn);
    backMenu->setLayout(SimpleRowLayout::create()->setMainAxisAlignment(MainAxisAlignment::Start)->setGap(5.f));
    this->addChildAtPosition(backMenu, Anchor::TopLeft, ccp(8, -23), false);

    // list
    packsList = ScrollLayer::create({356.0f, 220.0f});
    packsList->setPosition({winSize.width / 2 - listSize.x / 2, winSize.height / 2 - listSize.y / 2});
    // for (int i = 0; i < 5; i++) {
    //     // TODO a bunch of placeholders, replace with actual content later
    //     packsList->m_contentLayer->addChild(PackListItem::create(356.0f, PackInfo({}, "Pack " + std::to_string(i+1), "tier_unrated.png")));
    // }
    packsList->m_contentLayer->setLayout(ScrollLayer::createDefaultListLayout());
    packsList->scrollToTop();
    this->addChild(packsList);
    createListFrame();

    // next/prev page arrows
    const auto nextPageMenu = CCMenu::create();
    nextPageMenu->setContentSize({50.0f, 50.0f});
    nextPageMenu->setPosition({winSize.width / 2 + listSize.x / 2 + 50.0f, winSize.height / 2});
    this->addChild(nextPageMenu);
    const auto nextButtonSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    nextButtonSprite->setFlipX(true);
    const auto nextPageButton = CCMenuItemSpriteExtra::create(nextButtonSprite, this, menu_selector(GDDLPacksLayer::onNextPage));
    nextPageButton->setPosition({25.0f, 25.0f});
    nextPageMenu->addChild(nextPageButton);

    const auto prevPageMenu = CCMenu::create();
    prevPageMenu->setContentSize({50.0f, 50.0f});
    prevPageMenu->setPosition({winSize.width / 2 - listSize.x / 2 - 50.0f, winSize.height / 2});
    this->addChild(prevPageMenu);
    const auto prevButtonSprite = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
    const auto prevPageButton = CCMenuItemSpriteExtra::create(prevButtonSprite, this, menu_selector(GDDLPacksLayer::onPrevPage));
    prevPageButton->setPosition({25.0f, 25.0f});
    prevPageMenu->addChild(prevPageButton);

    // title label
    titleLabel = CCLabelBMFont::create("Packs", "bigFont.fnt");
    titleLabel->setPosition({winSize.width / 2, winSize.height / 2 + listSize.y / 2 + 40.0f});
    this->addChild(titleLabel);

    // TODO rest of the UI

    // request packs
    auto req = web::WebRequest();
    req.header("User-Agent", Utils::getUserAgent());
    packsTaskHolder.spawn(req.get(packsRequestApiUrl), getPacksDownloadLambda());

    return true;
}

void GDDLPacksLayer::createListFrame() {
    const std::vector<CCPoint> sidePositions = {
        {packsList->getPositionX() - 8.0f, packsList->getContentHeight() / 2 + packsList->getPositionY()}, // left
        {packsList->getContentWidth() / 2  + packsList->getPositionX(), packsList->getContentHeight() + packsList->getPositionY() + 7.5f}, // top
        {packsList->getContentWidth() + packsList->getPositionX() + 8.0f, packsList->getContentHeight() / 2 + packsList->getPositionY()}, // right
        {packsList->getContentWidth() / 2  + packsList->getPositionX(), packsList->getPositionY() - 7.5f} // bottom
    };
    for (int i = 0; i < sidePositions.size(); i++) {
        const auto sideSprite = CCSprite::createWithSpriteFrameName("GJ_table_side_001.png");
        sideSprite->setPosition(sidePositions[i]);
        sideSprite->setRotation(i * 90.0f);
        sideSprite->setScaleY(((i % 2 == 0 ? packsList->getContentHeight() : packsList->getContentWidth()) + 2.0f) / sideSprite->getContentHeight());
        this->addChild(sideSprite);
    }
    const std::vector<CCPoint> cornerPositions = {
        {packsList->getPositionX() - 8.0f, packsList->getContentHeight() + packsList->getPositionY() + 7.5f}, // top left
        {packsList->getContentWidth() + packsList->getPositionX() + 8.0f, packsList->getContentHeight() + packsList->getPositionY() + 7.5f}, // top right
        {packsList->getContentWidth() + packsList->getPositionX() + 8.0f, packsList->getPositionY() - 7.5f}, // bottom right
        {packsList->getPositionX() - 8.0f, packsList->getPositionY() - 7.5f} // bottom left
    };
    for (int i = 0; i < cornerPositions.size(); i++) {
        const auto cornerSprite = CCSprite::createWithSpriteFrameName("GJ_table_corner_001.png");
        cornerSprite->setPosition(cornerPositions[i]);
        cornerSprite->setRotation(i * 90.0f);
        this->addChild(cornerSprite);
    }
}

void GDDLPacksLayer::updateList() {
    packsList->m_contentLayer->removeAllChildren();
    for (const auto& packInfo : packInfos[page]) {
        packsList->m_contentLayer->addChild(PackListItem::create(356.0f, packInfo));
    }
    packsList->m_contentLayer->setLayout(ScrollLayer::createDefaultListLayout());
    packsList->scrollToTop();
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
            const Result<PackInfo> maybePackInfo = PackInfo::createFromJson(packInfoObject);
            if (maybePackInfo.isErr()) {
                // TODO error
                log::info("invalid inner pack json: {}", packInfoObject.dump());
                return;
            }
            const PackInfo packInfo = maybePackInfo.unwrap();
            packInfos[packInfo.getCategoryId()].push_back(packInfo);
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
