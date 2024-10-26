#ifndef GDDLINTEGRATION_CACHERESETSETTINGNODEV3_H
#define GDDLINTEGRATION_CACHERESETSETTINGNODEV3_H

#include <Geode/loader/SettingV3.hpp>
#include <Geode/utils/web.hpp>
#include "DummySettingV3.h"

using namespace geode::prelude;

class CacheResetSettingNodeV3 : public SettingNodeV3 {
protected:
    EventListener<web::WebTask> cacheEventListener;

    bool init(std::shared_ptr<DummySettingV3> setting, float width);
    void updateState(CCNode* invoker) override;

    void onCacheResetButtonClicked(CCObject *sender);

    void onCommit() override;
    void onResetToDefault() override;

public:
    bool hasUncommittedChanges() const override;
    bool hasNonDefaultValue() const override;

    static CacheResetSettingNodeV3* create(std::shared_ptr<DummySettingV3> setting, float width);
};


#endif //GDDLINTEGRATION_CACHERESETSETTINGNODEV3_H
