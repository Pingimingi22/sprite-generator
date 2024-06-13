cmake_minimum_required(VERSION 3.29)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(SpriteGenerator)

set(SOURCES
	"./src/Mesh.cpp"
	"./src/Source.cpp"
	"./src/OpenGL3test.cxx"
	"./fluid_ui_designs/CubeView.cxx"
	"./fluid_ui_designs/MyCubeViewUI.cxx"
)

file(GLOB HEADERS
	"./inc/Mesh.h"
	"./inc/SimpleGL3Window.h"
	"./inc/stb_image.h"
	"./inc/stb_image_write.h"
	"./fluid_ui_designs/MyCubeViewUI.h"
)

file(GLOB THIRD_PARTY_LIBRARIES 
	#"./Libraries/fltk/lib/*.lib" 
	#"./Libraries/glew/lib/Release/x64/glew32s.lib"
	)
file(GLOB THIRD_PARTY_INCLUDES 
	#"/Libraries/fltk/include/"
	#"/Libraries/glew/include/"
	#"Libraries/glm/glm/"
)

include(FetchContent)

FetchContent_Declare(
	assimp
	GIT_REPOSITORY https://github.com/assimp/assimp.git
)
FetchContent_Declare(
	fltk
	GIT_REPOSITORY https://github.com/fltk/fltk.git
)
set(FLTK_BUILD_GL ON)
set(FLTK_BUILD_FLUID OFF)
FetchContent_Declare(
	glew
	GIT_REPOSITORY https://github.com/Perlmint/glew-cmake.git
)

FetchContent_Declare(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
)

#set(GLEW_USE_STATIC_LIBS ON)
#set(glew-cmake_BUILD_SHARED OFF CACHE BOOL "Build shared GLEW library" FORCE)
#set(glew-cmake_BUILD_STATIC ON CACHE BOOL "Build static GLEW library" FORCE)

#set(BUILD_UTILS OFF CACHE BOOL "Don't build GLEW utilities" FORCE)
#set(glew-cmake_BUILD_SHARED OFF CACHE BOOL "Build shared GLEW library" FORCE)

FetchContent_MakeAvailable(fltk)
FetchContent_MakeAvailable(assimp)
FetchContent_MakeAvailable(glew)
FetchContent_MakeAvailable(glm)

target_include_directories(assimp
	PUBLIC
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>"
	"$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/FL>"
)

#target_include_directories(glew
#	PUBLIC
#	"$<BUILD_INTERFACE:${glew_SRC_DIR}/include>"
#	"$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>"
#)

#include_directories(${glew_SOURCE_DIR}/include)
#include_directories(${glew_BINARY_DIR})


include_directories(${HEADERS})
#include_directories("./Libraries/fltk/include")
#include_directories("./Libraries/glew/include")
include_directories("./Libraries/glm")
#include_directories("./Libraries/fltk")
include_directories("./inc/")
include_directories("./fluid_ui_designs/")

add_executable(DansSpriteGenerator ${SOURCES} ${HEADERS})

target_link_libraries(DansSpriteGenerator assimp)

target_link_libraries(DansSpriteGenerator ${THIRD_PARTY_LIBRARIES} 
	"opengl32.lib" 
	"ole32.lib" 
	"comctl32.lib" 
	"ws2_32.lib"
	"gdiplus.lib"
	"glu32.lib")
target_link_libraries(DansSpriteGenerator fltk fltk_gl libglew_static glm::glm-header-only)




set(DLL_SOURCE "../Libraries/assimp/lib/Debug/assimp-vc142-mtd.dll")
set(DLL_DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")

#add_custom_command(TARGET DansSpriteGenerator POST_BUILD
	#COMMAND ${CMAKE_COMMAND} -E copy_if_different
	#"${DLL_SOURCE}" "${DLL_DESTINATION}/Debug"
	#"../Models/Pirate_Desk_01.fbx" "${CMAKE_CURRENT_BINARY_DIR}/Debug"
	#"../Models/stanfordbunny.obj" "${CMAKE_CURRENT_BINARY_DIR}/Debug"
	#"../Models/Textures/colorchart.png" "${CMAKE_CURRENT_BINARY_DIR}/Debug"
	#"../Models/Textures/lambert1_Base_color.png" "${CMAKE_CURRENT_BINARY_DIR}/Debug"
	#"../Models/Textures/lambert2_Base_color.png" "${CMAKE_CURRENT_BINARY_DIR}/Debug"
	#"../Models/Textures/lambert3_Base_color.png" "${CMAKE_CURRENT_BINARY_DIR}/Debug"
#)

add_custom_command(TARGET DansSpriteGenerator POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${assimp_BINARY_DIR}/bin/Debug" 
	$<TARGET_FILE_DIR:DansSpriteGenerator>
	COMMENT "Copying DLL to output directory"
)

