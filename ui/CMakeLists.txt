cmake_minimum_required(VERSION 3.9)

# enable C++17
# disable any compiler specifc extensions
# add d suffix in debug mode
target_compile_features(ui PUBLIC cxx_std_17)
set_target_properties(ui PROPERTIES
	CXX_EXTENSIONS OFF
	DEBUG_POSTFIX d
)

# define debug macro
target_compile_definitions(ui PRIVATE "$<$<CONFIG:DEBUG>:DEBUG>")

# generate exports header file
include(GenerateExportHeader)
generate_export_header(ui
	EXPORT_FILE_NAME ${CMAKE_CURRENT_SOURCE_DIR}/include/ui/Exports.h
)

# list include directories
target_include_directories(ui
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)
