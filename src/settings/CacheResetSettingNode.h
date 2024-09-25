#ifndef GDDLINTEGRATION_CACHERESETSETTINGNODE_H
#define GDDLINTEGRATION_CACHERESETSETTINGNODE_H


#include <Geode/loader/SettingNode.hpp>
#include <Geode/utils/web.hpp>

#include "DummySettingCR.h"

using namespace geode::prelude;

class CacheResetSettingNode : public SettingNode {
protected:
    EventListener<web::WebTask> cacheEventListener;

    bool init(DummySettingCR* value, float width);

    void onCacheResetButtonClicked(CCObject *sender);
    void onInfo(CCObject *sender);

public:
    void commit() override;
    bool hasUncommittedChanges() override;
    bool hasNonDefaultValue() override;
    void resetToDefault() override;
    static CacheResetSettingNode* create(DummySettingCR* value, float width);
};


#endif //GDDLINTEGRATION_CACHERESETSETTINGNODE_H
