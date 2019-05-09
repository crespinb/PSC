cmake_minimum_required(VERSION 2.8)

project(serverManager)

set(TARGET_NAME server)

add_executable(${TARGET_NAME}
	test.cpp
	serverManager.cpp
	headtrackerManager.cpp)

target_link_libraries(${TARGET_NAME} PUBLIC ws2_32)

find_library(HEDROT_LIB hedrot HINTS .)
find_library(LAPACKE_LIB lapacke HINTS .)
find_library(LAPACK_LIB lapack HINTS .)
find_library(BLAS_LIB blas HINTS .)
target_link_libraries(${TARGET_NAME} PUBLIC ${HEDROT_LIB})
target_link_libraries(${TARGET_NAME} PUBLIC ${LAPACKE_LIB})
target_link_libraries(${TARGET_NAME} PUBLIC ${LAPACK_LIB})
target_link_libraries(${TARGET_NAME} PUBLIC ${BLAS_LIB})

find_library(JUCE_LIB juce_dll HINTS .)
target_link_libraries(${TARGET_NAME} PUBLIC ${JUCE_LIB})

target_include_directories(serverDemo PUBLIC include)