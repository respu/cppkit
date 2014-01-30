cmake_minimum_required(VERSION 2.8)
project(ut)

if(CMAKE_SYSTEM MATCHES "Linux-")
set(CUSTOM_FLAGS "-std=c++11 -DLINUX_OS -fPIC")
endif(CMAKE_SYSTEM MATCHES "Linux-")

if(CMAKE_SYSTEM MATCHES "Windows")
set(CUSTOM_FLAGS "")
endif(CMAKE_SYSTEM MATCHES "Windows")

set(CMAKE_CXX_FLAGS ${CUSTOM_FLAGS})
set(CMAKE_SHARED_LINKER_FLAGS "-rdynamic")

include_directories(include)

add_executable(ut source/framework.cpp
                  source/chrono_test.cpp
                  source/ck_byte_ptr_test.cpp
                  source/ck_large_files_test.cpp
                  source/ck_memory_test.cpp
                  source/ck_random_test.cpp
                  source/ck_socket_address_test.cpp
                  source/ck_socket_test.cpp
                  source/ck_string_test.cpp
                  source/condition_variable_test.cpp
                  source/recursive_mutex_test.cpp
                  source/ck_variant_test.cpp
                  source/ck_json_test.cpp
                  source/ck_nullable_test.cpp
                  source/ck_dynamic_library_test.cpp)

target_link_libraries(ut cppkit)
