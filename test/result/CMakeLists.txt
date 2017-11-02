set(TESTS
	tracker
	conversion
	copyable
	construction
	observers
	equality
)

foreach(test ${TESTS})
	add_executable(${PROJECT_NAME}_test_${test} ${test}.cpp)
	add_custom_target(run_test_${PROJECT_NAME}_${test} COMMAND ${PROJECT_NAME}_test_${test})
	add_dependencies(run_tests run_test_${PROJECT_NAME}_${test})
	add_test(NAME ${PROJECT_NAME}_${test} COMMAND ${PROJECT_NAME}_test_${test})
endforeach()
