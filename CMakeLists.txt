cmake_minimum_required(VERSION 3.8)
project(estd)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -Wpedantic)

option(WERROR "Turn compiler warnings into errors." OFF)
if (WERROR)
	add_compile_options(-Werror)
endif()

# Use catkin only if available or explicitly requested.
set(USE_CATKIN "auto" CACHE STRING "Use catkin as build tool. One of: auto, on, off")
string(TOLOWER "${USE_CATKIN}" use_catkin)

if(use_catkin STREQUAL "on")
	find_package(catkin REQUIRED)
	message("Use catkin: ON (forced)")
elseif(use_catkin STREQUAL "off")
	set(catkin_FOUND OFF)
	message("Use catkin: OFF (forced)")
elseif(use_catkin STREQUAL "auto")
	find_package(catkin QUIET)
	if (catkin_FOUND)
		message("Use catkin: ON (auto)")
	else()
		message("Use catkin: OFF (auto)")
	endif()
else()
	message(FATAL_ERROR "Invalid value for USE_CATKIN. Expected \"auto\", \"on\" or \"off\", got \"${USE_CATKIN}\".")
endif()


if (catkin_FOUND)
	catkin_package(INCLUDE_DIRS include)
endif()

if (DEFINED CATKIN_ENABLE_TESTING AND NOT CATKIN_ENABLE_TESTING)
	option(BUILD_TESTS "Build tests" OFF)
else()
	option(BUILD_TESTS "Build tests" ON)
endif()

include_directories(include/${PROJECT_NAME})

if (BUILD_TESTS)
	add_subdirectory(test)
endif()

if (catkin_FOUND)
	install(DIRECTORY include/${PROJECT_NAME}/
		DESTINATION ${CATKIN_PACKAGE_INCLUDE_DESTINATION}
	)
else()
	include(GNUInstallDirs)
	install(DIRECTORY include/${PROJECT_NAME}
		DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	)
endif()
