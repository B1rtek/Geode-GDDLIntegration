#include "DummySettingCR.h"

#include "CacheResetSettingNode.h"

bool DummySettingCR::load(const matjson::Value &json) {
    return true;
}

bool DummySettingCR::save(matjson::Value &json) const {
    return true;
}

SettingNode *DummySettingCR::createNode(float width) {
    return CacheResetSettingNode::create(this, width);
}
