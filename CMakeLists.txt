# STUDENT CMakeLists.txt
#
# tabs: 8 space

# CMake initialization

cmake_minimum_required(VERSION 3.20)
project("ProjetC_IG")

# Paths

get_filename_component(ROOT_DIR		${CMAKE_CURRENT_LIST_FILE} PATH)
set(HEADER_DIRS				"${ROOT_DIR}/api")
set(TEST_DIR				"${ROOT_DIR}/tests")
set(DOCS_DIR				"${ROOT_DIR}/docs")
set(SRC_DIR				"${ROOT_DIR}/implem")

set(LIB_FLAGS				-lfreeimage -lSDL2 -lSDL2_ttf)

if(IG_ASAN)
	set(CMAKE_C_FLAGS		"${CMAKE_C_FLAGS} -fsanitize=address")
endif(IG_ASAN)

# Platform specific definitions

if(${APPLE})
	set(HEADER_DIRS			${HEADER_DIRS}
					"/opt/local/include/SDL2"
					"/opt/local/include")
	set(PLATFORM_DIR		"${ROOT_DIR}/_macos")
	set(PLATFORM_LIB_FLAGS		eibase -L/opt/local/lib ${LIB_FLAGS})

	message(STATUS "Building for MacOS")

elseif(${UNIX})

	set(HEADER_DIRS			${HEADER_DIRS}
					"/usr/include/SDL2")
	set(PLATFORM_DIR		"${ROOT_DIR}/_x11")
	set(PLATFORM_LIB_FLAGS		eibase ${LIB_FLAGS} -lm)

	message(STATUS "Building for Linux")

else()

	set(SDL				"C:/projetc/SDL2_windows")
	set(HEADER_DIRS			${HEADER_DIRS}
					"${SDL}/include")
	add_definitions			(-D_AMD64_=1 -D__WIN__=1 -D_CRT_SECURE_NO_WARNINGS=1 /wd4068)

	set(PLATFORM_DIR		"${ROOT_DIR}/_win")
	set(PLATFORM_LIB_FLAGS		eibase
					${SDL}/lib/SDL2.lib
					${SDL}/lib/SDL2_ttf.lib
					${SDL}/lib/FreeImage.lib)

	message(STATUS "Building for Windows")

endif(${APPLE})

# General definitions for all targets

include_directories(${HEADER_DIRS})
link_directories(${PLATFORM_DIR})


# target ei (libei)

set(LIB_EI_SOURCES
    ${SRC_DIR}/primitive_graphique.c
)
add_library(ei STATIC			${LIB_EI_SOURCES})

# target minimal

add_executable(minimal 			${TEST_DIR}/minimal.c)
target_link_libraries(minimal		ei ${PLATFORM_LIB_FLAGS})

# target lines

add_executable(lines			${TEST_DIR}/lines.c)
target_link_libraries(lines		ei ${PLATFORM_LIB_FLAGS})

# target frame

add_executable(frame			${TEST_DIR}/frame.c)
target_link_libraries(frame		ei ${PLATFORM_LIB_FLAGS})

# target button

add_executable(button			${TEST_DIR}/button.c)
target_link_libraries(button		ei ${PLATFORM_LIB_FLAGS})

# target hello_world

add_executable(hello_world		${TEST_DIR}/hello_world.c)
target_link_libraries(hello_world	ei ${PLATFORM_LIB_FLAGS})

# target puzzle

add_executable(puzzle			${TEST_DIR}/puzzle.c)
target_link_libraries(puzzle		ei ${PLATFORM_LIB_FLAGS})

# target two048

add_executable(two048			${TEST_DIR}/two048.c)
target_link_libraries(two048		ei ${PLATFORM_LIB_FLAGS})

# target minesweeper

add_executable(minesweeper		${TEST_DIR}/minesweeper.c)
target_link_libraries(minesweeper	ei ${PLATFORM_LIB_FLAGS})

# target ext_testclass

add_executable(ext_testclass		${TEST_DIR}/ext_testclass.c)
target_link_libraries(ext_testclass	testclass ei ${PLATFORM_LIB_FLAGS})


# target to build the documentation

add_custom_target(doc doxygen		${DOCS_DIR}/doxygen.cfg WORKING_DIRECTORY ${ROOT_DIR})

# all

add_custom_target(ig_all ALL)
add_dependencies(ig_all minimal frame button hello_world puzzle two048 minesweeper)
