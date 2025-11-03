cmake_minimum_required(VERSION 3.16)
project("CryptoUtils")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(encryptor "encryptor.cpp" "utilities/get_secret_input.h" "utilities/get_secret_input.cpp"
    "utilities/exception.h" "include/cxxopts.hpp" "src/encrypt.hpp" "src/decrypt.hpp" "src/encrypt.cpp" "src/decrypt.cpp")

target_include_directories(encryptor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

if(WIN32)
    find_package(unofficial-sodium REQUIRED)

    if(NOT TARGET sodium::sodium)
        add_library(sodium::sodium ALIAS unofficial-sodium::sodium)
    endif()

    target_link_libraries(encryptor PRIVATE sodium::sodium)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SODIUM REQUIRED libsodium)

    target_include_directories(encryptor PRIVATE ${SODIUM_INCLUDE_DIRS})

    target_link_libraries(encryptor PRIVATE ${SODIUM_LIBRARIES})
endif()