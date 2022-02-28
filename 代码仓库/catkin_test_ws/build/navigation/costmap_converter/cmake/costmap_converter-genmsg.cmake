# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "costmap_converter: 2 messages, 0 services")

set(MSG_I_FLAGS "-Icostmap_converter:/home/wu/catkin_test_ws/src/navigation/costmap_converter/msg;-Igeometry_msgs:/opt/ros/melodic/share/geometry_msgs/cmake/../msg;-Istd_msgs:/opt/ros/melodic/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(costmap_converter_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/wu/catkin_test_ws/src/navigation/costmap_converter/msg/ObstacleMsg.msg" NAME_WE)
add_custom_target(_costmap_converter_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "costmap_converter" "/home/wu/catkin_test_ws/src/navigation/costmap_converter/msg/ObstacleMsg.msg" "std_msgs/Header:geometry_msgs/Twist:geometry_msgs/Vector3:geometry_msgs/TwistWithCovariance:geometry_msgs/Point32:geometry_msgs/Polygon:geometry_msgs/Quaternion"
)

get_filename_component(_filename "/home/wu/catkin_test_ws/src/navigation/costmap_converter/msg/ObstacleArrayMsg.msg" NAME_WE)
add_custom_target(_costmap_converter_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "costmap_converter" "/home/wu/catkin_test_ws/src/navigation/costmap_converter/msg/ObstacleArrayMsg.msg" "costmap_converter/ObstacleMsg:std_msgs/Header:geometry_msgs/Twist:geometry_msgs/Vector3:geometry_msgs/TwistWithCovariance:geometry_msgs/Point32:geometry_msgs/Polygon:geometry_msgs/Quaternion"
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(costmap_converter
  "/home/wu/catkin_test_ws/src/navigation/costmap_converter/msg/ObstacleMsg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Twist.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/TwistWithCovariance.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point32.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Polygon.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/costmap_converter
)
_generate_msg_cpp(costmap_converter
  "/home/wu/catkin_test_ws/src/navigation/costmap_converter/msg/ObstacleArrayMsg.msg"
  "${MSG_I_FLAGS}"
  "/home/wu/catkin_test_ws/src/navigation/costmap_converter/msg/ObstacleMsg.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Twist.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/TwistWithCovariance.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point32.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Polygon.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/costmap_converter
)

### Generating Services

### Generating Module File
_generate_module_cpp(costmap_converter
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/costmap_converter
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(costmap_converter_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(costmap_converter_generate_messages costmap_converter_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/wu/catkin_test_ws/src/navigation/costmap_converter/msg/ObstacleMsg.msg" NAME_WE)
add_dependencies(costmap_converter_generate_messages_cpp _costmap_converter_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/wu/catkin_test_ws/src/navigation/costmap_converter/msg/ObstacleArrayMsg.msg" NAME_WE)
add_dependencies(costmap_converter_generate_messages_cpp _costmap_converter_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(costmap_converter_gencpp)
add_dependencies(costmap_converter_gencpp costmap_converter_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS costmap_converter_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(costmap_converter
  "/home/wu/catkin_test_ws/src/navigation/costmap_converter/msg/ObstacleMsg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Twist.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/TwistWithCovariance.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point32.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Polygon.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/costmap_converter
)
_generate_msg_eus(costmap_converter
  "/home/wu/catkin_test_ws/src/navigation/costmap_converter/msg/ObstacleArrayMsg.msg"
  "${MSG_I_FLAGS}"
  "/home/wu/catkin_test_ws/src/navigation/costmap_converter/msg/ObstacleMsg.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Twist.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/TwistWithCovariance.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point32.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Polygon.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/costmap_converter
)

### Generating Services

### Generating Module File
_generate_module_eus(costmap_converter
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/costmap_converter
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(costmap_converter_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(costmap_converter_generate_messages costmap_converter_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/wu/catkin_test_ws/src/navigation/costmap_converter/msg/ObstacleMsg.msg" NAME_WE)
add_dependencies(costmap_converter_generate_messages_eus _costmap_converter_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/wu/catkin_test_ws/src/navigation/costmap_converter/msg/ObstacleArrayMsg.msg" NAME_WE)
add_dependencies(costmap_converter_generate_messages_eus _costmap_converter_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(costmap_converter_geneus)
add_dependencies(costmap_converter_geneus costmap_converter_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS costmap_converter_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(costmap_converter
  "/home/wu/catkin_test_ws/src/navigation/costmap_converter/msg/ObstacleMsg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Twist.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/TwistWithCovariance.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point32.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Polygon.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/costmap_converter
)
_generate_msg_lisp(costmap_converter
  "/home/wu/catkin_test_ws/src/navigation/costmap_converter/msg/ObstacleArrayMsg.msg"
  "${MSG_I_FLAGS}"
  "/home/wu/catkin_test_ws/src/navigation/costmap_converter/msg/ObstacleMsg.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Twist.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/TwistWithCovariance.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point32.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Polygon.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/costmap_converter
)

### Generating Services

### Generating Module File
_generate_module_lisp(costmap_converter
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/costmap_converter
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(costmap_converter_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(costmap_converter_generate_messages costmap_converter_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/wu/catkin_test_ws/src/navigation/costmap_converter/msg/ObstacleMsg.msg" NAME_WE)
add_dependencies(costmap_converter_generate_messages_lisp _costmap_converter_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/wu/catkin_test_ws/src/navigation/costmap_converter/msg/ObstacleArrayMsg.msg" NAME_WE)
add_dependencies(costmap_converter_generate_messages_lisp _costmap_converter_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(costmap_converter_genlisp)
add_dependencies(costmap_converter_genlisp costmap_converter_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS costmap_converter_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(costmap_converter
  "/home/wu/catkin_test_ws/src/navigation/costmap_converter/msg/ObstacleMsg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Twist.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/TwistWithCovariance.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point32.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Polygon.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/costmap_converter
)
_generate_msg_nodejs(costmap_converter
  "/home/wu/catkin_test_ws/src/navigation/costmap_converter/msg/ObstacleArrayMsg.msg"
  "${MSG_I_FLAGS}"
  "/home/wu/catkin_test_ws/src/navigation/costmap_converter/msg/ObstacleMsg.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Twist.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/TwistWithCovariance.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point32.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Polygon.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/costmap_converter
)

### Generating Services

### Generating Module File
_generate_module_nodejs(costmap_converter
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/costmap_converter
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(costmap_converter_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(costmap_converter_generate_messages costmap_converter_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/wu/catkin_test_ws/src/navigation/costmap_converter/msg/ObstacleMsg.msg" NAME_WE)
add_dependencies(costmap_converter_generate_messages_nodejs _costmap_converter_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/wu/catkin_test_ws/src/navigation/costmap_converter/msg/ObstacleArrayMsg.msg" NAME_WE)
add_dependencies(costmap_converter_generate_messages_nodejs _costmap_converter_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(costmap_converter_gennodejs)
add_dependencies(costmap_converter_gennodejs costmap_converter_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS costmap_converter_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(costmap_converter
  "/home/wu/catkin_test_ws/src/navigation/costmap_converter/msg/ObstacleMsg.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Twist.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/TwistWithCovariance.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point32.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Polygon.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/costmap_converter
)
_generate_msg_py(costmap_converter
  "/home/wu/catkin_test_ws/src/navigation/costmap_converter/msg/ObstacleArrayMsg.msg"
  "${MSG_I_FLAGS}"
  "/home/wu/catkin_test_ws/src/navigation/costmap_converter/msg/ObstacleMsg.msg;/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Twist.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Vector3.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/TwistWithCovariance.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Point32.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Polygon.msg;/opt/ros/melodic/share/geometry_msgs/cmake/../msg/Quaternion.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/costmap_converter
)

### Generating Services

### Generating Module File
_generate_module_py(costmap_converter
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/costmap_converter
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(costmap_converter_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(costmap_converter_generate_messages costmap_converter_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/wu/catkin_test_ws/src/navigation/costmap_converter/msg/ObstacleMsg.msg" NAME_WE)
add_dependencies(costmap_converter_generate_messages_py _costmap_converter_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/wu/catkin_test_ws/src/navigation/costmap_converter/msg/ObstacleArrayMsg.msg" NAME_WE)
add_dependencies(costmap_converter_generate_messages_py _costmap_converter_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(costmap_converter_genpy)
add_dependencies(costmap_converter_genpy costmap_converter_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS costmap_converter_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/costmap_converter)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/costmap_converter
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_cpp)
  add_dependencies(costmap_converter_generate_messages_cpp geometry_msgs_generate_messages_cpp)
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(costmap_converter_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/costmap_converter)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/costmap_converter
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_eus)
  add_dependencies(costmap_converter_generate_messages_eus geometry_msgs_generate_messages_eus)
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(costmap_converter_generate_messages_eus std_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/costmap_converter)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/costmap_converter
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_lisp)
  add_dependencies(costmap_converter_generate_messages_lisp geometry_msgs_generate_messages_lisp)
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(costmap_converter_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/costmap_converter)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/costmap_converter
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_nodejs)
  add_dependencies(costmap_converter_generate_messages_nodejs geometry_msgs_generate_messages_nodejs)
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(costmap_converter_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/costmap_converter)
  install(CODE "execute_process(COMMAND \"/usr/bin/python2\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/costmap_converter\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/costmap_converter
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET geometry_msgs_generate_messages_py)
  add_dependencies(costmap_converter_generate_messages_py geometry_msgs_generate_messages_py)
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(costmap_converter_generate_messages_py std_msgs_generate_messages_py)
endif()
