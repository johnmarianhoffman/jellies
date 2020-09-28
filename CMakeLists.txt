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
#find_package(GLUT REQUIRED)
find_package(PNG REQUIRED)

# Mac Specific
if (${APPLE})
  find_library(COCOA_LIBRARY Cocoa)
  find_library(IOKIT_LIBRARY IOKit)
  find_library(COREFOUNDATION_LIBRARY CoreFoundation)
endif()

# jellies
file( GLOB SOURCES src/*.cpp )

add_executable(jellies ${SOURCES})
target_include_directories(jellies PUBLIC
  include/
  /usr/local/include/
  ${EIGEN3_INCLUDE_DIRS})

if (${APPLE})
  #target_link_libraries(jellies -lz /usr/local/Cellar/libpng/1.6.37/lib/libpng.a  ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES})
  #target_link_libraries(jellies -lz /usr/local/Cellar/libpng/1.6.37/lib/libpng.a  ${OPENGL_LIBRARIES} /usr/local/lib/libglfw3.a "-framework Cocoa -framework IOKit -framework CoreFoundation")
  target_link_libraries(jellies -lz /usr/local/Cellar/libpng/1.6.37/lib/libpng.a  ${OPENGL_LIBRARIES} /usr/local/lib/libglfw3.a ${COCOA_LIBRARY} ${IOKIT_LIBRARY} ${COREFOUNDATION_LIBRARY})
endif()

if (${UNIX} AND (NOT ${APPLE}))
  target_link_libraries(jellies   -lX11 -lstdc++fs  ${OPENGL_LIBRARIES} ${ZLIB_LIBRARIES} ${PNG_LIBRARIES})
endif()
