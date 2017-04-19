# CMake settings
cmake_minimum_required(VERSION 3.6)

set(SOLUTION_NAME Nurn)
project(${SOLUTION_NAME})

option(COMPILE_RELEASE_BUILD "Unchecked is debug" OFF)

set(OUTPUT_FOLDER ${CMAKE_BINARY_DIR}/build)
set(OUTPUT_FOLDER ${OUTPUT_FOLDER}/32)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_FOLDER})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${OUTPUT_FOLDER})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_FOLDER})

# Path for copying .dll to correct folder
if(COMPILE_RELEASE_BUILD)
	set(OUTPUT_FOLDER ${OUTPUT_FOLDER}/Release)	
else()
	set(OUTPUT_FOLDER ${OUTPUT_FOLDER}/Debug)
endif()

# Find external libraries and modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/Modules/")
include(create_source_group)


set(EXTERNAL_INCLUDE_DIRS)
set(EXTERNAL_LIBRARIES_ALL)
include_directories(${EXTERNAL_INCLUDE_DIRS})

# Set global compiler options
add_definitions(-DUNICODE -D_UNICODE)

# Root project settings
set(PROJECT_NAME Nurn)

# Set the files used in the target
file(GLOB_RECURSE HEADERS *.hpp)
file(GLOB_RECURSE SOURCES *.cpp)

create_source_group("Header Files" "${CMAKE_CURRENT_SOURCE_DIR}/hpp" ${HEADERS})
create_source_group("Source Files" "${CMAKE_CURRENT_SOURCE_DIR}/cpp" ${SOURCES})

set(INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/hpp)

set(DEFINITIONS NURN_EXPORTS)

add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC "${INCLUDE_DIRECTORIES}")
target_compile_definitions(${PROJECT_NAME} PRIVATE "${DEFINITIONS}")
target_compile_features(${PROJECT_NAME} PRIVATE cxx_range_for)
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})



# Set the default configuration
if (COMPILE_RELEASE_BUILD)
    set(CMAKE_BUILD_TYPE "Release")
else()
    set(CMAKE_BUILD_TYPE "Debug")
endif()
set(CMAKE_CONFIGURATION_TYPES ${CMAKE_BUILD_TYPE} CACHE STRING "" FORCE)