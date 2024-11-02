// ReSharper disable CppTooWideScopeInitStatement
// ReSharper disable CppHidingFunction
#include <Geode/Geode.hpp>
#include <Geode/Bindings.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/utils/web.hpp>

#include "RatingsManager.h"
#include "Utils.h"
#include "layers/GDDLLevelInfoPopup.h"
#include "settings/ButtonPositionSettingV3.h"
#include "settings/UseOldTierLabelSettingV3.h"
#include "settings/ExcludeRangeSettingV3.h"
#include "layers/GDDLAdvancedLevelInfoPopup.h"

using namespace geode::prelude;

class $modify(GDDLInfoLayer, LevelInfoLayer) {
    struct Fields {
        EventListener<web::WebTask> infoLayerGetRatingListener;
        bool gddlTierUpdated = false;
        GDDLAdvancedLevelInfoPopup* advancedLevelInfoPopup = nullptr;
    };

    // ReSharper disable once CppParameterMayBeConst
    bool init(GJGameLevel *p0, bool p1) {
        if (!LevelInfoLayer::init(p0, p1))
            return false;

        // setup web req
        m_fields->infoLayerGetRatingListener.bind([this] (web::WebTask::Event* e) {
            if (web::WebResponse* res = e->getValue()) {
                const std::string response = res->string().unwrapOrDefault();
                if (response.empty()) {
                    updateButton(-1);
                } else {
                    const int levelID = m_level->m_levelID;
                    int tierAfterFetch = -1;
                    if(RatingsManager::addRatingFromResponse(levelID, response)) {
                        tierAfterFetch = RatingsManager::getDemonTier(levelID);
                    }
                    updateButton(tierAfterFetch);
                    if (m_fields->advancedLevelInfoPopup != nullptr) {
                        m_fields->advancedLevelInfoPopup->addRatingInfo();
                    }
                }
            } else if (e->isCancelled()) {
                updateButton(-1);
            }
        });

        const auto starsLabel = m_starsLabel;
        const bool isDemon = std::stoi(m_starsLabel->getString()) == 10;
        if (starsLabel && isDemon && Utils::notExcluded(m_level->m_levelID)) {
            m_fields->gddlTierUpdated = false;
            const bool displayAsLabel = static_pointer_cast<UseOldTierLabelSettingV3>(Mod::get()->getSettingV3("use-old-tier-label"))->isEnabled();
            if (!displayAsLabel) {
                const auto buttonPositionSetting = static_pointer_cast<ButtonPositionSettingV3>(Mod::get()->getSettingV3("button-position"))->getPosition();
                CCPoint menuPosition, buttonPosition;
                CCSize menuSize;
                float buttonScale = 1.0f;
                if (buttonPositionSetting != DEFAULT) {
                    const auto levelNameLabel = typeinfo_cast<CCLabelBMFont *>(getChildByID("title-label"));
                    const auto levelNamePosition = levelNameLabel->getPosition();
                    const auto levelNameSize = levelNameLabel->getContentSize();
                    if (buttonPositionSetting == TO_THE_RIGHT_OF_THE_LEVEL_TITLE) { // right
                        menuPosition = CCPoint{levelNamePosition.x + levelNameSize.width / 2.5f,
                                               levelNamePosition.y - levelNameSize.height / 2.25f};
                    } else { // left
                        menuPosition = CCPoint{levelNamePosition.x - levelNameSize.width / 2.5f - 25.0f,
                                               levelNamePosition.y - levelNameSize.height / 2.25f};
                    }
                    menuSize = CCSize{25, 25};
                    buttonPosition = CCPoint{12.5f, 12.5f};
                    buttonScale = 0.5f;
                } else {
                    const auto diffPosition = m_difficultySprite->getPosition();
                    const auto diffSize = m_difficultySprite->getContentSize();
                    menuPosition =
                            CCPoint{diffPosition.x - 50 - diffSize.width / 2, diffPosition.y - diffSize.height / 3.2f};
                    menuSize = CCSize{50, 50};
                    buttonPosition = CCPoint{25, 25};
                }
                placeGDDLButton(menuPosition, menuSize, buttonPosition, buttonScale);
            } else {
                const auto tierLabelSprite = CCLabelBMFont::create("Tier ??", "bigFont.fnt");
                tierLabelSprite->setID("gddl-rating_label"_spr);
                float labelShiftRows = 1.0f;
                if (m_level->m_coins > 0) {
                    labelShiftRows += 1.0f;
                }
                const auto moveRowsSetting = static_pointer_cast<UseOldTierLabelSettingV3>(Mod::get()->getSettingV3("use-old-tier-label"))->getPositionOffset();
                if (moveRowsSetting == -1) {
                    labelShiftRows = -4.5f;
                } else {
                    labelShiftRows += static_cast<float>(moveRowsSetting);
                }
                const auto tierLabel =
                        CCMenuItemSpriteExtra::create(tierLabelSprite, this, menu_selector(GDDLInfoLayer::onGDDLInfo));
                tierLabelSprite->setScale(0.4f);
                const auto tierLabelMenu = CCMenu::create();
                tierLabelMenu->setPosition(
                        {starsLabel->getPositionX(), starsLabel->getPositionY() - labelShiftRows * 15.5f});
                tierLabelMenu->addChild(tierLabel, 10);
                addChild(tierLabelMenu);
                reorderChild(tierLabelMenu, 10);
            }

            const int levelID = m_level->m_levelID;
            const int tier = RatingsManager::getDemonTier(levelID);
            if(tier != -1) {
                updateButton(tier);
                m_fields->gddlTierUpdated = true;
            }
        }

        return true;
    }

