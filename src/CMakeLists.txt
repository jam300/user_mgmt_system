# Recoge todos los archivos .cpp dentro de src/
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS *.cpp)

add_executable(user_mgmt_system ${SRC_FILES})

# Incluye headers
target_include_directories(user_mgmt_system PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)
