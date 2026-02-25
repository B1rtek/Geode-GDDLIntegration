#ifndef GDDLINTEGRATION_ISETTING_H
#define GDDLINTEGRATION_ISETTING_H

class ISetting {
public:
    virtual ~ISetting() = default;
    virtual void saveSetting() = 0;
    virtual void loadSetting() = 0;
    virtual void resetToDefault() = 0;
    virtual std::string getSearchQueryFragment() = 0;
};

#endif //GDDLINTEGRATION_ISETTING_H