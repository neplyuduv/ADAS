# Source Code
file(GLOB_RECURSE SOURCES *.cpp *.h)
add_executable(ADAS ${SOURCES})

# OpenCV
set(OpenCV_DIR "/home/k/Opensource/installation/OpenCV-master/lib/cmake/opencv4")
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
# Thread
find_package(Threads REQUIRED)
target_link_libraries(ADAS
        ${OpenCV_LIBS}
        ${Boost_LIBRARIES}
        Threads::Threads)

# 디버깅을 위한 컴파일러 옵션 추가
if (CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_definitions(ADAS PRIVATE DEBUG)
endif ()