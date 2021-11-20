cmake_minimum_required( VERSION 2.8 )
project( basic )

set( CMAKE_CXX_FLAGS "-std=c++11 -O3" )


add_executable( private_derive object-oriented/private_derive.cpp)
add_executable( move_assignment object-oriented/move_assignment.cpp)



