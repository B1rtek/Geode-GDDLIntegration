#ifndef GDDLINTEGRATION_PACKSMANAGER_H
#define GDDLINTEGRATION_PACKSMANAGER_H

#include <Geode/Geode.hpp>
#include <objects/IApiResponseObserver.h>
#include <objects/PackCategoryInfo.h>
#include <objects/PackInfo.h>

using namespace geode::prelude;

class PacksManager {
    inline static std::map<int, std::shared_ptr<PackInfo>> packsMap{};
    inline static std::map<int, PackCategoryInfo> packCategoryMap{};
    inline static int lastRefreshTimestamp = 0;
    inline static std::string packsCachePath = Mod::get()->getSaveDir().string() + "/packscache.json";
    inline static std::set<IApiResponseObserver*> packUpdateObservers{};
public:
    const inline static std::string packDownloadApiUrlBase = "https://gdladder.com/api/packs/";
    static inline const std::string packsRequestApiUrl = "https://gdladder.com/api/packs";
    inline static bool readCache = false;

    static void populateFromSave();
    static void dumpToSave();
    static Result<std::shared_ptr<PackInfo>> getPackInfo(int packID);
    static Result<PackCategoryInfo> getPackCategoryInfo(int categoryID);

    static Result<std::shared_ptr<PackInfo>> getPackInfoFromJson(const matjson::Value& json);
    static Result<PackCategoryInfo> getPackCategoryInfoFromJson(const matjson::Value& json);

    static void subscribeToObservers(IApiResponseObserver* newSubscriber);
    static void unsubscribeFromObservers(IApiResponseObserver* unsubscribing);
};


#endif //GDDLINTEGRATION_PACKSMANAGER_H