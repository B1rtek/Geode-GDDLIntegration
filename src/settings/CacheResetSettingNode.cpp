#include "CacheResetSettingNode.h"

#include <Utils.h>

bool CacheResetSettingNode::init(DummySettingCR *value, float width) {
    if (!SettingNode::init(value)) {
        return false;
    }
    this->setContentSize({width, 40.f});

    const auto menu = CCMenu::create();
    menu->setContentSize(this->getContentSize());

    // reset button
    const auto resetButtonSprite = ButtonSprite::create("Reset GDDL cache", "bigFont.fnt", "GJ_button_04.png");
    resetButtonSprite->setScale(0.6f);
    const auto resetButton = CCMenuItemSpriteExtra::create(resetButtonSprite, this, menu_selector(CacheResetSettingNode::onCacheResetButtonClicked));
    menu->addChild(resetButton);
    resetButton->setPosition({menu->getContentWidth() / 2, menu->getContentHeight() / 2});

    // info button
    const auto infoButtonSprite = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
    infoButtonSprite->setScale(0.6f);
    const auto infoButton = CCMenuItemSpriteExtra::create(infoButtonSprite, this,
                                                          menu_selector(CacheResetSettingNode::onInfo));
    menu->addChild(infoButton);
    infoButton->setPosition({menu->getContentWidth() / 2 - resetButton->getScaledContentWidth() / 2 -
                             3 * infoButton->getScaledContentWidth(), menu->getContentHeight() / 2});

    this->addChild(menu);
    this->updateLayout();
    menu->setPosition({0.0f, 0.0f});

    // no values to load

    cocos::handleTouchPriority(this);
    return true;
}

void CacheResetSettingNode::onCacheResetButtonClicked(CCObject *sender) {
    RatingsManager::clearCache();
    Utils::bindCacheDownloadCallback(cacheEventListener, true);
    auto req = web::WebRequest();
    cacheEventListener.setFilter(req.get(RatingsManager::gddlSheetUrl));
}

void CacheResetSettingNode::onInfo(CCObject *sender) {
    FLAlertLayer::create("Reset GDDL cache",
                         "Use if the <cr>GDDL Split layer</c> shows <cy>zero</c> for every tier or if the ratings on the level info page are <co>inaccurate</c>",
                         "OK")->show();
}

void CacheResetSettingNode::commit() {
    this->dispatchCommitted();
}

bool CacheResetSettingNode::hasUncommittedChanges() {
    return false;
}

bool CacheResetSettingNode::hasNonDefaultValue() {
    return false;
}

void CacheResetSettingNode::resetToDefault() {

}

CacheResetSettingNode *CacheResetSettingNode::create(DummySettingCR *value, float width) {
    auto ret = new CacheResetSettingNode;
    if (ret && ret->init(value, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
