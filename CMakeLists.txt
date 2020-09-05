cmake_minimum_required (VERSION 3.1 FATAL_ERROR)
project (jellies)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-pthread -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Og -g")

# Third party
set(3RDPARTYDIR 3rdParty)
find_package(Eigen3)

# OpenGL
find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)

# jellies
file( GLOB SOURCES src/*.cpp )

add_executable(jellies ${SOURCES})
target_include_directories(jellies PUBLIC
  include/
  /usr/local/include/
  ${EIGEN3_INCLUDE_DIRS})
target_link_libraries(jellies -lz /usr/local/Cellar/libpng/1.6.37/lib/libpng.a  ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} )

# OpenGL and context configuration
# TODO: Convert to findOpenGL() command
#find_package(SDL2 REQUIRED)

#get_cmake_property(_variableNames VARIABLES)
#list (SORT _variableNames)
#foreach (_variableName ${_variableNames})
#    message(STATUS "${_variableName}=${${_variableName}}")
#endforeach()

#target_link_libraries(jellies PUBLIC ${SDL2_LIBRARIES})
