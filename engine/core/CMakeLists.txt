#--------------------------------------------------------------------------
# core
#--------------------------------------------------------------------------

SET(files_core
	app.h
	app.cc
	camera/Camera.h
	camera/Camera.cc
	dirLight/DirLight.h
	dirLight/DirLight.cc
	graphicsNode/GraphicsNode.h
	graphicsNode/GraphicsNode.cc
	inputHandler/InputHandler.h
	inputHandler/InputHandler.cc
	lightManager/LightManager.h
	lightManager/LightManager.cc
	material/Material.h
	material/Material.cc
	material/blinnPhongMaterial/BlinnPhongMaterial.h
	material/blinnPhongMaterial/BlinnPhongMaterial.cc
	material/defaultMaterial/DefaultMaterial.h
	material/defaultMaterial/DefaultMaterial.cc
	mathlib/vec3.h
	mathlib/vec4.h
	mathlib/mat4.h
	meshResource/MeshResource.h
	meshResource/MeshResource.cc
	pointLight/PointLight.h
	pointLight/PointLight.cc
	shaderResource/ShaderResource.h
	shaderResource/ShaderResource.cc
	stb/stb_image.h
	textureResource/TextureResource.h
	textureResource/TextureResource.cc
	)
SOURCE_GROUP("core" FILES ${files_core})
	
SET(files_pch ../config.h ../config.cc)
SOURCE_GROUP("pch" FILES ${files_pch})
ADD_LIBRARY(core STATIC ${files_core} ${files_pch})
TARGET_PCH(core ../)
ADD_DEPENDENCIES(core glew)

TARGET_LINK_LIBRARIES(core PUBLIC engine exts glew)

SET_PROPERTY(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS GLEW_STATIC)
