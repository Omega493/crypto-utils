cmake_minimum_required(VERSION 3.16)
project("CryptoUtils")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(encryptor "encryptor.cpp" "utilities/get_secret_input.hpp" "utilities/get_secret_input.cpp"
    "utilities/exception.hpp" "include/cxxopts.hpp" "src/encrypt.hpp" "src/decrypt.hpp" "src/encrypt.cpp" "src/decrypt.cpp")

target_include_directories(encryptor PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/utilities
)

if(WIN32)
    if(CMAKE_SIZEOF_VOID_P EQUAL 8) 
        target_compile_definitions(encryptor PRIVATE ARCH_64BIT) 
        message(STATUS "Targeting 64-bit architecture") 
    else() 
        target_compile_definitions(encryptor PRIVATE ARCH_32BIT) 
        message(STATUS "Targeting 32-bit architecture") 
    endif() 

    find_package(unofficial-sodium CONFIG REQUIRED)
    target_link_libraries(encryptor PRIVATE unofficial-sodium::sodium)
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SODIUM REQUIRED libsodium)

    target_include_directories(encryptor PRIVATE ${SODIUM_INCLUDE_DIRS})

    target_link_libraries(encryptor PRIVATE ${SODIUM_LIBRARIES})
endif()