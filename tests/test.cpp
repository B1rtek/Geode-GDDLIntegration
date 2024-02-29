#include <GeodeTest/GeodeTest.h>
#include "../src/RatingsManager.h"

DEFINE_TEST_G(TestName, GDDLTest) {
    TEST_EQ(1, 2-1);
    TEST_EQ(6, 9-3);
    TEST_STR_EQ("btg", "btg");
}

DEFINE_TEST_G(GDTest, GDDLTest) {
    auto color = RatingsManager::getTierColor(-1);
    TEST_EQ(color.r, 255);
}

DEFINE_TEST_G(FailingTest, GDDLTest) {
    TEST_EQ(1, 0);
}