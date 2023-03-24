cmake_minimum_required (VERSION 3.0)
set(CMAKE_BUILD_TYPE Debug)

# Below line is for using C++14 standard
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

# Our project name
project (balling)

add_executable(balling main.cpp)

# For finding our SDL2 package!
INCLUDE(FindPkgConfig)

# Searching SDL2 package.
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
PKG_SEARCH_MODULE(SDL2IMAGE REQUIRED SDL2_image>=2.0.0)

# Including our external library's directory for header files!
INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})

# Linking SDL2 library with our project.
target_link_libraries(balling ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
