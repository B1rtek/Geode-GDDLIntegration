include_directories(simpletest)
add_subdirectory(simpletest)
add_library(GeodeTest GeodeTest.cpp GeodeTest.h)
target_link_libraries(GeodeTest geode-sdk SimpleTest)