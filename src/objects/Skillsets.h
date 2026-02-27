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

public:
    Skillset() = default;

    Skillset(const std::string &icon, const std::string &name, const std::string &description) : icon(
            icon), name(name), description(description) {}

    CCNode * getButtonSprite() const {
        const auto buttonSprite = CCSprite::create(Mod::get()->expandSpriteName(icon.c_str()).data());
        buttonSprite->setScale(0.55f);
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
    const static inline std::vector<std::string> skillsetNames = {
        "Any", "Cube", "Ship", "Ball", "UFO", "Wave", "Robot", "Spider", "Nerve Control", "Memory", "Learny", "Duals", "Chokepoints", "High CPS", "Timings", "Flow", "Overall", "Gimmicky", "Fast-Paced", "Slow-Paced", "Swing"
    };

    // thank you Hubercioch for the icons!
    const static inline std::vector<Skillset> skillsetsList = {
            Skillset("skillset_cube.png", skillsetNames[0], "If you're seeing this, it's a sign that something went wrong."), // just to align them with the IDs that start with 1
            Skillset("skillset_cube.png", skillsetNames[1], "This level has cube sections that make up a large portion of its difficulty."),
            Skillset("skillset_ship.png", skillsetNames[2], "This level has ship sections that make up a large portion of its difficulty."),
            Skillset("skillset_ball.png", skillsetNames[3], "This level has ball sections that make up a large portion of its difficulty."),
            Skillset("skillset_ufo.png", skillsetNames[4], "This level has UFO sections that make up a large portion of its difficulty."),
            Skillset("skillset_wave.png", skillsetNames[5], "This level has wave sections that make up a large portion of its difficulty."),
            Skillset("skillset_robot.png", skillsetNames[6], "This level has robot sections that make up a large portion of its difficulty."),
            Skillset("skillset_spider.png", skillsetNames[7], "This level has spider sections that make up a large portion of its difficulty."),
            Skillset("skillset_nerve_control.png", skillsetNames[8], "This level tests your consistency and ability to handle stress near the end of the level."),
            Skillset("skillset_memory.png", skillsetNames[9], "This level requires remembering a complex path to complete, usually with several fakes, potential routes, and/or visual obscurity."),
            Skillset("skillset_learny.png", skillsetNames[10], "This level needs a significant time investment in order to understand its complex/unintuitive gameplay."),
            Skillset("skillset_duals.png", skillsetNames[11], "This level has duals that make up a large portion of its difficulty. Generally refers to asymmetrical duals."),
            Skillset("skillset_chokepoints.png", skillsetNames[12], "This level contains parts with very condensed difficulty in relation to the rest of the level."),
            Skillset("skillset_high_cps.png", skillsetNames[13], "This level has several sections that require very fast (usually controlled) inputs."),
            Skillset("skillset_timings.png", skillsetNames[14], "This level tests your ability to perform many very precise inputs."),
            Skillset("skillset_flow.png", skillsetNames[15], "This level has many dynamic gameplay transitions throughout the level, forming a \"smooth\" and \"flowy\" type of gameplay."),
            Skillset("skillset_overall.png", skillsetNames[16], "This level has no specific skillset it tests, instead drawing on multiple skillsets in smaller proportion for its difficulty."),
            Skillset("skillset_gimmicky.png", skillsetNames[17], "This level primarily focuses on developing an experimental, unorthodox gameplay type."),
            Skillset("skillset_fast_paced.png", skillsetNames[18], "This level has fast-moving sections (3x or 4x speed) for the majority of the level."),
            Skillset("skillset_slow_paced.png", skillsetNames[19], "This level has slower-moving sections (0.5x) for a large part of the level."),
            Skillset("skillset_swing.png", skillsetNames[20], "This level has swing sections that make up a large portion of its difficulty.")
    };

    Skillsets() = default;

    explicit Skillsets(const matjson::Value& tagsJson);

    std::vector<int> getSkillsets() const;
};



#endif //SKILLSETS_H
