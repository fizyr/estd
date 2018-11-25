add_library(${PROJECT_NAME}_test_main EXCLUDE_FROM_ALL main.cpp)

define_property(GLOBAL PROPERTY CHECK_TARGET BRIEF_DOCS "." FULL_DOCS ".")
set_property(GLOBAL PROPERTY CHECK_TARGET "check")

function(declare_test name)
	add_executable(${name} EXCLUDE_FROM_ALL ${ARGN})
	target_link_libraries(${name} PRIVATE ${PROJECT_NAME}_test_main)
	add_custom_target(check_${name} COMMAND ${name})

	get_property(check_target GLOBAL PROPERTY CHECK_TARGET)
	list(APPEND check_target COMMAND cmake -E cmake_echo_color --white --bold ${name} COMMAND ${name} DEPENDS ${name})
	set_property(GLOBAL PROPERTY CHECK_TARGET "${check_target}")

	add_test(NAME ${name} COMMAND ${name})
endfunction()

function(declare_tests prefix)
	foreach(test ${ARGN})
		declare_test(${prefix}${test} ${test}.cpp)
	endforeach()
endfunction()

function(declare_compile_test name)
	add_library(${name} EXCLUDE_FROM_ALL ${ARGN})
	if (TARGET tests)
		add_dependencies(tests ${name})
	endif()

	get_property(check_target GLOBAL PROPERTY CHECK_TARGET)
	list(APPEND check_target DEPENDS ${name})
	set_property(GLOBAL PROPERTY CHECK_TARGET "${check_target}")
endfunction()

function(declare_compile_tests prefix)
	foreach(test ${ARGN})
		declare_compile_test(${prefix}${test} ${test}.cpp)
	endforeach()
endfunction()


add_subdirectory(any)
add_subdirectory(convert)
add_subdirectory(range)
add_subdirectory(result)
add_subdirectory(traits)
add_subdirectory(tuple)
add_subdirectory(utility)
add_subdirectory(view)

get_property(check_target GLOBAL PROPERTY CHECK_TARGET)
add_custom_target(${check_target} USES_TERMINAL)

if (TARGET run_tests)
	add_dependencies(run_tests check)
endif()
