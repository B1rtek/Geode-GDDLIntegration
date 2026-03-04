#ifndef GDDLINTEGRATION_ILOADINGCIRCLEHAVER_H
#define GDDLINTEGRATION_ILOADINGCIRCLEHAVER_H

class ILoadingCircleHaver {
public:
    virtual ~ILoadingCircleHaver() = default;
    virtual void showLoadingCircle() = 0;
    virtual void hideLoadingCircle() = 0;
};

#endif //GDDLINTEGRATION_ILOADINGCIRCLEHAVER_H