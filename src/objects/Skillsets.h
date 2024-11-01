#ifndef SKILLSETS_H
#define SKILLSETS_H

#include <vector>
#include <matjson.hpp>
#include <string>
#include <Geode/Prelude.hpp>

#include "layers/GDDLAdvancedLevelInfoPopup.h"

using namespace geode::prelude;

class Skillset {
    std::string icon, name, description;
    float iconScale;

public:
    Skillset() = default;

    Skillset(const std::string &icon, const std::string &name, const std::string &description, float iconScale = 1.0f) : icon(
            icon), name(name), description(description), iconScale(iconScale) {}

    CCSprite * getButtonSprite() const {
        const auto buttonSprite = CCSprite::createWithSpriteFrameName(icon.c_str());
        buttonSprite->setScale(iconScale * 0.5f);
        return buttonSprite;
    }

    std::string getName() const {
        return name;
    }

    std::string getDescription() const {
        return description;
    }
};

class Skillsets {
    std::vector<std::pair<int, int>> skillsets{};

public:
    // perhaps make this list refresh itself in the future or sth
    const static inline std::vector<Skillset> skillsetsList = {
            Skillset(), // just to align them with the IDs that start with 1
            Skillset("gj_iconBtn_on_001.png", "Cube", "This level has cube sections that make up a large portion of its difficulty."),
            Skillset("gj_shipBtn_on_001.png", "Ship", "This level has ship sections that make up a large portion of its difficulty."),
            Skillset("gj_ballBtn_on_001.png", "Ball", "This level has ball sections that make up a large portion of its difficulty."),
            Skillset("gj_birdBtn_on_001.png", "UFO", "This level has UFO sections that make up a large portion of its difficulty."),
            Skillset("gj_dartBtn_on_001.png", "Wave", "This level has wave sections that make up a large portion of its difficulty."),
            Skillset("gj_robotBtn_on_001.png", "Robot", "This level has robot sections that make up a large portion of its difficulty."),
            Skillset("gj_spiderBtn_on_001.png", "Spider", "This level has spider sections that make up a large portion of its difficulty."),
            Skillset("player_105_001.png", "Nerve Control", "This level tests your consistency and ability to handle stress near the end of the level."),
            Skillset("player_80_001.png", "Memory", "This level requires remembering a complex path to complete, usually with several fakes, potential routes, and/or visual obscurity."),
            Skillset("player_ball_72_001.png", "Learny", "This level needs a significant time investment in order to understand its complex/unintuitive gameplay."),
            Skillset("portal_12_front_001.png", "Duals", "This level has duals that make up a large portion of its difficulty. Generally refers to asymmetrical duals.", 0.4f),
            Skillset("player_180_001.png", "Chokepoints", "This level contains parts with very condensed difficulty in relation to the rest of the level."),
            Skillset("player_113_001.png", "High CPS", "This level has several sections that require very fast (usually controlled) inputs."),
            Skillset("d_time01_001.png", "Timings", "This level tests your ability to perform many very precise inputs."),
            Skillset("d_animWave_01_001.png", "Flow", "This level has many dynamic gameplay transitions throughout the level, forming a \"smooth\" and \"flowy\" type of gameplay."),
            Skillset("d_circle_01_001.png", "Overall", "This level has no specific skillset it tests, instead drawing on multiple skillsets in smaller proportion for its difficulty.", 0.6f),
            Skillset("player_ball_106_001.png", "Gimmicky", "This level primarily focuses on developing an experimental, unorthodox gameplay type."),
            Skillset("boost_04_001.png", "Fast-Paced", "This level has fast-moving sections (3x or 4x speed) for the majority of the level.", 0.5f),
            Skillset("boost_01_001.png", "Slow-Paced", "This level has slower-moving sections (0.5x) for a large part of the level.", 0.5f),
            Skillset("gj_swingBtn_on_001.png", "Swing", "This level has swing sections that make up a large portion of its difficulty.")
    };

    Skillsets() = default;

    explicit Skillsets(const matjson::Value& tagsJson);

    std::vector<int> getSkillsets() const;
};



#endif //SKILLSETS_H
