cmake_minimum_required(VERSION 3.9)

# list the header files
set(HEADER_FILES
	include/Immagine/Exports.h
	include/Immagine/Image.h
	include/Immagine/Imagedef.h
	include/Immagine/Kernel.h
	include/Immagine/Point_Processing.h
)

# list the source files
set(SOURCE_FILES
	src/Immagine/Image.cpp
	src/Immagine/Kernel.cpp
	src/Immagine/Point_Processing.cpp
)


# add library target
add_library(Immagine
	${HEADER_FILES}
	${SOURCE_FILES}
)

# make it reflect the same structure as the one on disk
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADER_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})

target_include_directories(Immagine
	PRIVATE
	${CMAKE_CURRENT_SOURCE_DIR}/../external/stb
)

# Windows specfic flags to enable W variations of winapi
if(WIN32)
	target_compile_definitions(Immagine
		PRIVATE
			OS_WINDOWS=1
	)
endif()


# enable C++17
# disable any compiler specifc extensions
# add d suffix in debug mode
target_compile_features(Immagine PUBLIC cxx_std_17)
set_target_properties(Immagine PROPERTIES
	CXX_EXTENSIONS OFF
	DEBUG_POSTFIX d
)

# define debug macro
target_compile_definitions(Immagine PRIVATE "$<$<CONFIG:DEBUG>:DEBUG>")

# generate exports header file
include(GenerateExportHeader)
generate_export_header(Immagine
	EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/include/Immagine/Exports.h
)

# list include directories
target_include_directories(Immagine
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)