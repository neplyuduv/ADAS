# Source Code
file(GLOB_RECURSE SOURCES *.cpp *.h)
add_executable(ADAS ${SOURCES})

# OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
# Boost
find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
# Thread
find_package(Threads REQUIRED)
# RxCpp
#include_directories(${rxcpp_INCLUDE_DIRS})
target_link_libraries(ADAS
        ${OpenCV_LIBS}
        ${Boost_LIBRARIES}
        Threads::Threads)