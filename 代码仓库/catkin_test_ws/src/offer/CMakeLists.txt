cmake_minimum_required(VERSION 3.0.2)
project(offer)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
)

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)


## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/catkin/html/user_guide/setup_dot_py.html
# catkin_python_setup()

################################################
## Declare ROS messages, services and actions ##
################################################

## To declare and build messages, services or actions from within this
## package, follow these steps:
## * Let MSG_DEP_SET be the set of packages whose message types you use in
##   your messages/services/actions (e.g. std_msgs, actionlib_msgs, ...).
## * In the file package.xml:
##   * add a build_depend tag for "message_generation"
##   * add a build_depend and a exec_depend tag for each package in MSG_DEP_SET
##   * If MSG_DEP_SET isn't empty the following dependency has been pulled in
##     but can be declared for certainty nonetheless:
##     * add a exec_depend tag for "message_runtime"
## * In this file (CMakeLists.txt):
##   * add "message_generation" and every package in MSG_DEP_SET to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * add "message_runtime" and every package in MSG_DEP_SET to
##     catkin_package(CATKIN_DEPENDS ...)
##   * uncomment the add_*_files sections below as needed
##     and list every .msg/.srv/.action file to be processed
##   * uncomment the generate_messages entry below
##   * add every package in MSG_DEP_SET to generate_messages(DEPENDENCIES ...)

## Generate messages in the 'msg' folder
# add_message_files(
#   FILES
#   Message1.msg
#   Message2.msg
# )

## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
# generate_messages(
#   DEPENDENCIES
#   std_msgs  # Or other packages containing msgs
# )

################################################
## Declare ROS dynamic reconfigure parameters ##
################################################

## To declare and build dynamic reconfigure parameters within this
## package, follow these steps:
## * In the file package.xml:
##   * add a build_depend and a exec_depend tag for "dynamic_reconfigure"
## * In this file (CMakeLists.txt):
##   * add "dynamic_reconfigure" to
##     find_package(catkin REQUIRED COMPONENTS ...)
##   * uncomment the "generate_dynamic_reconfigure_options" section below
##     and list every .cfg file to be processed

## Generate dynamic reconfigure parameters in the 'cfg' folder
# generate_dynamic_reconfigure_options(
#   cfg/DynReconf1.cfg
#   cfg/DynReconf2.cfg
# )

###################################
## catkin specific configuration ##
###################################
## The catkin_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if your package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## CATKIN_DEPENDS: catkin_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES offer
#  CATKIN_DEPENDS roscpp
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

## Declare a C++ library
# add_library(${PROJECT_NAME}
#   src/${PROJECT_NAME}/offer.cpp
# )

## Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure
# add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Declare a C++ executable
## With catkin_make all packages are built within a single CMake context
## The recommended prefix ensures that target names across packages don't collide
add_executable(s1 src/s1.cpp)
target_link_libraries(s1 ${catkin_LIBRARIES})

add_executable(s3 src/s3.cpp)
target_link_libraries(s3 ${catkin_LIBRARIES})

add_executable(s4 src/s4.cpp)
target_link_libraries(s4 ${catkin_LIBRARIES})

add_executable(s5 src/s5.cpp)
target_link_libraries(s5 ${catkin_LIBRARIES})

add_executable(s6 src/s6.cpp)
target_link_libraries(s6 ${catkin_LIBRARIES})

add_executable(s7 src/s7.cpp)
target_link_libraries(s7 ${catkin_LIBRARIES})

add_executable(s8 src/s8.cpp)
target_link_libraries(s8 ${catkin_LIBRARIES})

add_executable(s9 src/s9.cpp)
target_link_libraries(s9 ${catkin_LIBRARIES})

add_executable(s10 src/s10.cpp)
target_link_libraries(s10 ${catkin_LIBRARIES})

add_executable(s11 src/s11.cpp)
target_link_libraries(s11 ${catkin_LIBRARIES})

add_executable(s12 src/s12.cpp)
target_link_libraries(s12 ${catkin_LIBRARIES})

add_executable(s13 src/s13.cpp)
target_link_libraries(s13 ${catkin_LIBRARIES})

add_executable(s14 src/s14.cpp)
target_link_libraries(s14 ${catkin_LIBRARIES})

add_executable(s15 src/s15.cpp)
target_link_libraries(s15 ${catkin_LIBRARIES})

add_executable(s16 src/s16.cpp)
target_link_libraries(s16 ${catkin_LIBRARIES})

add_executable(s17 src/s17.cpp)
target_link_libraries(s17 ${catkin_LIBRARIES})

add_executable(s18 src/s18.cpp)
target_link_libraries(s18 ${catkin_LIBRARIES})

add_executable(s19 src/s19.cpp)
target_link_libraries(s19 ${catkin_LIBRARIES})

add_executable(s20 src/s20.cpp)
target_link_libraries(s20 ${catkin_LIBRARIES})

add_executable(s21 src/s21.cpp)
target_link_libraries(s21 ${catkin_LIBRARIES})

add_executable(s22 src/s22.cpp)
target_link_libraries(s22 ${catkin_LIBRARIES})

add_executable(s23 src/s23.cpp)
target_link_libraries(s23 ${catkin_LIBRARIES})

add_executable(s24 src/s24.cpp)
target_link_libraries(s24 ${catkin_LIBRARIES})

add_executable(s25 src/s25.cpp)
target_link_libraries(s25 ${catkin_LIBRARIES})

add_executable(s26 src/s26.cpp)
target_link_libraries(s26 ${catkin_LIBRARIES})

add_executable(s27 src/s27.cpp)
target_link_libraries(s27 ${catkin_LIBRARIES})

