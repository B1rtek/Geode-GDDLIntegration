#ifndef GDDLINTEGRATION_VALUES_H
#define GDDLINTEGRATION_VALUES_H

class Values {
public:
    // https://gdladder.com/references
    static constexpr int highestTier = 39;
    static constexpr int easyDemonStartTier = 1;
    static constexpr int mediumDemonStartTier = 6;
    static constexpr int hardDemonStartTier = 11;
    static constexpr int insaneDemonStartTier = 16;
    static constexpr int extremeDemonStartTier = 21;
    // https://gdladder.com/about#guidelines
    static constexpr int proofTier = 25;
    static constexpr int clicksTier = 31;
    // api endpoints used more than in one class
    const inline static std::string packIconsBaseUrl = "https://gdladder.com/packIcons/";
};

#endif //GDDLINTEGRATION_VALUES_H