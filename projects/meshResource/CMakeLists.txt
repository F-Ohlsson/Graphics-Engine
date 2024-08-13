#--------------------------------------------------------------------------
# MeshResource project
#--------------------------------------------------------------------------

PROJECT(MeshResource)
FILE(GLOB meshResource_headers code/*.h)
FILE(GLOB meshResource_sources code/*.cc)

SET(files_meshResource ${meshResource_headers} ${meshResource_sources})
SOURCE_GROUP("meshResource" FILES ${files_meshResource})

ADD_EXECUTABLE(MeshResource ${files_meshResource})
TARGET_LINK_LIBRARIES(MeshResource core render)
ADD_DEPENDENCIES(MeshResource core render)