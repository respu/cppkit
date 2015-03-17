cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 NEW)
project(buildy)

set(SOURCES source/build.cpp
            source/config.cpp
            source/option.cpp
            source/git.cpp
            source/utils.cpp
            source/main.cpp
            source/test.cpp)

set(CMAKE_BUILD_TYPE Debug)

macro(add_compiler_flag CONFIG FLAG)
    if("${CONFIG}" STREQUAL "Both")
        set(CMAKE_CXX_FLAGS "${FLAG} ${CMAKE_CXX_FLAGS}")
    elseif("${CONFIG}" STREQUAL "Debug")
        set(CMAKE_CXX_FLAGS_DEBUG "${FLAG} ${CMAKE_CXX_FLAGS_DEBUG}")
    elseif("${CONFIG}" STREQUAL "Release")
        set(CMAKE_CXX_FLAGS_RELEASE "${FLAG} ${CMAKE_CXX_FLAGS_RELEASE}")
    else()
        message(FATAL_ERROR "The CONFIG argument to add_compiler_flag must be \"Both\", \"Debug\", or \"Release\"")
    endif()
endmacro()

if(CMAKE_SYSTEM MATCHES "Linux-")
    add_compiler_flag(Both -fthreadsafe-statics)
    add_compiler_flag(Both -fPIC)
    add_compiler_flag(Release -O3)
    add_compiler_flag(Release -DNDEBUG)
    add_compiler_flag(Debug -ggdb)
    add_compiler_flag(Debug -O0)
    add_compiler_flag(Both -std=c++11)
    add_definitions(-D_LINUX)
    add_definitions(-DLINUX_OS)
    add_definitions(-D_REENTRANT)
    set(CMAKE_EXE_LINKER_FLAGS -rdynamic)
elseif(CMAKE_SYSTEM MATCHES "Windows")

    if(${CMAKE_SIZEOF_VOID_P} EQUAL 8)
        add_definitions(-DWIN64)
    endif(${CMAKE_SIZEOF_VOID_P} EQUAL 8)

    add_definitions(-DWIN32)
    add_definitions(-DUNICODE)
    add_definitions(-D_UNICODE)
    add_definitions(-DNOMINMAX)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
    add_definitions(-D__inline__=__inline)
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
    # Enable read-only string pooling.
    add_compiler_flag(Both /GF)
    # Make sure that destructors get executed when exceptions exit C++ code.
    add_compiler_flag(Both /EHsc)
    # Generate pdb files which support Edit and Continue Debugging
    add_compiler_flag(Debug /ZI)
    # Enable minimal rebuild
    add_compiler_flag(Debug /Gm)
    # Create an .sbr file with complete symbolic information.
    add_compiler_flag(Debug /FR)
    add_compiler_flag(Debug /MDd)
    add_compiler_flag(Release /MD)
    add_compiler_flag(Release /O2)
    SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
elseif(CMAKE_SYSTEM MATCHES "Darwin-")

    add_compiler_flag(Both -fthreadsafe-statics)
    add_compiler_flag(Both -fPIC)
    add_compiler_flag(Both -std=c++11)
    add_compiler_flag(Release -O3)
    add_compiler_flag(Release -DNDEBUG)
    add_compiler_flag(Debug -ggdb)
    add_compiler_flag(Debug -O0)
    add_definitions(-DMACOSX)
    add_definitions(-D_REENTRANT)
    set(CMAKE_EXE_LINKER_FLAGS -rdynamic)

endif(CMAKE_SYSTEM MATCHES "Linux-")

include_directories(include ../../../devel_artifacts/include)
link_directories(../../../devel_artifacts/lib)

SET(CMAKE_SKIP_BUILD_RPATH TRUE)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
SET(CMAKE_INSTALL_RPATH "\$ORIGIN")
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

if(CMAKE_SYSTEM MATCHES "Windows")
    set(RES_FILES ${RC_FILE})
    set(CMAKE_RC_COMPILER_INIT windres)
    ENABLE_LANGUAGE(RC)
    SET(CMAKE_RC_COMPILE_OBJECT
    "<CMAKE_RC_COMPILER> <FLAGS> -o coff <DEFINES> -i <SOURCE> -o <OBJECT>")
    add_executable(${PROJECT_NAME} ${SOURCES} ${RES_FILES})
else()
    add_executable(${PROJECT_NAME} ${SOURCES})
endif()

target_link_libraries(${PROJECT_NAME} ${WINDOWS_LIBS} cppkit)

set(CMAKE_INSTALL_PREFIX ../../../../)

install(TARGETS buildy DESTINATION buildy)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build_scripts DESTINATION buildy)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/example_config DESTINATION buildy)

if(CMAKE_SYSTEM MATCHES "Windows")
    install(FILES ../../../devel_artifacts/lib/libcppkit.dll DESTINATION buildy)
    if( "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" )
        install(FILES "c:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/redist/debug_nonredist/x64/Microsoft.VC140.DebugCRT/msvcp140d.dll" DESTINATION buildy)
    else()
        install(FILES "c:/Program Files (x86)/Microsoft Visual Studio 14.0/VC/redist/x64/Microsoft.VC140.CRT/msvcp140.dll" DESTINATION buildy)
    endif()
elseif(CMAKE_SYSTEM MATCHES "Linux")
    install(FILES ../../../devel_artifacts/lib/libcppkit.so DESTINATION buildy)
    install(PROGRAMS ${CMAKE_CURRENT_SOURCE_DIR}/utils/ssh_cmd DESTINATION buildy)
    install(PROGRAMS ${CMAKE_CURRENT_SOURCE_DIR}/utils/scp_cmd DESTINATION buildy)
endif()
