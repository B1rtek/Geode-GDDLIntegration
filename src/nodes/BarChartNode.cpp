#include "BarChartNode.h"

#include "Utils.h"

bool BarChartNode::init(std::vector<BarChartData> data, const CCSize &size, const float labelWidth, const float barHeight) {
    if (!CCNode::init()) return false;

    // set the size
    this->setContentSize(size);
    this->labelWidth = labelWidth;
    this->barHeight = barHeight;

    // compute bars
    int sum = 0, maxValue = 0;
    std::for_each(data.begin(), data.end(), [&sum, &maxValue](BarChartData &barData) {
        sum += barData.value;
        maxValue = std::max(barData.value, maxValue);
    });
    for (int i = 0; i < data.size(); i++) {
        this->processedData.push_back(BarChartNode::calculateBar(data[i], i, sum, size.width, maxValue));
    }

    return true;
}

BarChartBar
BarChartNode::calculateBar(const BarChartData &barData, const int index, const int sum, const float chartSize,
                           const int maxValue) {
    // calculate stuff
    float percent = (float) barData.value / (float) sum;
    float height = (chartSize - labelWidth) * ((float) barData.value / (float) sum) * ((float) sum / (float) maxValue);
    const std::string percentage = std::to_string(barData.value) + " (" + Utils::floatToString(percent * 100, 2) + "%)";
    // create labels
    auto labelLabel = CCLabelBMFont::create(barData.label.c_str(), "chatFont.fnt");
    Utils::scaleLabelToWidth(labelLabel, labelWidth);
    labelLabel->setAnchorPoint({0.0f, 0.5f});
    const float labelPositionAdjustement = labelLabel->getContentWidth() * (CCDirector::get()->getLoadedTextureQuality() == TextureQuality::kTextureQualityHigh ? 0.0f : 0.05f);
    labelLabel->setPosition({0.0f - labelPositionAdjustement, this->getContentHeight() - (barHeight * index) - barHeight / 2});
    if (labelLabel->getScaledContentHeight() > barHeight) {
        labelLabel->setScale(labelLabel->getScale() * (barHeight / labelLabel->getScaledContentHeight()));
    }
    this->addChild(labelLabel);
    auto percentageLabel = CCLabelBMFont::create(percentage.c_str(), "chatFont.fnt");
    Utils::scaleLabelToWidth(percentageLabel, labelWidth * 1.5f);
    percentageLabel->setAnchorPoint({0.0f, 0.5f});
    percentageLabel->setPosition({labelWidth + height + 2.0f, this->getContentHeight() - (barHeight * index) - barHeight / 2});
    if (percentageLabel->getScaledContentHeight() > barHeight) {
        percentageLabel->setScale(percentageLabel->getScale() * (barHeight / percentageLabel->getScaledContentHeight()));
    }
    this->addChild(percentageLabel);
    return {barData.color, height};
}


BarChartNode *BarChartNode::create(const std::vector<BarChartData> &data, const CCSize &size, const float labelWidth, const float barHeight) {
    if (const auto newNode = new BarChartNode(); newNode != nullptr && newNode->init(data, size, labelWidth, barHeight)) {
        newNode->autorelease();
        return newNode;
    } else {
        delete newNode;
        return nullptr;
    }
}

void BarChartNode::draw() {
    CCNode::draw();
    const float padding = Mod::get()->getSettingValue<bool>("thin-chart-bars") ? barHeight * 0.05f : 0.0f;
    for (int i = 0; i < processedData.size(); i++) {
        ccDrawSolidRect({this->labelWidth, this->getContentHeight() - this->barHeight * i - padding},
                        {this->labelWidth + processedData[i].height,
                         this->getContentHeight() - this->barHeight * (i + 1) + padding}, processedData[i].color);
    }
}