    void updateLabelValues() {
        LevelInfoLayer::updateLabelValues();
        const auto starsLabel = m_starsLabel;
        const bool isDemon = std::stoi(m_starsLabel->getString()) == 10;
        if (!starsLabel || !isDemon || m_fields->gddlTierUpdated) return;

        // fetch information
        const int levelID = m_level->m_levelID;
        const int tier = RatingsManager::getDemonTier(levelID);

        if (tier == -1) {
            // web request 2.0 yaaay
            auto req = web::WebRequest();
            m_fields->infoLayerGetRatingListener.setFilter(req.get(RatingsManager::getRequestUrl(levelID)));
        }
    }

    void placeGDDLButton(const CCPoint& menuPosition, const CCSize& menuSize, const CCPoint& buttonPosition,
                         const float buttonScale = 1.0f) {
        const auto menu = CCMenu::create();
        menu->setPosition(menuPosition);
        menu->setContentSize(menuSize);
        menu->setScale(buttonScale);
        menu->setID("rating-menu"_spr);
        addChild(menu);

        const int levelID = m_level->m_levelID;
        // just so it displays a bit earlier
        const auto button = CCMenuItemSpriteExtra::create(Utils::getSpriteFromTier(RatingsManager::getCachedTier(levelID)), this, menu_selector(GDDLInfoLayer::onGDDLInfo));
        button->setPosition(buttonPosition);
        button->setID("rating"_spr);
        menu->addChild(button);
    }

    void updateButton(const int tier) {
        const bool displayAsLabel = static_pointer_cast<UseOldTierLabelSettingV3>(Mod::get()->getSettingV3("use-old-tier-label"))->isEnabled();
        if (!displayAsLabel) {
            const auto menu = typeinfo_cast<CCMenu*>(getChildByID("rating-menu"_spr));
            if (!menu)
                return;
            const auto tierButton = typeinfo_cast<CCMenuItemSpriteExtra*>(menu->getChildByID("rating"_spr));
            if (!tierButton) return;
            tierButton->removeAllChildren();

            const auto tierSprite = Utils::getSpriteFromTier(tier);
            tierButton->addChild(tierSprite);
        } else {
            const auto tierLabelSprite = typeinfo_cast<CCLabelBMFont*>(getChildByIDRecursive("gddl-rating_label"_spr));
            if (!tierLabelSprite)
                return;
            const std::string newLabelContent = "Tier " + std::to_string(tier);
            tierLabelSprite->setString(newLabelContent.c_str());
            tierLabelSprite->setColor(RatingsManager::getTierColor(tier));
        }
    }

    // ReSharper disable once CppMemberFunctionMayBeConst
    void onGDDLInfo(CCObject *sender) {
        if (Mod::get()->getSettingValue<bool>("use-old-info-popup")) {
            GDDLLevelInfoPopup::create(m_level->m_levelID)->show();
        } else {
            m_fields->advancedLevelInfoPopup = GDDLAdvancedLevelInfoPopup::create(m_level);
            m_fields->advancedLevelInfoPopup->show();
        }
    }
};
