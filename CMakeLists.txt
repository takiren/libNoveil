cmake_minimum_required(VERSION 3.22)
project(LibNoveil VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

set(LibNoveil_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(EXT_LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external)

add_definitions(-D_UNICODE)

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
    ${EXT_LIB_DIR}/quill
    ${EXT_LIB_DIR}/uuid_v4
    ${EXT_LIB_DIR}/json/single_include/nlohmann
    ${EXT_LIB_DIR}/magic_enum/include
    ${LibNoveil_PATH}/include
)

file(GLOB_RECURSE SOURCE_LIST CONFIGURE_DEPENDS "${LibNoveil_PATH}/include/*.h" "${LibNoveil_PATH}/src/*.cpp")

# ----------------------------------------
# ----------------------------------------
# ----------------------------------------
set(CMAKE_BUILD_TYPE $<IF:$<CONFIG:Debug>,Debug, Release)

add_library(NEngine STATIC ${SOURCE_LIST})

target_link_libraries(NEngine PUBLIC $<IF:$<CONFIG:Debug>,SDL2d.lib SDL2maind.lib,SDL2.lib SDL2main.lib>)

target_include_directories(NEngine PUBLIC ${LibNoveil_INC_DIR})

target_link_options(NEngine PRIVATE $<$<CONFIG:Debug>:/Debug>)

target_compile_options(NEngine PRIVATE -Wall -MP)

target_compile_features(NEngine PUBLIC cxx_std_17)

include(FetchContent)
FetchContent_Declare(
    googletest

    # Specify the commit you depend on and update it regularly.
    URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Now simply link against gtest or gtest_main as needed. Eg
add_executable(${PROJECT_NAME}-googletest include/NFileIO.h include/NGraph.h include/NSequencer.h LibNoveilTest/test.cpp include/NVariant.h)
target_link_libraries(${PROJECT_NAME}-googletest gtest_main)
target_include_directories(${PROJECT_NAME}-googletest PUBLIC ${LibNoveil_PATH}/include ${LibNoveil_PATH}/src ${LibNoveil_INC_DIR})
target_compile_features(${PROJECT_NAME}-googletest PUBLIC cxx_std_17)
add_test(NAME parsetest COMMAND ${PROJECT_NAME}-googletest)