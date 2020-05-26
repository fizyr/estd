cmake_minimum_required(VERSION 3.8)
project(estd)
set(PROJECT_VERSION 0.1.12)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Wextra -Wpedantic)
add_link_options(LINKER:--as-needed)

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
	install(DIRECTORY "include/${PROJECT_NAME}/"
		DESTINATION "${CATKIN_PACKAGE_INCLUDE_DESTINATION}"
		COMPONENT development
	)
else()
	include(GNUInstallDirs)
	set(CMAKE_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}" CACHE PATH "Installation directory for CMake files.")

	include(CMakePackageConfigHelpers)
	write_basic_package_version_file("${PROJECT_NAME}-config-version.cmake" COMPATIBILITY SameMajorVersion)

	add_library(${PROJECT_NAME} INTERFACE)
	target_include_directories(${PROJECT_NAME} INTERFACE "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
	target_compile_features(${PROJECT_NAME} INTERFACE cxx_std_17)

	install(TARGETS ${PROJECT_NAME} EXPORT targets)
	install(EXPORT targets DESTINATION "${CMAKE_INSTALL_CMAKEDIR}")
	install(FILES
		"${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME}-config.cmake"
		"${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}-config-version.cmake"
		DESTINATION "${CMAKE_INSTALL_CMAKEDIR}"
	)

	install(DIRECTORY "include/${PROJECT_NAME}"
		DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}"
		COMPONENT development
	)
endif()
