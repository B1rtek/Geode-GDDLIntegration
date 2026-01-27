#ifndef IRATINGOBSERVER_H
#define IRATINGOBSERVER_H

class IRatingObserver {
public:
    virtual ~IRatingObserver() = default;
    virtual void updateRating() = 0;
};

#endif //IRATINGOBSERVER_H
