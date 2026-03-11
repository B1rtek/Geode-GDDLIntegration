#ifndef GDDLINTEGRATION_PACKINFO_H
#define GDDLINTEGRATION_PACKINFO_H

#include <matjson.hpp>
#include <Geode/binding/GJSearchObject.hpp>
#include <Geode/utils/web.hpp>

using namespace geode::prelude;

struct GDDLPackLevelBrowser; // circular import oops

class PackInfo {
    const inline static std::string packDownloadApiUrlBase = "https://gdladder.com/api/packs/";

    int id{};
    int categoryId{};
    std::string name;
    std::string iconPath;
    int medianTier{};
    std::vector<int> levels;
    TaskHolder<web::WebResponse> packDownloadTaskHolder;

    std::function<void(web::WebResponse)> getPackDownloadLambda();
    static std::string getPackDownloadUrl(int packId);
    void forwardToLevelBrowser(GJSearchObject* gjSearchObject, GDDLPackLevelBrowser* callingLayer, const int actualPageNumber);

public:
    PackInfo() = default;

    PackInfo(const int id, const int categoryId, const std::string& name, const std::string& iconPath, const int medianTier);

    static Result<std::shared_ptr<PackInfo>> createFromJson(const matjson::Value& json);
    void downloadAndOpenPack();
    void requestPage(int pageNumber, GDDLPackLevelBrowser* callingLayer);

    int getId() const;
    int getCategoryId() const;
    std::string getName() const;
    std::string getIconPath() const;
    int getMedianTier() const;
    std::vector<int> getLevels() const;
};


#endif //GDDLINTEGRATION_PACKINFO_H