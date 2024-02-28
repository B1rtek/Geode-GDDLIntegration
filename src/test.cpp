#include <GeodeTest/GeodeTest.h>
#include "RatingsManager.h"

DEFINE_TEST_G(GDDLTest, TestName) {
    TEST_EQ(1, 2-1);
}

DEFINE_TEST_G(GDDLTest, GDTest) {
    auto color = RatingsManager::getTierColor(-1);
    TEST_EQ(color.r, 255);
}