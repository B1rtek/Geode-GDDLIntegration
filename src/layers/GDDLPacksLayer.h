#ifndef GDDLINTEGRATION_GDDLPACKSLAYER_H
#define GDDLINTEGRATION_GDDLPACKSLAYER_H

#include <Geode/Bindings.hpp>
#include <Geode/ui/ScrollLayer.hpp>
#include <Geode/utils/web.hpp>
#include <objects/PackInfo.h>

using namespace geode::prelude;

class GDDLPacksLayer : public CCLayer {
    static constexpr CCPoint listSize = {356.0f, 220.0f};
    static inline const std::string packsRequestApiUrl = "https://gdladder.com/api/packs";

    ScrollLayer* packsList = nullptr;
    CCLabelBMFont* titleLabel = nullptr;
    int page = 1, highestPage = 1;
    std::map<int, std::vector<PackInfo>> packInfos;
    TaskHolder<web::WebResponse> packsTaskHolder;

    bool init() override;

    void createListFrame();
    void updateList();
    std::function<void(web::WebResponse)> getPacksDownloadLambda();

    void onNextPage(CCObject* sender);
    void onPrevPage(CCObject* sender);
    void onBack(CCObject* sender);
    void keyBackClicked() override;
    void backActions();
public:

    static GDDLPacksLayer* create();
    static GDDLPacksLayer* scene();
};


#endif //GDDLINTEGRATION_GDDLPACKSLAYER_H