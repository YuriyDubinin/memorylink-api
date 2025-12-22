cmake_minimum_required(VERSION 3.16)

# =========================
# vcpkg toolchain
# =========================
if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(
            CMAKE_TOOLCHAIN_FILE
            "${CMAKE_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake"
            CACHE STRING "Файл toolchain для vcpkg"
    )
endif()

# =========================
# Определение платформы и архитектуры
# =========================
if (APPLE)
    set(VCPKG_PLATFORM "osx")
elseif (UNIX)
    set(VCPKG_PLATFORM "linux")
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

if (CMAKE_SYSTEM_PROCESSOR MATCHES "arm64|aarch64")
    set(VCPKG_ARCH "arm64")
else()
    set(VCPKG_ARCH "x64")
endif()

set(VCPKG_TRIPLET "${VCPKG_ARCH}-${VCPKG_PLATFORM}")

# =========================
# Явное указание пути к libpqxx
# =========================
if (NOT DEFINED libpqxx_DIR)
    set(
            libpqxx_DIR
            "${CMAKE_SOURCE_DIR}/vcpkg_installed/${VCPKG_TRIPLET}/share/libpqxx"
            CACHE PATH "Путь к libpqxxConfig.cmake"
    )
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
file(GLOB_RECURSE CONFIG_SOURCE src/config/*.cpp)
file(GLOB_RECURSE NETWORK_SOURCE src/network/*.cpp)
file(GLOB_RECURSE VALIDATORS_SOURCE src/validators/*.cpp)
file(GLOB_RECURSE SERVICES_SOURCE src/services/*.cpp)
file(GLOB_RECURSE MODELS_SOURCE src/models/*.cpp)
file(GLOB_RECURSE UTILS_SOURCE src/utils/*.cpp)
file(GLOB_RECURSE REPOSITORIES_SOURCE src/repositories/*.cpp)
file(GLOB_RECURSE DB_SOURCE src/db/*.cpp)

add_executable(memorylink_api
        src/main.cpp
        ${CONFIG_SOURCE}
        ${NETWORK_SOURCE}
        ${VALIDATORS_SOURCE}
        ${SERVICES_SOURCE}
        ${MODELS_SOURCE}
        ${UTILS_SOURCE}
        ${REPOSITORIES_SOURCE}
        ${DB_SOURCE}
)

# =========================
# Пути к заголовочным файлам
# =========================
target_include_directories(memorylink_api PRIVATE
        ${CMAKE_SOURCE_DIR}/src
        ${CMAKE_SOURCE_DIR}/external
)

# =========================
# Зависимости
# =========================
find_package(libpqxx CONFIG REQUIRED)

target_link_libraries(memorylink_api PRIVATE
        libpqxx::pqxx
)


