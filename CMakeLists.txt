cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64") # this mod doesn't have a mac version anyway lol
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(GDDLIntegration VERSION 1.0.0)

# Use GLOB_RECURSE instead of GLOB
# to recursively add all source files 
# under src/ 
file(GLOB SOURCES
	src/*.cpp src/*.h src/*/*.cpp src/*/*.h src/*/*/*.cpp src/*/*/*.h
)

include_directories(src)

# Set up the mod binary
add_library(${PROJECT_NAME} SHARED ${SOURCES})

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

# no yapping
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=return-type")


add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

# dank I love you <3
# code from Globed https://github.com/GlobedGD/globed2/blob/b14a198ea61dabb29d371f18804fe10f7355c937/CMakeLists.txt#L216-L256
CPMAddPackage(
        NAME curl
        GIT_REPOSITORY "https://github.com/curl/curl.git"
        GIT_TAG "curl-8_8_0"
        DOWNLOAD_ONLY YES
)

add_library(curl STATIC IMPORTED)

if (ANDROID)
    if (ANDROID_ABI STREQUAL "arm64-v8a")
        set(LIB_PLATFORM "android64")
    else()
        set(LIB_PLATFORM "android32")
    endif()
elseif (WIN32)
    set(LIB_PLATFORM "win64")
else()
    set(LIB_PLATFORM "macos")
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE CURL_STATICLIB=1)

# we are kinda leeching off of geode but we can always snatch those libraries in the future
if (WIN32)
    set_target_properties(curl PROPERTIES IMPORTED_LOCATION "$ENV{GEODE_SDK}/loader/include/link/${LIB_PLATFORM}/libcurl.lib")
else()
    set_target_properties(curl PROPERTIES IMPORTED_LOCATION "$ENV{GEODE_SDK}/loader/include/link/${LIB_PLATFORM}/libcurl.a")
endif()

target_include_directories(${PROJECT_NAME} PRIVATE "${curl_SOURCE_DIR}/include")

target_link_libraries(${PROJECT_NAME} curl)

# mac needs this for curl
if (APPLE)
    target_link_libraries(${PROJECT_NAME} z)
elseif (WIN32)
    # windows needs this ig?
    target_link_libraries(${PROJECT_NAME} crypt32)
endif()
# end of Globed CMake code
# dank I love you once again <3

# Set up dependencies, resources, link Geode
setup_geode_mod(${PROJECT_NAME} EXTERNALS geode.node-ids:1.13.2)
