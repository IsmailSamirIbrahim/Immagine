cmake_minimum_required(VERSION 3.9)

# list the source files
set(SOURCE_FILES
	src/playground.cpp
)


# add library target
add_executable(playground
	${SOURCE_FILES}
)

# make it reflect the same structure as the one on disk
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADER_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})

target_link_libraries(playground
	PRIVATE
		Immagine
)

# Windows specfic flags to enable W variations of winapi
if(WIN32)
	target_compile_definitions(playground
		PRIVATE
			OS_WINDOWS=1
	)
elseif(UNIX)
	target_compile_definitions(playground
		PRIVATE
			OS_LINUX=1
	)
endif()


# enable C++17
# disable any compiler specifc extensions
# add d suffix in debug mode
target_compile_features(playground PUBLIC cxx_std_17)
set_target_properties(playground PROPERTIES
	CXX_EXTENSIONS OFF
)

find_package(OpenMP)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

# define debug macro
target_compile_definitions(playground PRIVATE "$<$<CONFIG:DEBUG>:DEBUG>")

# list include directories
target_include_directories(playground
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_compile_definitions(playground
	PRIVATE
		IMAGE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/../resources"
)