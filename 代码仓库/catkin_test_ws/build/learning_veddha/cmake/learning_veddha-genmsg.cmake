# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "learning_veddha: 1 messages, 0 services")

set(MSG_I_FLAGS "-Ilearning_veddha:/home/wu/catkin_test_ws/src/learning_veddha/msg;-Istd_msgs:/opt/ros/melodic/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(learning_veddha_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/wu/catkin_test_ws/src/learning_veddha/msg/GpsInfo.msg" NAME_WE)
add_custom_target(_learning_veddha_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "learning_veddha" "/home/wu/catkin_test_ws/src/learning_veddha/msg/GpsInfo.msg" "std_msgs/Header"
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(learning_veddha
  "/home/wu/catkin_test_ws/src/learning_veddha/msg/GpsInfo.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/learning_veddha
)

### Generating Services

### Generating Module File
_generate_module_cpp(learning_veddha
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/learning_veddha
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(learning_veddha_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(learning_veddha_generate_messages learning_veddha_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/wu/catkin_test_ws/src/learning_veddha/msg/GpsInfo.msg" NAME_WE)
add_dependencies(learning_veddha_generate_messages_cpp _learning_veddha_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(learning_veddha_gencpp)
add_dependencies(learning_veddha_gencpp learning_veddha_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS learning_veddha_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(learning_veddha
  "/home/wu/catkin_test_ws/src/learning_veddha/msg/GpsInfo.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/learning_veddha
)

### Generating Services

### Generating Module File
_generate_module_eus(learning_veddha
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/learning_veddha
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(learning_veddha_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(learning_veddha_generate_messages learning_veddha_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/wu/catkin_test_ws/src/learning_veddha/msg/GpsInfo.msg" NAME_WE)
add_dependencies(learning_veddha_generate_messages_eus _learning_veddha_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(learning_veddha_geneus)
add_dependencies(learning_veddha_geneus learning_veddha_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS learning_veddha_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(learning_veddha
  "/home/wu/catkin_test_ws/src/learning_veddha/msg/GpsInfo.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/learning_veddha
)

### Generating Services

### Generating Module File
_generate_module_lisp(learning_veddha
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/learning_veddha
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(learning_veddha_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(learning_veddha_generate_messages learning_veddha_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/wu/catkin_test_ws/src/learning_veddha/msg/GpsInfo.msg" NAME_WE)
add_dependencies(learning_veddha_generate_messages_lisp _learning_veddha_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(learning_veddha_genlisp)
add_dependencies(learning_veddha_genlisp learning_veddha_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS learning_veddha_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(learning_veddha
  "/home/wu/catkin_test_ws/src/learning_veddha/msg/GpsInfo.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/learning_veddha
)

### Generating Services

### Generating Module File
_generate_module_nodejs(learning_veddha
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/learning_veddha
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(learning_veddha_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(learning_veddha_generate_messages learning_veddha_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/wu/catkin_test_ws/src/learning_veddha/msg/GpsInfo.msg" NAME_WE)
add_dependencies(learning_veddha_generate_messages_nodejs _learning_veddha_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(learning_veddha_gennodejs)
add_dependencies(learning_veddha_gennodejs learning_veddha_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS learning_veddha_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(learning_veddha
  "/home/wu/catkin_test_ws/src/learning_veddha/msg/GpsInfo.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/learning_veddha
)

### Generating Services

### Generating Module File
_generate_module_py(learning_veddha
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/learning_veddha
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(learning_veddha_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(learning_veddha_generate_messages learning_veddha_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/wu/catkin_test_ws/src/learning_veddha/msg/GpsInfo.msg" NAME_WE)
add_dependencies(learning_veddha_generate_messages_py _learning_veddha_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(learning_veddha_genpy)
add_dependencies(learning_veddha_genpy learning_veddha_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS learning_veddha_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/learning_veddha)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/learning_veddha
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(learning_veddha_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/learning_veddha)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/learning_veddha
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(learning_veddha_generate_messages_eus std_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/learning_veddha)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/learning_veddha
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(learning_veddha_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/learning_veddha)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/learning_veddha
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(learning_veddha_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/learning_veddha)
  install(CODE "execute_process(COMMAND \"/usr/bin/python2\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/learning_veddha\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/learning_veddha
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(learning_veddha_generate_messages_py std_msgs_generate_messages_py)
endif()
