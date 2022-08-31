cmake_minimum_required(VERSION 3.0.2)
# project(The Cam_to_Jpeg package)
project(cam_to_jpeg)
set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-std=c++14)

find_package(OpenCV  REQUIRED)
find_package(Boost REQUIRED COMPONENTS system)

find_package(catkin REQUIRED COMPONENTS
  cv_bridge
  # image_transport
  roscpp
  sensor_msgs
  std_msgs
  tf
)

catkin_package(
 INCLUDE_DIRS include
 LIBRARIES The cam_to_jpeg
 CATKIN_DEPENDS cv_bridge roscpp sensor_msgs std_msgs tf
#  DEPENDS system_lib
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(cam_jpeg src/cam_jpeg.cpp
)

target_link_libraries(cam_jpeg
  ${catkin_LIBRARIES}
  ${OpenCV_LIBS}
)

# add_dependencies(cam_jpeg ${${PROJECT_NAME}_EXPORTED_TARGETS})

