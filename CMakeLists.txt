cmake_minimum_required(VERSION 2.8)
project(cppkit)

if(CMAKE_SYSTEM MATCHES "Linux-")
set(CUSTOM_FLAGS "-std=c++11 -DLINUX_OS -fPIC")
endif(CMAKE_SYSTEM MATCHES "Linux-")

if(CMAKE_SYSTEM MATCHES "Windows")
set(CUSTOM_FLAGS "")
endif(CMAKE_SYSTEM MATCHES "Windows")

set(CMAKE_CXX_FLAGS ${CUSTOM_FLAGS})
set(CMAKE_SHARED_LINKER_FLAGS "-rdynamic")

include_directories(include)
add_library(cppkit SHARED source/ck_library_id.cpp
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
                          source/ck_memory_map.cpp)

if(${UNIX})
target_link_libraries(cppkit pthread rt dl)
endif(${UNIX})

install(TARGETS cppkit LIBRARY DESTINATION lib COMPONENT library)
install(DIRECTORY include/${PROJECT_NAME} DESTINATION include)
