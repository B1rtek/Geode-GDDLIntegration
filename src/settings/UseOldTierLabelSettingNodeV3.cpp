#include "UseOldTierLabelSettingNodeV3.h"

#include "Utils.h"

bool UseOldTierLabelSettingNodeV3::init(std::shared_ptr<UseOldTierLabelSettingV3> setting, float width) {
    if (!SettingNodeV3::init(setting, width)) {
        return false;
    }

    // setting UI
    Utils::createCheckbox(this->getButtonMenu(), enabledToggler, "Enable", 15.0f, 0.6f, {36.0f, 20.0f}, this,
                          menu_selector(UseOldTierLabelSettingNodeV3::onToggleEnabled));
    Utils::createTextInputNode(this->getButtonMenu(), positionOffsetTextfield, "bigFont.fnt", "", 2, {35.0f, 25.0f},
                               {121.0f, 15.0f});
    Utils::createLeftRightButtonsAround(positionOffsetTextfield, {13.0f, 19.0f}, this,
                                        menu_selector(UseOldTierLabelSettingNodeV3::onPositionOffsetLeft),
                                        menu_selector(UseOldTierLabelSettingNodeV3::onPositionOffsetRight));
    Utils::createLabel(this->getButtonMenu(), "chatFont.fnt", "Offset:", 50.0f, {71.0f, 15.0f});

    this->getButtonMenu()->setContentWidth(170.0f);
    this->getButtonMenu()->updateLayout();

    // load values
    loadValues();
    this->updateState(nullptr);

    return true;
}

void UseOldTierLabelSettingNodeV3::loadValues() {
    currentEnabled = this->getSetting()->isEnabled();
    currentPositionOffset = this->getSetting()->getPositionOffset();
}

void UseOldTierLabelSettingNodeV3::updateState(CCNode *invoker) {
    SettingNodeV3::updateState(invoker);
    if (!dontTouchTheToggler) {
        enabledToggler->toggle(currentEnabled);
    }
    positionOffsetTextfield->setString(std::to_string(currentPositionOffset).c_str());
}

void UseOldTierLabelSettingNodeV3::onPositionOffsetLeft(CCObject *sender) {
    --currentPositionOffset;
    if (currentPositionOffset < UseOldTierLabelSettingV3::minOffset) {
        currentPositionOffset = UseOldTierLabelSettingV3::maxOffset;
    }
    this->markChanged(nullptr);
}

void UseOldTierLabelSettingNodeV3::onPositionOffsetRight(CCObject *sender) {
    ++currentPositionOffset;
    if (currentPositionOffset > UseOldTierLabelSettingV3::maxOffset) {
        currentPositionOffset = UseOldTierLabelSettingV3::minOffset;
    }
    this->markChanged(nullptr);
}

// i absolutely hate togglers
void UseOldTierLabelSettingNodeV3::onToggleEnabled(CCObject *sender) {
    currentEnabled = !enabledToggler->isOn();
    // amazing workaround for broken animation
    dontTouchTheToggler = true;
    this->markChanged(nullptr);
    dontTouchTheToggler = false;
}

void UseOldTierLabelSettingNodeV3::onCommit() {
    this->getSetting()->setEnabled(currentEnabled);
    this->getSetting()->setPositionOffset(currentPositionOffset);
}

void UseOldTierLabelSettingNodeV3::onResetToDefault() {
    currentEnabled = UseOldTierLabelSettingV3::defaultEnabled;
    currentPositionOffset = UseOldTierLabelSettingV3::defaultPositionOffset;
    this->markChanged(nullptr);
}

bool UseOldTierLabelSettingNodeV3::hasUncommittedChanges() const {
    return this->getSetting()->isEnabled() != currentEnabled ||
           this->getSetting()->getPositionOffset() != currentPositionOffset;
}

bool UseOldTierLabelSettingNodeV3::hasNonDefaultValue() const {
    return !this->getSetting()->isDefaultValue();
}

std::shared_ptr<UseOldTierLabelSettingV3> UseOldTierLabelSettingNodeV3::getSetting() const {
    return std::static_pointer_cast<UseOldTierLabelSettingV3>(SettingNodeV3::getSetting());
}

UseOldTierLabelSettingNodeV3 *
UseOldTierLabelSettingNodeV3::create(std::shared_ptr<UseOldTierLabelSettingV3> setting, float width) {
    auto ret = new UseOldTierLabelSettingNodeV3();
    if(ret && ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
