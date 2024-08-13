#--------------------------------------------------------------------------
# shadersGraphicsNodesInput project
#--------------------------------------------------------------------------

PROJECT(shadersGraphicsNodesInput)
FILE(GLOB example_headers code/*.h)
FILE(GLOB example_sources code/*.cc)

SET(files_example ${example_headers} ${example_sources})
SOURCE_GROUP("example" FILES ${files_example})

ADD_EXECUTABLE(shadersGraphicsNodesInput ${files_example})
TARGET_LINK_LIBRARIES(shadersGraphicsNodesInput core render)
ADD_DEPENDENCIES(shadersGraphicsNodesInput core render)

if (MSVC)
	set_property(TARGET shadersGraphicsNodesInput PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
endif()