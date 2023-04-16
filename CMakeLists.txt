cmake_minimum_required(VERSION 3.10.2)
project(gazebo_custom_sensor_preloader)

find_package(catkin REQUIRED COMPONENTS
  gazebo_dev
  pluginlib
)

find_package(Boost REQUIRED COMPONENTS filesystem)

catkin_package(
  INCLUDE_DIRS include
  LIBRARIES ${PROJECT_NAME} custom_sensor_config_plugin
  CATKIN_DEPENDS gazebo_dev pluginlib
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME} src/CustomSensorPreloader.cpp)
add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES} ${Boost_LIBRARIES})

add_library(example_custom_sensor src/ExampleCustomSensor.cpp)
add_dependencies(example_custom_sensor ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(example_custom_sensor ${catkin_LIBRARIES})

add_library(custom_sensor_config_plugin src/CustomSensorConfigPlugin.cpp)
add_dependencies(custom_sensor_config_plugin ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(custom_sensor_config_plugin ${catkin_LIBRARIES})

file(GENERATE
  OUTPUT "${CATKIN_DEVEL_PREFIX}/${CATKIN_PACKAGE_BIN_DESTINATION}/gzserver"
  INPUT "${CMAKE_CURRENT_SOURCE_DIR}/scripts/gzserver.in")

file(GENERATE
  OUTPUT "${CATKIN_DEVEL_PREFIX}/${CATKIN_PACKAGE_BIN_DESTINATION}/gazebo"
  INPUT "${CMAKE_CURRENT_SOURCE_DIR}/scripts/gazebo.in")

install(TARGETS ${PROJECT_NAME} example_custom_sensor custom_sensor_config_plugin
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
)

install(FILES
  "${CATKIN_DEVEL_PREFIX}/${CATKIN_PACKAGE_BIN_DESTINATION}/gzserver"
  "${CATKIN_DEVEL_PREFIX}/${CATKIN_PACKAGE_BIN_DESTINATION}/gazebo"
  DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

install(FILES example_custom_sensor.xml DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION})

install(DIRECTORY include/${PROJECT_NAME}/
  DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
  FILES_MATCHING PATTERN "*.h"
)