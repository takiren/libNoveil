cmake_minimum_required(VERSION 3.22)
project(LibNoveil VERSION 0.1.0 LANGUAGES CXX)

set(LibNoveil_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(LibNOVEIL_TARGET_BASE_DIR "${LibNoveil_PATH}")

set(EXT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)

add_definitions(-D_UNICODE)

add_subdirectory(${EXT_LIB_DIR}/SDL)
add_subdirectory(${EXT_LIB_DIR}/quill)
add_subdirectory(${EXT_LIB_DIR}/uuid_v4)

if(CMAKE_SYSTEM_NAME MATCHES "Windows")
    set(LibNOVEIL_TARGET "win")
    set(LibNOVEIL_WINDOWS TRUE)
    set(LibNOVEIL_ARCH "x86")

    if(CMAKE_CL_64)
        set(LibNOVEIL_ARCH "x64")
    endif()

    set(LibNOVEIL_TARGET_BASE_DIR ${LibNoveil_PATH}/${LibNOVEIL_ARCH})
endif()

if(LibNOVEIL_WINDOWS)
    set(PLATFORM_TOOLSET "$(PlatformToolset)")

    if(NOT("${CMAKE_GENERATOR}" MATCHES "Visual Studio.+"))
        set(PLATFORM_TOOLSET "v143")
    endif()
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${LibNOVEIL_TARGET_BASE_DIR}/$<IF:$<CONFIG:Debug>,Debug,Release>)

# ----------------------------------------
# ----------------------------------------
# ----------------------------------------
list(APPEND LibNoveil_INC_DIR
    ${EXT_LIB_DIR}/SDL/include
    ${LibNoveil_PATH}/include
    ${EXT_LIB_DIR}/quill
)

file(GLOB_RECURSE SOURCE_LIST CONFIGURE_DEPENDS "include/*.h" "src/*.cpp")

# ----------------------------------------
# ----------------------------------------
# ----------------------------------------
set(CMAKE_BUILD_TYPE $<IF:$<CONFIG:Debug>,Debug, Release)

add_library(NEngine STATIC ${SOURCE_LIST})

target_include_directories(NEngine PUBLIC ${LibNoveil_INC_DIR})

target_link_options(NEngine PRIVATE $<$<CONFIG:Debug>:/Debug>)

target_compile_options(NEngine PRIVATE -Wall -MP)

target_compile_features(NEngine PUBLIC cxx_std_17)

target_link_libraries(NEngine PUBLIC )