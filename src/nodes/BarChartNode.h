#ifndef GDDLINTEGRATION_BARCHARTNODE_H
#define GDDLINTEGRATION_BARCHARTNODE_H

#include <Geode/Geode.hpp>

using namespace geode::prelude;

struct BarChartData {
    const _ccColor4F color;
    const std::string label;
    const int value;
};

struct BarChartBar {
    const _ccColor4F color;
    const float height;

    BarChartBar(const _ccColor4F &color, const float height) : color(color), height(height) {}
};

class BarChartNode final : public CCNode {
    std::vector<BarChartBar> processedData{};
    float barHeight = 20.0f;
    float labelWidth = 30.0f;

    bool init(std::vector<BarChartData> data, const CCSize &size, const float labelWidth, const float barHeight);

    BarChartBar calculateBar(const BarChartData &barData, const int index, const int sum, const float chartSize, const int maxValue);

public:
    static BarChartNode *create(const std::vector<BarChartData> &data, const CCSize &size, const float labelWidth, const float barHeight);

    void draw() override;
};


#endif //GDDLINTEGRATION_BARCHARTNODE_H
