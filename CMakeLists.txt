cmake_minimum_required(VERSION 2.8.3)
project(atc_sim_ros)


find_package(catkin REQUIRED COMPONENTS
  geometry_msgs
  tf2_geometry_msgs
  tf2
  tf2_ros
  roslint
  roscpp
  rospy
  std_msgs
  message_generation
)


include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

#add_library(geometry_tf src/geometry_tf/transforms.cpp)

set(LINK_LIBS
  geometry_tf
  ${catkin_LIBRARIES}
)


## Generate messages in the 'msg' folder
# add_message_files(
#   FILES
#   Message1.msg
#   Message2.msg
# )

## Generate services in the 'srv' folder
 add_service_files(
   FILES
   GenerateFlight.srv
 )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
 generate_messages(
   DEPENDENCIES
   std_msgs
 )

catkin_package(
   INCLUDE_DIRS include
   LIBRARIES geometry_tf
   CATKIN_DEPENDS geometry_msgs tf2_geometry_msgs roscpp tf2 tf2_ros message_runtime
)



include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)



add_executable(flight_gen_server src/flight_gen_server.cpp)
target_link_libraries(flight_gen_server ${catkin_LIBRARIES})
#add_dependencies(add_two_ints_server beginner_tutorials_gencpp)

add_executable(main src/main.cpp)
target_link_libraries(main ${catkin_LIBRARIES})
#add_dependencies(add_two_ints_client beginner_tutorials_gencpp)


