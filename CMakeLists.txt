cmake_minimum_required(VERSION 3.16)

project(AdvPT VERSION 1.0
              DESCRIPTION "Tensors"
              LANGUAGES CXX)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/data
        DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

add_executable(test_tensor test_tensor.cpp)
target_compile_features(test_tensor PRIVATE cxx_std_20)
target_compile_options(test_tensor PRIVATE -Wall -pg -g) #removed -Wextra -pedantic -Werror as they treat warnings as errors to enforce "clean code" so it can be fixed later
target_link_options(test_tensor PRIVATE -pg)

add_executable(test_matvec test_matvec.cpp)
target_compile_features(test_matvec PRIVATE cxx_std_20)
target_compile_options(test_matvec PRIVATE -Wall -pg -g) #removed -Wextra -pedantic -Werror as they treat warnings as errors to enforce "clean code" so it can be fixed later
target_link_options(test_matvec PRIVATE -pg)
# commented the stuff above as they are meant for matvec which will be worked on later