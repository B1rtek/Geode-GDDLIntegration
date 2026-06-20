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
    inline static TaskHolder<web::WebResponse> packsTaskHolder;
public:
    const inline static std::string packLevelsDownloadApiUrlBase = "https://gdladder.com/api/packs/";
    static inline const std::string packsRequestApiUrl = "https://gdladder.com/api/packs";
    inline static bool readCache = false;

    static void populateFromSave();
    static void dumpToSave();
    static Result<std::shared_ptr<PackInfo>> getOrRequestPackInfo(int packID, bool withLevels);
    static Result<PackCategoryInfo> getOrRequestPackCategoryInfo(int categoryID);
    static void requestPackListRefresh();
    static int getCategoryCount();
    static std::vector<std::shared_ptr<PackInfo>> getPacksFromCategory(int categoryID);
    static std::function<void(web::WebResponse)> getPacksDownloadLambda(bool refresh);
    static std::function<void(web::WebResponse)> getPackLevelsDownloadLambda(int packID);
    static std::string getPackLevelsDownloadUrl(int packID);


    static Result<std::shared_ptr<PackInfo>> getPackInfoFromJson(const matjson::Value& json);
    static Result<PackCategoryInfo> getPackCategoryInfoFromJson(const matjson::Value& json);
    static Result<std::shared_ptr<PackInfo>> getPackLevelsFromJson(const matjson::Value& json, int packID);

    static void subscribeToObservers(IApiResponseObserver* newSubscriber);
    static void unsubscribeFromObservers(IApiResponseObserver* unsubscribing);
    static void notifyObservers();
};


#endif //GDDLINTEGRATION_PACKSMANAGER_H