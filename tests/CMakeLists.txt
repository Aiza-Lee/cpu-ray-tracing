add_executable(unit_tests test_main.cpp)
target_link_libraries(unit_tests PRIVATE rt_lib Catch2::Catch2WithMain)

include(CTest)
add_test(NAME unit_tests COMMAND unit_tests)
