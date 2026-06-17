#ifndef IRATINGOBSERVER_H
#define IRATINGOBSERVER_H

class IApiResponseObserver {
public:
    virtual ~IApiResponseObserver() = default;
    virtual void updateData() = 0;
};

#endif //IRATINGOBSERVER_H
