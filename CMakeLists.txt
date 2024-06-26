cmake_minimum_required(VERSION 3.0)
project(samplemqttapp)

# Find necessary packages
find_package(Qt5 COMPONENTS Widgets Sql REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(Qt5Widgets REQUIRED)
pkg_check_modules(MOSQUITTO REQUIRED libmosquitto)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)

# Include directories for header files
include_directories(${Qt5Widgets_INCLUDE_DIRS} ${MOSQUITTO_INCLUDE_DIRS} ${JSONCPP_INCLUDE_DIRS})

# Define executable targets for your applications
add_executable(publisher_1 publisher_1.cpp)
add_executable(publisher_2 publisher_2.cpp)
add_executable(publisher_3 publisher_3.cpp)
add_executable(subscriber1_gui subscriber1_gui.cpp)

# Link the executables with the necessary libraries
target_link_libraries(publisher_1 Qt5::Widgets ${MOSQUITTO_LIBRARIES} ${JSONCPP_LIBRARIES})
target_link_libraries(publisher_2 Qt5::Widgets ${MOSQUITTO_LIBRARIES} ${JSONCPP_LIBRARIES})
target_link_libraries(publisher_3 Qt5::Widgets ${MOSQUITTO_LIBRARIES} ${JSONCPP_LIBRARIES})
target_link_libraries(subscriber1_gui Qt5::Widgets Qt5::Sql ${MOSQUITTO_LIBRARIES} ${JSONCPP_LIBRARIES})
