#ifndef GDDLROBTOPLEVELSLAYER_H
#define GDDLROBTOPLEVELSLAYER_H

#include <Geode/Bindings.hpp>
#include <Geode/modify/LevelSelectLayer.hpp>
#include "RatingsManager.h"
#include "Utils.h"
#include "layers/GDDLLevelInfoPopup.h"

struct GDDLRobtopLevelsLayer : public geode::Modify<GDDLRobtopLevelsLayer, LevelSelectLayer> {
    enum Level {
        TOE = 11,
        ELECTROMAN_ADVENTURES = 12,
        CLUBSTEP = 13,
        ELECTRODYNAMIX = 14,
        HEXAGON_FORCE = 15,
        BLAST_PROCESSING = 16,
        TOE2 = 17,
        GEOMETRICAL_DOMINATOR = 18,
        DEADLOCKED = 19,
        FINGERDASH = 20,
        DASH = 21,
    };

    struct Fields {
        int currentPage = 0;
        static constexpr int pageCount = 24;
        static inline bool beingBrowsed = false;
        bool buttonsAdded[3] = {false, false, false};
        bool changedBySwiping = false;
        EventListener<web::WebTask> robtopLevelsLayerGetRatingListener;
    };

    bool init(int page);

    void onNext(CCObject *sender);

    void onPrev(CCObject *sender);

    void swiped(const int newPage);

    void onBack(CCObject *sender);

    virtual void keyBackClicked();

    void backActions();

    void pageChanged(int previousPage);

    void removeFrom(int scrollLayerPage);

    void addTo(int scrollLayerPage, int levelID);

    int convertPageToLevel(int page);

    CCNode* getLevelButton(int pageID);

    void onGDDLInfo(CCObject *sender);

    void updateButton(const int tier);
};

#endif //GDDLROBTOPLEVELSLAYER_H
