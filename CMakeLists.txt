cmake_minimum_required(VERSION 3.25)
project(Dungi VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ============================================================
# OUTPUT DIRECTORIES
# ============================================================
# ============================================================
# PRESET DETECTION
# Dev preset: add "DEV_PRESET": "ON" to cacheVariables
# ============================================================
if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR DEFINED DEV_PRESET)
  set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/build/dev")
else()
  set(OUTPUT_DIR "${CMAKE_SOURCE_DIR}/build/release")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${OUTPUT_DIR}/lib")

foreach(CONFIG_TYPE Debug Release RelWithDebInfo MinSizeRel)
  string(TOUPPER ${CONFIG_TYPE} CONFIG_UPPER)
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${CONFIG_UPPER} "${OUTPUT_DIR}/bin")
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${CONFIG_UPPER} "${OUTPUT_DIR}/lib")
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_UPPER} "${OUTPUT_DIR}/lib")
endforeach()

# ============================================================
# CROW2D DEPENDENCY
# During development: points at Crow2D's live build output.
# For release: swap CROW2D_DIR to an installed SDK path or
# a third_party copy — nothing else in this file changes.
# ============================================================

# Can be overridden from the preset or command line:
# cmake --preset dev -DCROW2D_DIR=D:/Apps/Crow2D/Crow2D/build/dev
set(CROW2D_DIR "D:/Apps/Crow2D/Crow2D/build/dev"
  CACHE PATH "Path to Crow2D build or install directory")

find_package(Crow2D REQUIRED
  PATHS "${CROW2D_DIR}"
  NO_DEFAULT_PATH
)

# ============================================================
# DUNGI  –  EXECUTABLE
# ============================================================
file(GLOB_RECURSE DUNGI_SOURCES
  CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/src/*.cpp"
  "${CMAKE_SOURCE_DIR}/src/*.cxx"
)
file(GLOB_RECURSE DUNGI_HEADERS
  CONFIGURE_DEPENDS
  "${CMAKE_SOURCE_DIR}/src/*.h"
  "${CMAKE_SOURCE_DIR}/src/*.hpp"
  "${CMAKE_SOURCE_DIR}/include/*.h"
  "${CMAKE_SOURCE_DIR}/include/*.hpp"
)

add_executable(Dungi ${DUNGI_SOURCES} ${DUNGI_HEADERS})

target_include_directories(Dungi
  PRIVATE
  "${CMAKE_SOURCE_DIR}/src"
  "${CMAKE_SOURCE_DIR}/include"
)

# Crow2D::Crow2D transitively pulls SDL3, SDL3_image and Ultralight
target_link_libraries(Dungi
  PRIVATE
  Crow2D::Crow2D
)
# ============================================================
# POST-BUILD  –  COPY CROW2D DLLs + RESOURCES
# Mirror what Crow2D puts in its own bin/ so Dungi can run
# without a full install step.
# ============================================================
add_custom_command(TARGET Dungi POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
  "${CROW2D_DIR}/bin"
  "${OUTPUT_DIR}/bin"
  COMMENT "Copying Crow2D runtime (DLLs + resources) → ${OUTPUT_DIR}/bin"
)
