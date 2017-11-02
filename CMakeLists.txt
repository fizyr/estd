cmake_minimum_required(VERSION 3.8)
project(estd)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -Wpedantic)

option(BUILD_TESTS "Build tests" ON)

find_package(catkin REQUIRED)

catkin_package(
	INCLUDE_DIRS include
)

include_directories(include/${PROJECT_NAME})
include_directories(SYSTEM ${catkin_INCLUDE_DIRS})

if (BUILD_TESTS)
	add_subdirectory(test)
endif()

install(DIRECTORY include/${PROJECT_NAME}/
	DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
)
