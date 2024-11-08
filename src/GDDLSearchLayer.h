#ifndef GDDLSEARCHLAYER_H
#define GDDLSEARCHLAYER_H

#include <Geode/Bindings.hpp>
#include <Geode/utils/web.hpp>
#include "GDDLDemonSplitLayer.h"

struct GDDLBrowserLayer;
using namespace geode::prelude;

enum LevelCompleteness { ANY, UNCOMPLETED, COMPLETED };

class GDDLSearchLayer final : public FLAlertLayer {
    // endpoint args, ignoring chunk (always set to 10) and stddev args
    // value limits
    static constexpr int highestTier = 35;
    static constexpr float highestEnjoyment = 10.0f;
    static constexpr int maxSubmissions = 9999;
    // 0 defaults treated as null (not passed to the query) (well, it depends)
    // page - parse as is, check for valid numbers of course
    inline static int page = 1;
    // name - use a url parser
    inline static std::string name;
    // lowTier, highTier - parse as is, check for valid numbers
    inline static int lowTier = 0, highTier = 0;
    // difficulty - passed as a number from 2 to 6, shown as strings from the vector below, 5 = any
    // 1 is actually reserved for "Official" demons, but since they can't be displayed in a level browser, we can use it
    // 0 is for nothing - just add 1 (API 1.9.0 change) to the index and we're good
    inline static int difficulty = 5;
    const inline static std::vector<std::string> demonDifficulties = {
            "Easy", "Medium", "Hard", "Insane", "Extreme", "Any"
    };
    // creator - use a url parser
    inline static std::string creator;
    // song - use a url parser
    inline static std::string song;
    // bool switches
    inline static bool exactName = false, removeUnrated = false, removeUnratedEnj = false, removeRated = false,
                       removeRatedEnj = false;
    // submission count limits
    inline static int subLowCount = 0, subHighCount = 0;
    // enjoyment submission count limits
    inline static int enjLowCount = 0, enjHighCount = 0;
    // enjoyment rating limits
    inline static float enjLow = 0, enjHigh = highestEnjoyment;
    // sort - displayed as sortBy, mapped to a value in the sort vector, default - ID
    inline static int sortOptionIndex = 0;
    const inline static std::vector<std::string> sortBy = {
            "ID", "Name", "Rating", "Enjoyment", "Rating Count", "Enjoyment Count", "Random"};
    const inline static std::vector<std::string> sort = {"ID",          "Name",           "Rating", "Enjoyment",
                                                         "RatingCount", "EnjoymentCount", "Random"};
    // sortDirection - same as sort basically, default - asc
    inline static int sortDirectionIndex = 0;
    const inline static std::vector<std::string> orderDirection = {"Ascending", "Descending"};
    const inline static std::vector<std::string> sortDirection = {"asc", "desc"};
    // LevelCompleteness - represented as two checkboxes for completed and uncompleted
    inline static LevelCompleteness completeness = ANY;
    inline static bool completed = true, uncompleted = true;
    // have fun remembering all of that lmao
    inline static bool firstLoad = true, simplified = false;
    bool normalLoaded = false, simplifiedLoaded = false; // so they reset with each reopen
    // additional settings so the gddl demon split doesn't change the values
    inline static std::string savedName, savedCreator, savedSong;
    inline static int savedLowTier = -1, savedHighTier = 0, savedDifficulty = 5, savedSubLowCount = 0, savedSubHighCount = 0, savedEnjLowCount = 0, savedEnjHighCount = 0, savedSortOptionIndex = 0, savedSortDirectionIndex = 0;
    inline static float savedEnjLow = 0.0f, savedEnjHigh = highestEnjoyment;
    inline static bool savedCompleted = true, savedUncompleted = false, savedExactName = false, savedRemoveUnrated = false, savedRemoveRated = false, savedRemoveUnratedEnj = false, savedRemoveRatedEnj = false;
    // assigned when gddl demon split requests a search, restored when the main layer is loaded
    // -1 on savedLowTier means that there's nothing to restore

    const inline static std::string searchEndpoint = "https://gdladder.com/api/level/search";
    inline static int totalOnlineResults = 0;
    inline static std::vector<int> cachedResults = {};
    inline static int onlinePagesFetched = 0;
    inline static bool searching = false;
    inline static EventListener<web::WebTask> searchListener;
    inline static int requestRequestedPage; // for the sole purpose of using it inside of the request lambda
    inline static GDDLBrowserLayer* searchCallbackObject;
    inline static GDDLDemonSplitLayer* demonSplitLayer = nullptr;

    // some of the controls should probably be here so searching with getChildByIDRecursive() isn't needed
    // page normal
    CCMenu *normalMenu = nullptr;
    CCMenuItemSpriteExtra *searchButton = nullptr;
    CCMenuItemSpriteExtra *resetButton = nullptr;
    CCTextInputNode *nameTextfield = nullptr;
    CCMenuItemToggler *nameExactMatchToggler = nullptr;
    CCTextInputNode *creatorTextfield = nullptr;
    CCTextInputNode *songTextfield = nullptr;
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
    CCMenuItemToggler *completedToggler = nullptr;
    CCMenuItemToggler *uncompletedToggler = nullptr;
    CCLabelBMFont *sortByLabel = nullptr;
    CCLabelBMFont *sortDirectionLabel = nullptr;
    // page simplified
    CCMenu *simplifiedMenu = nullptr;
    std::vector<CCMenuItemSpriteExtra*> tierButtons;
    CCMenuItemToggler *completedTogglerSimple = nullptr;
    CCMenuItemToggler *uncompletedTogglerSimple = nullptr;
    CCLabelBMFont *completedLabelSimple = nullptr;
    CCLabelBMFont *uncompletedLabelSimple = nullptr;

