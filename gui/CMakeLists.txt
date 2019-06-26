cmake_minimum_required(VERSION 3.9)

# list the header files
set(HEADER_FILES
	include/gui/mainwindow.h
)

# list the source files
set(SOURCE_FILES
	src/gui/mainwindow.cpp
	src/gui/main.cpp
)

# list ui files
set(UI_FILES
	ui/mainwindow.ui
)

# list resources files
set(RESOURCES_FILES
	resources/resources.qrc
)


# link with qt5
find_package(Qt5 COMPONENTS Widgets REQUIRED)
qt5_wrap_cpp(UI_HEADER_MOC ${HEADER_FILES})
qt5_wrap_ui(UI_FILES_MOC ${UI_FILES})
qt5_add_resources(RCC_SOURCES ${RESOURCES_FILES})


# add library target
add_executable(gui
	${HEADER_FILES}
	${SOURCE_FILES}
	${UI_FILES}
	${UI_HEADER_MOC}
	${UI_FILES_MOC}
	${RCC_SOURCES}
)


# make it reflect the same structure as the one on disk
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${HEADER_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${SOURCE_FILES})
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${UI_FILES})

target_link_libraries(gui
	PUBLIC
		Qt5::Widgets
		Immagine
)

# Windows specfic flags to enable W variations of winapi
if(WIN32)
	target_compile_definitions(gui
		PRIVATE
			OS_WINDOWS=1
	)
	target_compile_options(gui
		PRIVATE
			/WX
	)
endif()


# enable C++17
# disable any compiler specifc extensions
# add d suffix in debug mode
target_compile_features(gui PUBLIC cxx_std_17)
set_target_properties(gui PROPERTIES
	CXX_EXTENSIONS OFF
)

# define debug macro
target_compile_definitions(gui PRIVATE "$<$<CONFIG:DEBUG>:DEBUG>")

# list include directories
target_include_directories(gui
	PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
	${PROJECT_BINARY_DIR}
)
