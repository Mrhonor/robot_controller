cmake_minimum_required(VERSION 3.0.2)
project(robot_controller)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  roscpp
  std_msgs
  message_generation
)

add_message_files(
    FILES
    aruco_msg.msg
)

generate_messages(
    DEPENDENCIES
    std_msgs
    geometry_msgs
)


set(CMAKE_CXX_FLAGS "-std=c++17 ${CMAKE_CXX_FLAGS}")

catkin_package(
  INCLUDE_DIRS
    include
  CATKIN_DEPENDS
    roscpp
    message_runtime
  DEPENDS
    ${LIBS}
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(robot_controller
    src/controller_main.cpp
    src/controller.cpp
    src/controller_subscriber.cpp
    src/controller_publisher.cpp
)
add_dependencies(robot_controller ${${PROJECT_NAME}_EXPORTTED_TARGETS}
                                   ${catkin_EXPORTED_TARGETS}
                                   ${PROJECT_NAME}_generate_messages_cpp
                                   ${PROJECT_NAME}_gencpp
                                   )

target_link_libraries(robot_controller 
    ${catkin_LIBRARIES}
)

