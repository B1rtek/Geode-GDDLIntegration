#ifndef GDDLSEARCHLAYER_H
#define GDDLSEARCHLAYER_H

#include <Geode/Bindings.hpp>

using namespace geode::prelude;

class GDDLSearchLayer : public FLAlertLayer {
    // endpoint args, ignoring chunk (always set to 10) and stddev args
    // -1 defaults treated as null (not passed to the query)
    // page - parse as is, check for valid numbers of course
    inline static int page = 1;
    // name - use a url parser
    inline static std::string name;
    // lowTier, highTier - parse as is, check for valid numbers
    inline static int lowTier = -1, highTier = -1;
    // difficulty - passed as a number from 0 to 4, shown as strings from the vector below
    inline static int difficulty = -1;
    const inline static std::vector<std::string> demonDifficulties = {"Easy", "Medium", "Hard", "Insane", "Extreme"};
    // creator - use a url parser
    inline static std::string creator;
    // song - use a url parser
    inline static std::string song;
    // bool switches
    inline static bool exactName = false, removeUnrated = false, removeUnratedEnj = false, removeRated = false, removeRatedEnj = false;
    // submission count limits
    inline static int subLowCount = -1, subHighCount = -1;
    // enjoyment submission count limits
    inline static int enjLowCount = -1, enjHighCount = -1;
    // enjoyment rating limits
    inline static int enjLow = -1, enjHigh = -1;
    // sort - displayed as sortBy, mapped to a value in the sort vector, default - ID
    inline static int sortOptionIndex = 0;
    const inline static std::vector<std::string> sortBy = {"ID", "Name", "Rating", "Enjoyment", "Rating Count", "Enjoyment Count", "Random"};
    const inline static std::vector<std::string> sort = {"ID", "Name", "Rating", "Enjoyment", "RatingCount", "EnjoymentCount", "Random"};
    // sortDirection - same as sort basically, default - asc
    inline static int sortDirectionIndex = 0;
    const inline static std::vector<std::string> orderDirection = {"Ascending", "Descending"};
    const inline static std::vector<std::string> sortDirection = {"asc", "desc"};
    // have fun remembering all of that lmao

    // some of the controls should probably be here so searching with getChildByIDRecursive() isn't needed
    CCTextInputNode *nameTextfield = nullptr;
    CCMenuItemToggler *nameExactMatchToggler = nullptr;
    CCTextInputNode *creatorTextfield = nullptr;
    CCTextInputNode *songTextField = nullptr;
    CCLabelBMFont *difficultyLabel = nullptr;
    CCTextInputNode *tierLowTextfield = nullptr;
    CCTextInputNode *tierHighTextfield = nullptr;
    CCTextInputNode *enjoymentLowTextfield = nullptr;
    CCTextInputNode *enjoymentHighTextfield = nullptr;
    CCTextInputNode *submissionsCountHighTextfield = nullptr;
    CCTextInputNode *submissionsCountLowTextfield = nullptr;
    CCTextInputNode *enjSubmissionsCountHighTextfield = nullptr;
    CCTextInputNode *enjSubmissionsCountLowTextfield = nullptr;
    CCMenuItemToggler *noUnratedToggler = nullptr;
    CCMenuItemToggler *noRatedToggler = nullptr;
    CCMenuItemToggler *noUnratedEnjToggler = nullptr;
    CCMenuItemToggler *noRatedEnjToggler = nullptr;
    CCLabelBMFont *sortByLabel = nullptr;
    CCLabelBMFont *sortDirectionLabel = nullptr;

    const CCPoint popupSize = {440.0f, 290.0f};

    bool init() override;
    void loadPage();
    void onClose(CCObject *sender);
    void onInfo(CCObject *sender);
    void onSearchOptionSelected(CCObject *sender); // will be removed
    void onTierSearch(CCObject *sender); // relink to a search button
    CCMenuItemSpriteExtra *createTierNode(int tier);
    CCMenu *createCheckboxNode(const std::string &idSuffix, const std::string &name);
    // callbacks for all buttons that will be needed
    void onToggleExactMatch(CCObject* sender);
    void onInGameRatingLeft(CCObject* sender);
    void onInGameRatingRight(CCObject* sender);
    void onTierLowLeft(CCObject* sender);
    void onTierLowRight(CCObject* sender);
    void onTierHighLeft(CCObject* sender);
    void onTierHighRight(CCObject* sender);
    void onEnjoymentLowLeft(CCObject* sender);
    void onEnjoymentLowRight(CCObject* sender);
    void onEnjoymentHighLeft(CCObject* sender);
    void onEnjoymentHighRight(CCObject* sender);
    void onSubmissionCountLowLeft(CCObject* sender);
    void onSubmissionCountLowRight(CCObject* sender);
    void onSubmissionCountHighLeft(CCObject* sender);
    void onSubmissionCountHighRight(CCObject* sender);
    void onEnjSubmissionCountLowLeft(CCObject* sender);
    void onEnjSubmissionCountLowRight(CCObject* sender);
    void onEnjSubmissionCountHighLeft(CCObject* sender);
    void onEnjSubmissionCountHighRight(CCObject* sender);
    void onToggleNoUnrated(CCObject* sender);
    void onToggleNoRated(CCObject* sender);
    void onToggleNoUnratedEnj(CCObject* sender);
    void onToggleNoRatedEnj(CCObject* sender);
    void onSortByLeft(CCObject* sender);
    void onSortByRight(CCObject* sender);
    void onSortDirectionLeft(CCObject* sender);
    void onSortDirectionRight(CCObject* sender);

public:
    static GDDLSearchLayer *create();
    void show() override;
    static void loadSettings();
    static void saveSettings();
};


#endif // GDDLSEARCHLAYER_H
