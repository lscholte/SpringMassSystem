cmake_minimum_required(VERSION 3.0 FATAL_ERROR)

project(SpringMassSystem LANGUAGES CXX)

set(ATLAS_DIR "${PROJECT_SOURCE_DIR}/lib/atlas")
set(ATLAS_CONFIG_DIR "${ATLAS_DIR}/config")
set(INCLUDE_DIR "${PROJECT_SOURCE_DIR}/include")
set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/source")

include("${ATLAS_CONFIG_DIR}/Compiler.cmake")

add_subdirectory(${ATLAS_DIR})
add_subdirectory(${INCLUDE_DIR})
add_subdirectory(${SOURCE_DIR})

include_directories(${ATLAS_INCLUDE_DIRS})
include_directories(${INCLUDE_DIR})

link_libraries(${APP_NAME} ${ATLAS_LIBRARIES})

source_group(include FILES ${INCLUDE_FILES})
source_group(source FILES ${SOURCE_FILES})

add_executable(springMassSystem ${SOURCE_FILES} ${INCLUDE_FILES})
