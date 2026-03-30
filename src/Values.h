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

    static const inline std::vector<int> tierColors = {
        0x360432,
        0xdddfee,
        0xd5d3e9,
        0xd3cbe7,
        0xd3c3e4,
        0xd4bbe2,
        0xd5b0de,
        0xdba7dc,
        0xdb9fd1,
        0xd991c1,
        0xda86b0,
        0xdb7698,
        0xdc6a7d,
        0xdd5a5a,
        0xdc514c,
        0xda493e,
        0xda4534,
        0xd93e26,
        0xdc3f23,
        0xc03a1b,
        0xb23415,
        0xa23010,
        0x9b2b0c,
        0x932b0b,
        0x892608,
        0x832607,
        0x752105,
        0x6c1e04,
        0x601a02,
        0x5a1802,
        0x521700,
        0x4f0a01,
        0x4a0207,
        0x470313,
        0x42041e,
        0x400427,
        0x3d0630,
        0x380635,
        0x300636,
        0x250630,
        0x1C072E,
        0x000000
    };

    /*
     * HSV values for tiers 30-40 after the reform
    17 100 32
    7 99 31
    356 97 29
    346 96 28
    335 94 26
    325 93 25
    314 91 24
    304 90 22
    293 88 21
    283 87 19
    272 85 18
     */
};

#endif //GDDLINTEGRATION_VALUES_H