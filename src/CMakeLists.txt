cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

# Set project name and version
project(snake-engine VERSION 0.10 LANGUAGES CXX)

# Enable debug symbols by default
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release 
    CACHE STRING 
      "Choose the type of build (Debug or Release)" 
      FORCE
    )
endif()

# Find SFML shared libraries
find_package(SFML 2.5.1
  COMPONENTS 
    system window graphics network audio REQUIRED
)

# Set include directory search paths


# Compile executable
add_executable(snake-engine main.cpp SnakeEngine.cpp Snake.cpp
  Apple.cpp Field.cpp Cell.cpp SimpleRect.cpp) 

target_include_directories(snake-engine
  PRIVATE
  "${CMAKE_SOURCE_DIR}"
 )

target_compile_features(snake-engine PRIVATE cxx_std_17)

# Link executable to required SFML libraries
target_link_libraries(snake-engine sfml-graphics)

# Install target
install(TARGETS snake-engine DESTINATION bin)

