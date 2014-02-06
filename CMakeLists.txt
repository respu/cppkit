cmake_minimum_required(VERSION 2.8)
project(cppkit)

set(SOURCES source/ck_library_id.cpp
            source/ck_byte_ptr.cpp
            source/ck_error_msg.cpp
            source/ck_exception.cpp
            source/ck_large_files.cpp
            source/ck_logger.cpp
            source/ck_memory.cpp
            source/ck_socket_address.cpp
            source/ck_socket.cpp
            source/ck_sprintf.cpp
            source/ck_stack_trace.cpp
            source/ck_string.cpp
            source/ck_time_utils.cpp
            source/ck_uuid_utils.cpp
            source/ck_variant.cpp
            source/ck_json.cpp
            source/ck_nullable.cpp
            source/ck_dynamic_library.cpp
            source/ck_actor.cpp
            source/ck_command_queue.cpp
            source/ck_memory_map.cpp
            source/ck_path.cpp
            source/ck_udp_sender.cpp
            source/ck_udp_receiver.cpp)

set(WINDOWS_LIBS DbgHelp ws2_32 iphlpapi)
set(LINUX_LIBS pthread rt dl)
#set(COMMON_LIBS "")

# common stuff...


# First, append our compile options...
#

if(CMAKE_SYSTEM MATCHES "Linux-")
    set(CUSTOM_FLAGS ${CUSTOM_FLAGS} "-std=c++11 -DLINUX_OS -fPIC")
    set(CMAKE_SHARED_LINKER_FLAGS "-rdynamic")
elseif(CMAKE_SYSTEM MATCHES "Windows")
    set(CUSTOM_FLAGS ${CUSTOM_FLAGS} "")
endif(CMAKE_SYSTEM MATCHES "Linux-")
string(REPLACE ';' ' ' CUSTOM_FLAGS ${CUSTOM_FLAGS})
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${CUSTOM_FLAGS})


# Now, setup our artifact install root
#

set(DEVEL_INSTALL_PATH "../../devel_artifacts")
set(CMAKE_INSTALL_PREFIX ${DEVEL_INSTALL_PATH})
include_directories(include ${DEVEL_INSTALL_PATH})


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
set(CMAKE_INSTALL_RPATH "./Libs" ${DEVEL_INSTALL_PATH})


# Define our installation rules
#

install(TARGETS ${SHARED_TARGET_NAME} LIBRARY DESTINATION "lib"
                                      ARCHIVE DESTINATION "lib"
                                      RUNTIME DESTINATION "lib"
                                      COMPONENT library)

install(TARGETS ${STATIC_TARGET_NAME} ARCHIVE DESTINATION "lib"
                                      COMPONENT library)

install(DIRECTORY include/${PROJECT_NAME} DESTINATION include)
