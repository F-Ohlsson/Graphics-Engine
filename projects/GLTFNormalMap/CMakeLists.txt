#--------------------------------------------------------------------------
# GLTFNormalMap project
#--------------------------------------------------------------------------

PROJECT(GLTFNormalMap)
FILE(GLOB example_headers code/*.h)
FILE(GLOB example_sources code/*.cc)

SET(files_example ${example_headers} ${example_sources})
SOURCE_GROUP("example" FILES ${files_example})

ADD_EXECUTABLE(GLTFNormalMap ${files_example})
TARGET_LINK_LIBRARIES(GLTFNormalMap core render)
ADD_DEPENDENCIES(GLTFNormalMap core render)

if (MSVC)
	set_property(TARGET GLTFNormalMap PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
endif()