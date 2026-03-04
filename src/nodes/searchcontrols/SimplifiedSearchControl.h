#ifndef GDDLINTEGRATION_SIMPLIFIEDSEARCHCONTROL_H
#define GDDLINTEGRATION_SIMPLIFIEDSEARCHCONTROL_H

#include <objects/SearchObject.h>

#include "CheckboxInputControl.h"

class SimplifiedSearchControl : public CheckboxInputControl {
    // abomination
    static constexpr float spacing = 35.0f;
    static constexpr int rows = 5;
    static constexpr int columns = Values::highestTier % rows == 0 ? Values::highestTier / rows : Values::highestTier / rows + 1;
    static constexpr CCPoint tiersGridPosition = {95.0f, 190.0f};
    SearchObject searchObject;

    bool init(SearchObject* searchObject);
    CCMenuItemSpriteExtra* createTierNode(const int tier);
    void onTierSearch(CCObject* sender);
public:
    static SimplifiedSearchControl* create(SearchObject* searchObject);
};


#endif //GDDLINTEGRATION_SIMPLIFIEDSEARCHCONTROL_H
