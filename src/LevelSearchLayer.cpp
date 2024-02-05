#include <Geode/Bindings.hpp>
#include <Geode/modify/LevelSearchLayer.hpp>


#include "RatingsManager.h"

class $modify(GDDLSearchLayer, LevelSearchLayer) {
    bool init(int p0) {
        if(!LevelSearchLayer::init(p0)) return false;
        RatingsManager::stopSearch();
        return true;
    }
};