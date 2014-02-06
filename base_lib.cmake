

# First, append our compile options...
#

if(CMAKE_SYSTEM MATCHES "Linux-")
    set(CMAKE_CXX_FLAGS_RELEASE "-std=c++11 -O3 -DNDEBUG -fthreadsafe-statics -fPIC ${CUSTOM_FLAGS}")
    set(CMAKE_CXX_FLAGS_DEBUG "-std=c++11 -ggdb -fthreadsafe-statics -fPIC ${CUSTOM_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS -rdynamic)
    add_definitions(-D_LINUX)
    add_definitions(-DLINUX_OS)
    add_definitions(-D_REENTRANT)
elseif(CMAKE_SYSTEM MATCHES "Windows")
    add_definitions(-DWIN32)
    add_definitions(-D_USE_32BIT_TIME_T)
    add_definitions(-DUNICODE)
    add_definitions(-D_UNICODE)
    add_definitions(-DNOMINMAX)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
    add_definitions(-D__inline__=__inline)
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
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
    add_compiler_flag(Both /GF)    # Enable read-only string pooling.
    add_compiler_flag(Both /EHsc)  # Make sure that destructors get executed when exceptions exit C++ code.
    add_compiler_flag(Debug /ZI)   # Generate pdb files which support Edit and Continue Debugging
    add_compiler_flag(Debug /Gm)   # Enable minimal rebuild
    add_compiler_flag(Debug /FR)   # Create an .sbr file with complete symbolic information.
    SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)
endif(CMAKE_SYSTEM MATCHES "Linux-")


# Now, setup our artifact install root and add our default header and lib paths.
#

set(DEVEL_INSTALL_PATH "../../devel_artifacts")
set(CMAKE_INSTALL_PREFIX ${DEVEL_INSTALL_PATH})
include_directories(include ${DEVEL_INSTALL_PATH}/include)
get_filename_component(ABSOLUTE_LIB_DIR ${DEVEL_INSTALL_PATH}/lib ABSOLUTE)
link_directories(${ABSOLUTE_LIB_DIR})


# Define our target name and build both SHARED and STATIC libs.
#

set(SHARED_TARGET_NAME ${PROJECT_NAME})
set(STATIC_TARGET_NAME ${PROJECT_NAME}S)

add_library(${SHARED_TARGET_NAME} SHARED ${SOURCES})
add_library(${STATIC_TARGET_NAME} STATIC ${SOURCES})


# Add platform appropriate libraries correct targets...
#

if(CMAKE_SYSTEM MATCHES "Windows")
    target_link_libraries(${SHARED_TARGET_NAME} ${WINDOWS_LIBS} ${COMMON_LIBS})
    target_link_libraries(${STATIC_TARGET_NAME} ${WINDOWS_LIBS} ${COMMON_LIBS})
elseif(CMAKE_SYSTEM MATCHES "Linux")
    target_link_libraries(${SHARED_TARGET_NAME} ${LINUX_LIBS} ${COMMON_LIBS})
    target_link_libraries(${STATIC_TARGET_NAME} ${LINUX_LIBS} ${COMMON_LIBS})
endif(CMAKE_SYSTEM MATCHES "Windows")


# rpath setup
#

set(CMAKE_SKIP_BUILD_RPATH false)
set(CMAKE_BUILD_WITH_INSTALL_RPATH true)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH true)
set(CMAKE_INSTALL_RPATH "./libs" ${DEVEL_INSTALL_PATH})


# Define our installation rules
#

install(TARGETS ${SHARED_TARGET_NAME} LIBRARY DESTINATION "lib"
                                      ARCHIVE DESTINATION "lib"
                                      RUNTIME DESTINATION "lib"
                                      COMPONENT library)

install(TARGETS ${STATIC_TARGET_NAME} ARCHIVE DESTINATION "lib"
                                      COMPONENT library)

install(DIRECTORY include/${PROJECT_NAME} DESTINATION include)
