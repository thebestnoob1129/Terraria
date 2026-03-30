cmake_minimum_required(VERSION 4.2)
project(fastNoiceSIMD)

file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/FastNoiseSIMD/*.cpp")

add_library(fastNoiseSIMD)

# Seperate the AVX2 file and gite it special compile flag
foreach(SRC IN LISTS MY_SOURCES)
	if(SRC MATCHES "FastNoiseSIMD_avx2.cpp$")
		set(AVX2_SRC ${SRC})
		list(REMOVE_ITEM MY_SOURCES ${SRC})
	endif()
	if(SRC MATCHES "FastNoiseSIMD_avx512.cpp$")
		set(AVX512_SRC ${SRC})
		list(REMOVE_ITEM MY_SOURCES ${SRC})
	endif()
endforeach()

target_sources(fastNoiseSIMD PRIVATE ${MY_SOURCES})

if (MSVC)
	set_source_files_properties(${AVX2_SRC} PROPERTIES COMPILE_FLAGS "/arch:AVX2")
	set_source_files_properties(${AVX512_SRC} PROPERTIES COMPILE_FLAGS "/arch:AVX512")
else()
	set_source_files_properties(${AVX2_SRC} PROPERTIES COMPILE_FLAGS "-mavx2")
	set_source_files_properties(${AVX512_SRC} PROPERTIES COMPILE_FLAGS "-mavx512f")
endif()

target_sources(fastNoiseSIMD PRIVATE ${AVX2_SRC} ${AVX512_SRC})
target_include_directories(fastNoiseSIMD PUBLIC "FastNoiseSIMD")