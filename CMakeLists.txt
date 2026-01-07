cmake_minimum_required(VERSION 3.16)

# =========================
# vcpkg toolchain
# =========================
if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")
endif()

# =========================
# Проект
# =========================
project(memorylink_api LANGUAGES CXX)

# =========================
# Стандарт C++
# =========================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# =========================
# Исходные файлы
# =========================
file(GLOB_RECURSE EXTERNAL_SOURCE external/*.cpp)
file(GLOB_RECURSE CONFIG_SOURCE src/config/*.cpp)
file(GLOB_RECURSE NETWORK_SOURCE src/network/*.cpp)
file(GLOB_RECURSE VALIDATORS_SOURCE src/validators/*.cpp)
file(GLOB_RECURSE SERVICES_SOURCE src/services/*.cpp)
file(GLOB_RECURSE MODELS_SOURCE src/models/*.cpp)
file(GLOB_RECURSE UTILS_SOURCE src/utils/*.cpp)
file(GLOB_RECURSE REPOSITORIES_SOURCE src/repositories/*.cpp)
file(GLOB_RECURSE DB_SOURCE src/db/*.cpp)

add_executable(memorylink_api
        ${EXTERNAL_SOURCE}
        ${CONFIG_SOURCE}
        ${NETWORK_SOURCE}
        ${VALIDATORS_SOURCE}
        ${SERVICES_SOURCE}
        ${MODELS_SOURCE}
        ${UTILS_SOURCE}
        ${REPOSITORIES_SOURCE}
        ${DB_SOURCE}
        src/main.cpp
)

# =========================
# Пути к заголовочным файлам проекта
# =========================
target_include_directories(memorylink_api PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/external
)

# =========================
# Зависимости
# =========================

# libpqxx
find_package(libpqxx CONFIG REQUIRED)
target_link_libraries(memorylink_api PRIVATE libpqxx::pqxx)

# AWS SDK S3
find_package(AWSSDK REQUIRED COMPONENTS s3)
target_include_directories(memorylink_api PRIVATE ${AWSSDK_INCLUDE_DIRS})
target_link_libraries(memorylink_api PRIVATE ${AWSSDK_LIBRARIES})

# FFmpeg
find_package(FFMPEG REQUIRED)
target_link_libraries(memorylink_api PRIVATE ${FFMPEG_LIBRARIES})
target_include_directories(memorylink_api PRIVATE ${FFMPEG_INCLUDE_DIRS})