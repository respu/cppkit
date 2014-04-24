
# First, append our compile options...
#

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug)
ENDIF(NOT CMAKE_BUILD_TYPE)

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
    add_definitions(-DWIN32)
#    add_definitions(-D_USE_32BIT_TIME_T)
    add_definitions(-DUNICODE)
    add_definitions(-D_UNICODE)
    add_definitions(-DNOMINMAX)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
    add_definitions(-D__inline__=__inline)
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
    add_compiler_flag(Both /GF)    # Enable read-only string pooling.
    add_compiler_flag(Both /EHsc)  # Make sure that destructors get executed when exceptions exit C++ code.
    add_compiler_flag(Debug /ZI)   # Generate pdb files which support Edit and Continue Debugging
    add_compiler_flag(Debug /Gm)   # Enable minimal rebuild
    add_compiler_flag(Debug /FR)   # Create an .sbr file with complete symbolic information.
    SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
endif(CMAKE_SYSTEM MATCHES "Linux-")


# Now, convert the relative path to "devel_artifacts" into an absolute path, use that as our CMAKE_INSTALL_PREFIX
# and then set include and lib dir compile options...
#

get_filename_component(DEVEL_INSTALL_PATH_PREFIX_ABSOLUTE "../devel_artifacts" ABSOLUTE)
set(CMAKE_INSTALL_PREFIX ${DEVEL_INSTALL_PATH_PREFIX_ABSOLUTE})
include_directories(include "${DEVEL_INSTALL_PATH_PREFIX_ABSOLUTE}/include")
link_directories("${DEVEL_INSTALL_PATH_PREFIX_ABSOLUTE}/lib")

# Define our target name and build both SHARED and STATIC libs.
#

add_library(${PROJECT_NAME} SHARED ${SOURCES})
add_library(${PROJECT_NAME}S STATIC ${SOURCES})


# Add platform appropriate libraries correct targets...
#

if(CMAKE_SYSTEM MATCHES "Windows")
    target_link_libraries(${PROJECT_NAME} ${WINDOWS_LIBS} ${COMMON_LIBS})
    target_link_libraries(${PROJECT_NAME}S ${WINDOWS_LIBS} ${COMMON_LIBS})
elseif(CMAKE_SYSTEM MATCHES "Linux")
    target_link_libraries(${PROJECT_NAME} ${LINUX_LIBS} ${COMMON_LIBS})
    target_link_libraries(${PROJECT_NAME}S ${LINUX_LIBS} ${COMMON_LIBS})
endif(CMAKE_SYSTEM MATCHES "Windows")


# rpath setup
#

set(CMAKE_SKIP_BUILD_RPATH false)
set(CMAKE_BUILD_WITH_INSTALL_RPATH true)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH true)
set(CMAKE_INSTALL_RPATH "./libs" ${DEVEL_INSTALL_PATH})


# Define our installation rules
#

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION "lib"
                                ARCHIVE DESTINATION "lib"
                                RUNTIME DESTINATION "lib"
                                COMPONENT library)

# if we're on MSVC, install our .pdb files as well (for debugging)
if(MSVC)
    INSTALL(FILES ${PROJECT_BINARY_DIR}/Debug/${PROJECT_NAME}.pdb
                 DESTINATION lib
                 CONFIGURATIONS Debug)
endif(MSVC)

install(TARGETS ${PROJECT_NAME}S ARCHIVE DESTINATION "lib"
                                 COMPONENT library)

install(DIRECTORY include/${PROJECT_NAME} DESTINATION include USE_SOURCE_PERMISSIONS)
