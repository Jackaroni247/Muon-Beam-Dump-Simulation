# CMakeLists.txt
cmake_minimum_required(VERSION 3.16)
project(BeamDumpTest)

# Find Geant4 package, requiring User Interface and Visualization modules
find_package(Geant4 REQUIRED ui_all vis_all)

# Include Geant4's standard use file to set up directories and flags
include(${Geant4_USE_FILE})

# Automatically find all C++ source and header files in the current directory
file(GLOB sources ${PROJECT_SOURCE_DIR}/*.cc)
file(GLOB headers ${PROJECT_SOURCE_DIR}/*.hh)

# Define the executable name and the files needed to build it
add_executable(BeamDumpTest ${sources} ${headers})

# Link the Geant4 libraries to your new executable
target_link_libraries(BeamDumpTest ${Geant4_LIBRARIES})