    const CCPoint popupSize = {440.0f, 290.0f};

    bool init() override;
    void loadPageFull();
    void loadPageSimple();
    void showPage();
    void loadValues();
    void saveValues();
    void resetValues();
    static void cacheValues();
    static void restoreValues();
    void onClose(CCObject *sender);
    void keyBackClicked() override;
    void onInfo(CCObject *sender);
    // request related
    static std::string urlEncodeString(std::string toEncode);
    static std::string addStringToRequest(const std::string &paramName, const std::string &value);
    static std::string addBoolToRequest(const std::string &paramName, bool value);
    template<typename T>
    static std::string addValueToRequest(const std::string &paramName, T value, T defaultValue);
    static std::string formSearchRequest();
    static std::vector<int> parseResponse(const std::string &response);
    static std::vector<int> filterResults(std::vector<int> ids, LevelCompleteness completionStatus);
    static int getMaxPotentialPages();
    static int getOnlinePagesCount();
    static GJSearchObject *makeASearchObjectFrom(int firstIndex, int lastIndex);
    static void appendFetchedResults(const std::string &response);
    static std::pair<int, int> getReadyRange(int requestedPage);
    static void handleSearchObject(GJSearchObject *searchObject, GDDLBrowserLayer *callbackObject, int resultsCount);
    static void prepareSearchListener();
    // utility (that and Utils.h)
    static float calculateNewFloat(float currentValue, bool increase, float lowerbound, float upperbound);
    CCMenuItemSpriteExtra *createTierNode(int tier);
    CCMenu *createCheckboxNode(const std::string &idSuffix, const std::string &name, CCMenuItemToggler *&toggler, SEL_MenuHandler callback);
    // callbacks for all buttons that will be needed
    void onToggleExactMatch(CCObject *sender);
    void onInGameRatingLeft(CCObject *sender);
    void onInGameRatingRight(CCObject *sender);
    void onTierLowLeft(CCObject *sender);
    void onTierLowRight(CCObject *sender);
    void onTierHighLeft(CCObject *sender);
    void onTierHighRight(CCObject *sender);
    void onEnjoymentLowLeft(CCObject *sender);
    void onEnjoymentLowRight(CCObject *sender);
    void onEnjoymentHighLeft(CCObject *sender);
    void onEnjoymentHighRight(CCObject *sender);
    void onSubmissionCountLowLeft(CCObject *sender);
    void onSubmissionCountLowRight(CCObject *sender);
    void onSubmissionCountHighLeft(CCObject *sender);
    void onSubmissionCountHighRight(CCObject *sender);
    void onEnjSubmissionCountLowLeft(CCObject *sender);
    void onEnjSubmissionCountLowRight(CCObject *sender);
    void onEnjSubmissionCountHighLeft(CCObject *sender);
    void onEnjSubmissionCountHighRight(CCObject *sender);
    void onToggleNoUnrated(CCObject *sender);
    void onToggleNoRated(CCObject *sender);
    void onToggleNoUnratedEnj(CCObject *sender);
    void onToggleNoRatedEnj(CCObject *sender);
    void onToggleCompleted(CCObject *sender);
    void onToggleUncompleted(CCObject *sender);
    void onSortByLeft(CCObject *sender);
    void onSortByRight(CCObject *sender);
    void onSortDirectionLeft(CCObject *sender);
    void onSortDirectionRight(CCObject *sender);
    void onSearchClicked(CCObject *sender);
    void onResetClicked(CCObject *sender);
    void onSwapLayout(CCObject *sender);
    // page simplified
    void onTierSearch(CCObject *sender);
    // setters so I don't have to repeat that spaghetti again
    static void setNumberFloatTextfield(float value, CCTextInputNode *&textfield);
    void setDifficultyLabel();
    void setSortByLabel();
    void setSortDirectionLabel();
    // getters for the same thing
    static float getFloatTextfieldValue(CCTextInputNode *&textfield, float defaultValue);
    // bug fixes
    void onEnter() override;
    void clickOffTextfields();

public:
    void onExit() override;

public:
    static GDDLSearchLayer *create();
    void show() override;
    static void loadSettings(); // called on game startup
    static void saveSettings(); // called in menulayer after every modification of the search values
    static void requestSearchPage(int requestedPage, GDDLBrowserLayer *callbackObject);
    static void requestSearchFromDemonSplit(int tier, GDDLDemonSplitLayer* layer);
    static int getSearchResultsPageCount();
    static int getSearchResultsCount();
    static GJSearchObject *getSearchObjectForPage(int requestedPage);
    static bool isSearching();
    static void stopSearch();
    static void restoreValuesAfterSplit();
};

template<typename T>
std::string GDDLSearchLayer::addValueToRequest(const std::string &paramName, T value, T defaultValue) {
    if (value == defaultValue) {
        return "";
    }
    return "&" + paramName + "=" + std::to_string(value);
}


#endif // GDDLSEARCHLAYER_H
