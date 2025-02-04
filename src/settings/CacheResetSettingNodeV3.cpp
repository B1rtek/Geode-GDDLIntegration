#include "CacheResetSettingNodeV3.h"

#include "RatingsManager.h"
#include "Utils.h"

bool CacheResetSettingNodeV3::init(std::shared_ptr<DummySettingV3> setting, float width) {
    if (!SettingNodeV3::init(setting, width)) {
        return false;
    }

    // reset button
    const auto resetButtonSprite = ButtonSprite::create("Reset", "bigFont.fnt", "GJ_button_04.png");
    resetButtonSprite->setScale(0.6f);
    const auto resetButton = CCMenuItemSpriteExtra::create(resetButtonSprite, this, menu_selector(CacheResetSettingNodeV3::onCacheResetButtonClicked));

    this->getButtonMenu()->addChildAtPosition(resetButton, Anchor::Center);
    this->getButtonMenu()->setContentWidth(150);
    this->getButtonMenu()->updateLayout();

    return true;
}

void CacheResetSettingNodeV3::updateState(CCNode *invoker) {
    SettingNodeV3::updateState(invoker);
}

void CacheResetSettingNodeV3::onCacheResetButtonClicked(CCObject *sender) {
    RatingsManager::clearCache();
    Utils::bindCacheDownloadCallback(cacheEventListener, true);
    auto req = web::WebRequest();
    req.header("User-Agent", Utils::getUserAgent());
    cacheEventListener.setFilter(req.get(RatingsManager::gddlSheetUrl));
}

void CacheResetSettingNodeV3::onCommit() {

}

void CacheResetSettingNodeV3::onResetToDefault() {

}

bool CacheResetSettingNodeV3::hasUncommittedChanges() const {
    return false;
}

bool CacheResetSettingNodeV3::hasNonDefaultValue() const {
    return false;
}

CacheResetSettingNodeV3 *CacheResetSettingNodeV3::create(std::shared_ptr<DummySettingV3> setting, float width) {
    auto ret = new CacheResetSettingNodeV3();
    if (ret && ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
