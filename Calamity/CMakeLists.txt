project(Calamity)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_FLAGS "-Wall -std=c++0x")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/Modules/")

#SDL2 plus dependencies
set(SDL2
	#/usr/local/lib/libSDL2.a
	/usr/lib64/libSDL2.so
	#/usr/local/lib/libSDL2_image.a
	/usr/lib64/libSDL2_image.so
	pthread
	dl
	rt
	m
)

set(BOX2D
	${PROJECT_SOURCE_DIR}/libs/libBox2D.a
)

set(ASSIMP
	${PROJECT_SOURCE_DIR}/libs/libassimp.so
)

set(BULLET
	${PROJECT_SOURCE_DIR}/libs/libBulletSoftBody.a
	${PROJECT_SOURCE_DIR}/libs/libBulletDynamics.a
	${PROJECT_SOURCE_DIR}/libs/libBulletCollision.a
	${PROJECT_SOURCE_DIR}/libs/libLinearMath.a
)

#set(BULLET
#	${PROJECT_SOURCE_DIR}/libs/bullet_new/libBulletSoftBody_gmake_x64_release.a
#	${PROJECT_SOURCE_DIR}/libs/bullet_new/libBulletDynamics_gmake_x64_release.a
#	${PROJECT_SOURCE_DIR}/libs/bullet_new/libBulletCollision_gmake_x64_release.a
#	${PROJECT_SOURCE_DIR}/libs/bullet_new/libLinearMath_gmake_x64_release.a
#)

find_package(OpenGL)
find_package(GLEW)

include_directories("${PROJECT_SOURCE_DIR}/include/")
include_directories(${OPENGL_INCLUDE_DIR})
include_directories("/usr/include/freetype2")
include_directories("${PROJECT_SOURCE_DIR}/include/bullet/")
#include_directories("${PROJECT_SOURCE_DIR}/include/bullet_new/")

set(SRC_LIST
	Utility.cpp
	Functor.h
	Shader/Shader.cpp
	Shader/SimpleShader.cpp
	Shader/SpriteShader.cpp
	Shader/TextureShader.cpp
	Shader/ColorShader.cpp
	Shader/RadialBlurShader.cpp
	Shader/FontShader.cpp
	Shader/MeshShader.cpp
	Shader/CharacterMeshShader.cpp
	Shader/NullShader.cpp
	Shader/DeferredShader.cpp
	Shader/Color3DShader.cpp
	TextureAtlas.cpp
	TextureManager.cpp
	Renderer/Renderer.cpp
	Renderer/RenderLayer.cpp
	Math/Vec2.cpp
	Math/Vec3.cpp
	FboManager.cpp
	SpriteManager.cpp
	FontManager.cpp
	#GameManager.cpp
	#LayerManager.cpp
	#BoneManager.cpp
	#SharedTriangleData.cpp
	#TriangleObject.cpp
	#SpriteObject.cpp
	#BoneObject.cpp
	Action.h
	ActionManager.cpp
	Gui/ActionHandler/ScaleWindowHandler.cpp
	Gui/ActionHandler/MoveWindowHandler.cpp
	Gui/ActionHandler/ScaleVerticalBoxHandler.cpp
	Gui/ActionHandler/SegmentControlHandler.cpp
	Gui/ActionHandler/ButtonHandler.cpp
	Gui/ActionHandler/InputHandler.cpp
	Gui/ActionHandler/LineEditHandler.cpp
	Gui/Widget.h
	Gui/Window.cpp
	Gui/WindowManager.cpp
	Gui/VerticalBoxLayout.cpp
	Gui/VerticalLayout.cpp
	Gui/HorizontalLayout.cpp
	Gui/ContextTabs.cpp
	Gui/TestWidget.cpp
	Gui/SegmentControl.cpp
	Gui/ButtonGroup.cpp
	Gui/InputGroup.cpp
	Gui/LineEdit.cpp
	Gui/Checkbox.cpp
	Gui/ListView.cpp
	Gui/TreeList.cpp
	#Gui/LayerItem.cpp
	Context.h
	ContextManager.cpp
	Properties.cpp
	Core.cpp
	Calamity.h
)

add_library(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(
	${PROJECT_NAME}
	${SDL2}
	${OPENGL_gl_LIBRARY}
	${GLEW_LIBRARY}
	${BOX2D}
	${ASSIMP}
	${BULLET}
	png
	freetype
)
