cmake_minimum_required(VERSION 3.10)

# Set some basic project attributes
project (test-thread-pinning
	VERSION 0.1
	DESCRIPTION "A small project to test thread pinning"
	LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSSL REQUIRED)
find_package(OpenMP REQUIRED)

include_directories(${OPENSSL_INCLUDE_DIR})

add_executable(${PROJECT_NAME} test-thread-pinning.cpp)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fopenmp -O3 -flto")

# Create a simple configuration header
configure_file(config.h.in config.h)

# Include the configuration header in the build
target_include_directories(${PROJECT_NAME} PUBLIC "${PROJECT_BINARY_DIR}")

target_link_libraries(test-thread-pinning ${OPENSSL_LIBRARIES} OpenMP::OpenMP_CXX)