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
find_package(Crow REQUIRED)


file(GLOB RESOURCE_FILES "${Crow_ASSETS_DIR}/*")

add_custom_command(TARGET Dungi POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E make_directory
          "$<TARGET_FILE_DIR:Dungi>/resources"
)

foreach(file ${RESOURCE_FILES})
  get_filename_component(name ${file} NAME)

  add_custom_command(TARGET Dungi POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${file}"
            "$<TARGET_FILE_DIR:Dungi>/resources/${name}"
  )
endforeach()

add_custom_command(TARGET Dungi POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${Crow_ULTRALIGHT_INSPECTOR_DIR}"
    "$<TARGET_FILE_DIR:Dungi>/inspector"
)

add_custom_command(TARGET Dungi POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${Crow_ULTRALIGHT_RESOURCES_DIR}"
    "$<TARGET_FILE_DIR:Dungi>/resources"
)
