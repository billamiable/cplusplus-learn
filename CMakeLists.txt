cmake_minimum_required( VERSION 2.8 )
project( basic )

set( CMAKE_CXX_FLAGS "-std=c++17 -O3" )

# add_executable( private_derive private_derive.cpp)
# add_executable( move_assignment move_assignment.cpp)
add_executable( modern-feat modern-feat.cpp)
add_executable( oop oop.cpp)
add_executable( test_memory_leak test_memory_leak.cpp)



