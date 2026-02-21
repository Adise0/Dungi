cmake_minimum_required(VERSION 3.16)
project(Dungi VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)



file(GLOB_RECURSE DUNGI_SRC CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)
add_executable(Dungi ${DUNGI_SRC})
target_link_libraries(Dungi PRIVATE Crow::Crow)
# -----------------------------
# Dependencies
# -----------------------------

# ---- Crow
list(PREPEND CMAKE_PREFIX_PATH "/mnt/storage/Downloads/SDKs")
find_package(Crow REQUIRED)


add_custom_command(TARGET Dungi POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${Crow_RESOURCES_DIR}"
    "$<TARGET_FILE_DIR:Dungi>/resources"
)

add_custom_command(TARGET Dungi POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${Crow_INSPECTOR_DIR}"
    "$<TARGET_FILE_DIR:Dungi>/inspector"
)

