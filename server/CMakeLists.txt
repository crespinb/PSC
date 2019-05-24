cmake_minimum_required(VERSION 2.8)

project(serverManager)

set(TARGET_NAME server)

add_executable(${TARGET_NAME}
	test.cpp
	serverManager.cpp
	headtrackerManager.cpp
	)#audioManager.cpp) #linker error fix

find_library(HEDROT_LIB hedrot HINTS .)
find_library(LAPACKE_LIB lapacke HINTS .)
find_library(LAPACK_LIB lapack HINTS .)
find_library(BLAS_LIB blas HINTS .)

find_library(SOUNDFILE_LIB soundfile HINTS .)

find_library(FFTW_LIB fftw3-3 HINTS .)

target_link_libraries(${TARGET_NAME} PUBLIC
	${HEDROT_LIB}
	${LAPACKE_LIB}
	${LAPACK_LIB}
	${BLAS_LIB}
	${SOUNDFILE_LIB}
	${FFTW_LIB}
	ws2_32)

target_include_directories(${TARGET_NAME} PUBLIC
	include
	sigpack
	soundfile)