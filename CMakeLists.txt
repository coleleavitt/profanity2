cmake_minimum_required(VERSION 3.16)
project(profanity2 CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenCL REQUIRED)

add_executable(profanity2.x64
    Dispatcher.cpp
    Mode.cpp
    precomp.cpp
    profanity.cpp
    SpeedSample.cpp
)

target_link_libraries(profanity2.x64 PRIVATE OpenCL::OpenCL)

target_compile_options(profanity2.x64 PRIVATE -Wall -mmmx -O2)

# precomp.cpp's tables overflow the default small code model
if(NOT APPLE)
    target_compile_options(profanity2.x64 PRIVATE -mcmodel=large)
    target_link_options(profanity2.x64 PRIVATE -mcmodel=large)
endif()

# The binary loads its OpenCL kernels from the working directory at runtime
foreach(kernel profanity.cl keccak.cl)
    add_custom_command(TARGET profanity2.x64 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${CMAKE_CURRENT_SOURCE_DIR}/${kernel}
                $<TARGET_FILE_DIR:profanity2.x64>/${kernel})
endforeach()
