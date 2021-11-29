cmake_minimum_required(VERSION 2.8.3)
project(impedance_module)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  geometry_msgs
  ft_sensor_msgs
)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES impedance_module
  CATKIN_DEPENDS roscpp std_msgs geometry_msgs ft_sensor_msgs
  DEPENDS system_lib
)

set(OS "linux")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

include_directories(
  include/${PROJECT_NAME}
  ${catkin_INCLUDE_DIRS}
)

add_executable(impedance_module_node src/impedance_module_node.cpp)
add_dependencies(impedance_module_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(impedance_module_node ${catkin_LIBRARIES}) 