add_executable(s28 src/s28.cpp)
target_link_libraries(s28 ${catkin_LIBRARIES})

add_executable(s29 src/s29.cpp)
target_link_libraries(s29 ${catkin_LIBRARIES})

add_executable(s30 src/s30.cpp)
target_link_libraries(s30 ${catkin_LIBRARIES})

add_executable(s31 src/s31.cpp)
target_link_libraries(s31 ${catkin_LIBRARIES})

add_executable(s32 src/s32.cpp)
target_link_libraries(s32 ${catkin_LIBRARIES})

add_executable(s33 src/s33.cpp)
target_link_libraries(s33 ${catkin_LIBRARIES})

add_executable(s34 src/s34.cpp)
target_link_libraries(s34 ${catkin_LIBRARIES})

add_executable(s35 src/s35.cpp)
target_link_libraries(s35 ${catkin_LIBRARIES})

add_executable(s36 src/s36.cpp)
target_link_libraries(s36 ${catkin_LIBRARIES})

add_executable(s37 src/s37.cpp)
target_link_libraries(s37 ${catkin_LIBRARIES})

add_executable(s38 src/s38.cpp)
target_link_libraries(s38 ${catkin_LIBRARIES})

add_executable(s39 src/s39.cpp)
target_link_libraries(s39 ${catkin_LIBRARIES})

add_executable(s40 src/s40.cpp)
target_link_libraries(s40 ${catkin_LIBRARIES})

add_executable(s41 src/s41.cpp)
target_link_libraries(s41 ${catkin_LIBRARIES})

add_executable(s42 src/s42.cpp)
target_link_libraries(s42 ${catkin_LIBRARIES})

add_executable(s43 src/s43.cpp)
target_link_libraries(s43 ${catkin_LIBRARIES})

add_executable(s44 src/s44.cpp)
target_link_libraries(s44 ${catkin_LIBRARIES})

add_executable(s45 src/s45.cpp)
target_link_libraries(s45 ${catkin_LIBRARIES})

add_executable(s46 src/s46.cpp)
target_link_libraries(s46 ${catkin_LIBRARIES})

add_executable(s47 src/s47.cpp)
target_link_libraries(s47 ${catkin_LIBRARIES})

add_executable(s48 src/s48.cpp)
target_link_libraries(s48 ${catkin_LIBRARIES})

add_executable(s49 src/s49.cpp)
target_link_libraries(s49 ${catkin_LIBRARIES})

add_executable(s50 src/s50.cpp)
target_link_libraries(s50 ${catkin_LIBRARIES})

add_executable(s51 src/s51.cpp)
target_link_libraries(s51 ${catkin_LIBRARIES})

add_executable(s52 src/s52.cpp)
target_link_libraries(s52 ${catkin_LIBRARIES})

add_executable(s53 src/s53.cpp)
target_link_libraries(s53 ${catkin_LIBRARIES})

add_executable(s54 src/s54.cpp)
target_link_libraries(s54 ${catkin_LIBRARIES})

add_executable(s55 src/s55.cpp)
target_link_libraries(s55 ${catkin_LIBRARIES})

add_executable(s56 src/s56.cpp)
target_link_libraries(s56 ${catkin_LIBRARIES})

add_executable(s57 src/s57.cpp)
target_link_libraries(s57 ${catkin_LIBRARIES})

add_executable(s58 src/s58.cpp)
target_link_libraries(s58 ${catkin_LIBRARIES})

add_executable(s59 src/s59.cpp)
target_link_libraries(s59 ${catkin_LIBRARIES})

add_executable(s60 src/s60.cpp)
target_link_libraries(s60 ${catkin_LIBRARIES})

add_executable(s61 src/s61.cpp)
target_link_libraries(s61 ${catkin_LIBRARIES})

add_executable(s62 src/s62.cpp)
target_link_libraries(s62 ${catkin_LIBRARIES})

add_executable(s63 src/s63.cpp)
target_link_libraries(s63 ${catkin_LIBRARIES})

add_executable(s64 src/s64.cpp)
target_link_libraries(s64 ${catkin_LIBRARIES})

add_executable(s65 src/s65.cpp)
target_link_libraries(s65 ${catkin_LIBRARIES})

add_executable(s66 src/s66.cpp)
target_link_libraries(s66 ${catkin_LIBRARIES})

add_executable(s67 src/s67.cpp)
target_link_libraries(s67 ${catkin_LIBRARIES})

add_executable(s68 src/s68.cpp)
target_link_libraries(s68 ${catkin_LIBRARIES})
## Rename C++ executable without prefix
## The above recommended prefix causes long target names, the following renames the
## target back to the shorter version for ease of user use
## e.g. "rosrun someones_pkg node" instead of "rosrun someones_pkg someones_pkg_node"
# set_target_properties(${PROJECT_NAME}_node PROPERTIES OUTPUT_NAME node PREFIX "")

## Add cmake target dependencies of the executable
## same as for the library above
# add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against
# target_link_libraries(${PROJECT_NAME}_node
#   ${catkin_LIBRARIES}
# )

#############
## Install ##
#############

# all install targets should use catkin DESTINATION variables
# See http://ros.org/doc/api/catkin/html/adv_user_guide/variables.html

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# catkin_install_python(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark executables for installation
## See http://docs.ros.org/melodic/api/catkin/html/howto/format1/building_executables.html
# install(TARGETS ${PROJECT_NAME}_node
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

## Mark libraries for installation
## See http://docs.ros.org/melodic/api/catkin/html/howto/format1/building_libraries.html
# install(TARGETS ${PROJECT_NAME}
#   ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# catkin_add_gtest(${PROJECT_NAME}-test test/test_offer.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# catkin_add_nosetests(test)
