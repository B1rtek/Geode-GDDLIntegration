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
    std::set<int> extraLevels;
    TaskHolder<web::WebResponse> packDownloadTaskHolder;

    std::function<void(web::WebResponse)> getPackDownloadLambda();
    static std::string getPackDownloadUrl(int packId);
    void forwardToLevelBrowser(GJSearchObject* gjSearchObject, GDDLPackLevelBrowser* callingLayer, const int actualPageNumber);
    std::pair<int, int> calculateCompletionStats() const;

public:
    PackInfo() = default;

    PackInfo(const int id, const int categoryId, const std::string& name, const std::string& iconPath, const int medianTier);

    static Result<std::shared_ptr<PackInfo>> createFromJson(const matjson::Value& json);
    // these should probably be an interface or sth
    void downloadAndOpenPack();
    void requestPage(int pageNumber, GDDLPackLevelBrowser* callingLayer);
    std::string getPageCountText(const int pageNumber);
    bool shouldShowRightArrow(const int pageNumber);

    std::pair<float, bool> getCompletionStatus() const;
    std::pair<std::pair<int, int>, bool> getCompletedFraction() const;
    int getId() const;
    int getCategoryId() const;
    std::string getName() const;
    std::string getIconPath() const;
    int getMedianTier() const;
    std::vector<int> getLevels() const;
    bool isExtra(const int levelId) const;
};


#endif //GDDLINTEGRATION_PACKINFO_H