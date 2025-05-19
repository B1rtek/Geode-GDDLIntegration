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
    // thank you Hubercioch for the icons!
    const static inline std::vector<Skillset> skillsetsList = {
            Skillset("skillset_cube.png", "Oops", "If you're seeing this, it's a sign that something went wrong."), // just to align them with the IDs that start with 1
            Skillset("skillset_cube.png", "Cube", "This level has cube sections that make up a large portion of its difficulty."),
            Skillset("skillset_ship.png", "Ship", "This level has ship sections that make up a large portion of its difficulty."),
            Skillset("skillset_ball.png", "Ball", "This level has ball sections that make up a large portion of its difficulty."),
            Skillset("skillset_ufo.png", "UFO", "This level has UFO sections that make up a large portion of its difficulty."),
            Skillset("skillset_wave.png", "Wave", "This level has wave sections that make up a large portion of its difficulty."),
            Skillset("skillset_robot.png", "Robot", "This level has robot sections that make up a large portion of its difficulty."),
            Skillset("skillset_spider.png", "Spider", "This level has spider sections that make up a large portion of its difficulty."),
            Skillset("skillset_nerve_control.png", "Nerve Control", "This level tests your consistency and ability to handle stress near the end of the level."),
            Skillset("skillset_memory.png", "Memory", "This level requires remembering a complex path to complete, usually with several fakes, potential routes, and/or visual obscurity."),
            Skillset("skillset_learny.png", "Learny", "This level needs a significant time investment in order to understand its complex/unintuitive gameplay."),
            Skillset("skillset_duals.png", "Duals", "This level has duals that make up a large portion of its difficulty. Generally refers to asymmetrical duals."),
            Skillset("skillset_chokepoints.png", "Chokepoints", "This level contains parts with very condensed difficulty in relation to the rest of the level."),
            Skillset("skillset_high_cps.png", "High CPS", "This level has several sections that require very fast (usually controlled) inputs."),
            Skillset("skillset_timings.png", "Timings", "This level tests your ability to perform many very precise inputs."),
            Skillset("skillset_flow.png", "Flow", "This level has many dynamic gameplay transitions throughout the level, forming a \"smooth\" and \"flowy\" type of gameplay."),
            Skillset("skillset_overall.png", "Overall", "This level has no specific skillset it tests, instead drawing on multiple skillsets in smaller proportion for its difficulty."),
            Skillset("skillset_gimmicky.png", "Gimmicky", "This level primarily focuses on developing an experimental, unorthodox gameplay type."),
            Skillset("skillset_fast_paced.png", "Fast-Paced", "This level has fast-moving sections (3x or 4x speed) for the majority of the level."),
            Skillset("skillset_slow_paced.png", "Slow-Paced", "This level has slower-moving sections (0.5x) for a large part of the level."),
            Skillset("skillset_swing.png", "Swing", "This level has swing sections that make up a large portion of its difficulty.")
    };

    Skillsets() = default;

    explicit Skillsets(const matjson::Value& tagsJson);

    std::vector<int> getSkillsets() const;
};



#endif //SKILLSETS_H
