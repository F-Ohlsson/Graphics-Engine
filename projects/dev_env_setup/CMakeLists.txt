#--------------------------------------------------------------------------
# dev_env_setup project
#--------------------------------------------------------------------------

PROJECT(dev_env_setup)
FILE(GLOB example_headers code/*.h)
FILE(GLOB example_sources code/*.cc)

SET(files_example ${example_headers} ${example_sources})
SOURCE_GROUP("example" FILES ${files_example})

ADD_EXECUTABLE(dev_env_setup ${files_example})
TARGET_LINK_LIBRARIES(dev_env_setup core render)
ADD_DEPENDENCIES(dev_env_setup core render)