cmake_minimum_required (VERSION 2.8.11)
project (CrunchLua)
set(CMAKE_CXX_FLAGS "-std=c++11 -fno-exceptions")
include_directories (${CMAKE_CURRENT_SOURCE_DIR} /usr/local/include)
link_directories(/usr/local/lib)

set (CRUNCHLUADEPS Stick lua)

set (CRUNCHLUAINC
CrunchLua/CrunchLua.hpp
)

install (FILES ${CRUNCHLUAINC} DESTINATION /usr/local/include/CrunchLua)
add_subdirectory (Tests)
