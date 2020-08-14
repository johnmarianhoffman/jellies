cmake_minimum_required (VERSION 3.1 FATAL_ERROR)
project (j_blank_cpp_project)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O3 -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Og -g -pthread")

# Third party
set(3RDPARTYDIR 3rdParty)

# isocharts
file( GLOB SOURCES src/*.cpp )

add_executable(j_blank_cpp_project ${SOURCES})
target_include_directories(j_blank_cpp_project PUBLIC
  include/
  ${3RDPARTYDIR}/Eigen)

target_link_libraries(j_blank_cpp_project)

