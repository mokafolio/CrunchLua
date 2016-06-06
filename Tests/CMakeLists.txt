add_executable (CrunchLuaTests CrunchLuaTests.cpp)
target_link_libraries(CrunchLuaTests ${CRUNCHLUADEPS})
add_custom_target(check COMMAND CrunchLuaTests)