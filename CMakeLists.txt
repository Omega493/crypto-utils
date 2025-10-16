cmake_minimum_required(VERSION 3.16)
project("CryptoUtils")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(encryptor "encryptor.cpp" "get_secret_input.h" "get_secret_input.cpp")
add_executable(decryptor "decryptor.cpp" "get_secret_input.h" "get_secret_input.cpp")

if(WIN32)
    find_package(unofficial-sodium REQUIRED)

    if(NOT TARGET sodium::sodium)
        add_library(sodium::sodium ALIAS unofficial-sodium::sodium)
    endif()

    target_link_libraries(encryptor PRIVATE sodium::sodium)
    target_link_libraries(decryptor PRIVATE sodium::sodium)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SODIUM REQUIRED libsodium)

    target_include_directories(encryptor PRIVATE ${SODIUM_INCLUDE_DIRS})
    target_include_directories(decryptor PRIVATE ${SODIUM_INCLUDE_DIRS})

    target_link_libraries(encryptor PRIVATE ${SODIUM_LIBRARIES})
    target_link_libraries(decryptor PRIVATE ${SODIUM_LIBRARIES})
endif()