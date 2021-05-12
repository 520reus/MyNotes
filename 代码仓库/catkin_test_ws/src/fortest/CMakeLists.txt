cmake_minimum_required(VERSION 3.0.2)
project(fortest)



find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  tf
  tf2
  dynamic_reconfigure
)

#set(CMAKE_LIBRARY_PATH "/usr/local/lib")
#set(Ipopt_lib_path "/usr/local/lib")

#find_package(Eigen REQUIRED)
#INCLUDE_DIRECTORIES(${Eigen_INCLUDE_DIRS})
include_directories( "/usr/include/eigen3/Eigen")


catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES forTest
#  CATKIN_DEPENDS roscpp std_msgs tf tf2
#  DEPENDS system_lib
)

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)


add_executable(mytest src/mytest.cpp)
target_link_libraries(mytest ${catkin_LIBRARIES})

add_executable(test3 src/test3.cpp)
target_link_libraries(test3 ${catkin_LIBRARIES} ${Eigen_LIBRARIES})

############ipopt#################
set(IPOPT_LIBRARY_DIRS /usr/local/lib)
set(IPOPT_INCLUDE_DIRS /usr/local/include/coin)

message("ipopt binary path is: " ${IPOPT_LIBRARY_DIRS} )
message("ipopt include path is: " ${IPOPT_INCLUDE_DIRS} )


include_directories(${IPOPT_INCLUDE_DIRS} ${CMAKE_SOURCE_DIR}/include)
link_directories(${IPOPT_LIBRARY_DIRS})

add_library(ipopt_nlp src/hs071_nlp.cpp)

#list(APPEND ALL_TARGET_LIBRARIES /usr/local/lib/libipopt.so)

add_executable(ipopt_example src/hs071_main.cpp)
target_link_libraries(ipopt_example ipopt_nlp)
target_link_libraries(ipopt_example ipopt)   #此句一定要写！

add_executable(test2 src/test2.cpp)
target_link_libraries(test2 ${catkin_LIBRARIES} ipopt)

#target_link_libraries(ipopt_example ${ALL_TARGET_LIBRARIES}) 
#############Cppad#################
add_executable(cppad_example src/cppad_example.cpp)
target_link_libraries(cppad_example ipopt) #cppad依赖于ipopt求解器，因此一定要链接才不会出错

# mytest
add_executable(cppad_mytest src/cppad_mytest.cpp)
target_link_libraries(cppad_mytest ipopt)

##############generate_discrete_points-matplotlib#############

##python
find_package(PythonLibs 2.7)
add_executable(generate_discrete_points src/generate_discrete_points.cpp)
target_include_directories(generate_discrete_points PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(generate_discrete_points ${PYTHON_LIBRARIES} ipopt)

add_executable(dataType_trans src/dataType_trans.cpp)
##############discrete_points_smoother-ipopt#############
set(ADOLC_LIBRARY_DIRS "/usr/lib/x86_64-linux-gnu") # libadolc.so所在位置
link_directories(${ADOLC_LIBRARY_DIRS})

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/lib)
add_library(discrete_points_smoothing src/discrete_points_smooth_ipopt_interface.cpp 
                                      src/discrete_points_smoother.cpp)
target_link_libraries(discrete_points_smoothing ipopt adolc)

add_executable(discrete_points_main src/discrete_points_main.cpp)
target_include_directories(discrete_points_main PRIVATE ${PYTHON_INCLUDE_DIRS})
target_link_libraries(discrete_points_main ${PYTHON_LIBRARIES} ${Eigen_LIBRARIES} discrete_points_smoothing)





