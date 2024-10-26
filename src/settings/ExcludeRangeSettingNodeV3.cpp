#include "ExcludeRangeSettingNodeV3.h"

#include "Utils.h"

bool ExcludeRangeSettingNodeV3::init(std::shared_ptr<ExcludeRangeSettingV3> setting, float width) {
    if (!SettingNodeV3::init(setting, width)) {
        return false;
    }

    // range textboxes
    Utils::createTextInputNode(this->getButtonMenu(), textfields[0], "bigFont.fnt", "", 2, {35.0f, 25.0f},
                               {40.0f, 15.0f});
    Utils::createLeftRightButtonsAround(textfields[0], {13.0f, 19.0f}, this,
                                        menu_selector(ExcludeRangeSettingNodeV3::onRangeBeginLeft),
                                        menu_selector(ExcludeRangeSettingNodeV3::onRangeBeginRight));
    Utils::createTextInputNode(this->getButtonMenu(), textfields[1], "bigFont.fnt", "", 2, {35.0f, 25.0f},
                               {115.0f, 15.0f});
    Utils::createLeftRightButtonsAround(textfields[1], {13.0f, 19.0f}, this,
                                        menu_selector(ExcludeRangeSettingNodeV3::onRangeEndLeft),
                                        menu_selector(ExcludeRangeSettingNodeV3::onRangeEndRight));
    Utils::createLabel(this->getButtonMenu(), "chatFont.fnt", "to", 30.0f, {77.5f, 15.0f});

    // include/exclude toggle
    Utils::createCheckbox(this->getButtonMenu(), includeToggler, "Include", 15.0f, 0.6f, {170.0f, 20.0f}, this,
                          menu_selector(ExcludeRangeSettingNodeV3::onToggleInclude));

    this->getButtonMenu()->setContentWidth(190.0f);
    this->getButtonMenu()->updateLayout();

    // load values
    loadValues();
    this->updateState(nullptr);

    return true;
}

void ExcludeRangeSettingNodeV3::loadValues() {
    currentRangeBegin = this->getSetting()->getRangeBegin();
    currentRangeEnd = this->getSetting()->getRangeEnd();
    currentInclude = this->getSetting()->isInclude();
}

void ExcludeRangeSettingNodeV3::updateState(CCNode *invoker) {
    SettingNodeV3::updateState(invoker);
    if (!dontTouchTheToggler) {
        includeToggler->toggle(currentInclude);
    }
    Utils::setNumberWithDefZeroTextfield(currentRangeBegin, textfields[0]);
    Utils::setNumberWithDefZeroTextfield(currentRangeEnd, textfields[1]);
}

void ExcludeRangeSettingNodeV3::onRangeBeginLeft(CCObject *sender) {
    --currentRangeBegin;
    if (currentRangeBegin < 0)
        currentRangeBegin = ExcludeRangeSettingV3::highestTier;
    this->markChanged(nullptr);
}

void ExcludeRangeSettingNodeV3::onRangeBeginRight(CCObject *sender) {
    ++currentRangeBegin;
    if (currentRangeBegin > ExcludeRangeSettingV3::highestTier)
        currentRangeBegin = 0;
    this->markChanged(nullptr);
}

void ExcludeRangeSettingNodeV3::onRangeEndLeft(CCObject *sender) {
    --currentRangeEnd;
    if (currentRangeEnd < 0)
        currentRangeEnd = ExcludeRangeSettingV3::highestTier;
    this->markChanged(nullptr);
}

void ExcludeRangeSettingNodeV3::onRangeEndRight(CCObject *sender) {
    ++currentRangeEnd;
    if (currentRangeEnd > ExcludeRangeSettingV3::highestTier)
        currentRangeEnd = 0;
    this->markChanged(nullptr);
}

void ExcludeRangeSettingNodeV3::onToggleInclude(CCObject *sender) {
    currentInclude = !includeToggler->isOn();
    // amazing workaround for broken animation
    dontTouchTheToggler = true;
    this->markChanged(nullptr);
    dontTouchTheToggler = false;
}

void ExcludeRangeSettingNodeV3::onCommit() {
    this->getSetting()->setRangeBegin(currentRangeBegin);
    this->getSetting()->setRangeEnd(currentRangeEnd);
    this->getSetting()->setInclude(currentInclude);
}

void ExcludeRangeSettingNodeV3::onResetToDefault() {
    currentRangeBegin = ExcludeRangeSettingV3::defaultRangeBegin;
    currentRangeEnd = ExcludeRangeSettingV3::defaultRangeEnd;
    currentInclude = ExcludeRangeSettingV3::defaultInclude;
    this->markChanged(nullptr);
}

bool ExcludeRangeSettingNodeV3::hasUncommittedChanges() const {
    return this->getSetting()->getRangeBegin() != currentRangeBegin ||
           this->getSetting()->getRangeEnd() != currentRangeEnd || this->getSetting()->isInclude() != currentInclude;
}

bool ExcludeRangeSettingNodeV3::hasNonDefaultValue() const {
    return !this->getSetting()->isDefaultValue();
}

std::shared_ptr<ExcludeRangeSettingV3> ExcludeRangeSettingNodeV3::getSetting() const {
    return std::static_pointer_cast<ExcludeRangeSettingV3>(SettingNodeV3::getSetting());

}

ExcludeRangeSettingNodeV3 *
ExcludeRangeSettingNodeV3::create(std::shared_ptr<ExcludeRangeSettingV3> setting, float width) {
    auto ret = new ExcludeRangeSettingNodeV3();
    if(ret && ret->init(setting, width)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}
