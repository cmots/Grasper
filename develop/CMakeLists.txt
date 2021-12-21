include_directories(${PROJECT_SOURCE_DIR} ${GRASPER_EXTERNAL_INCLUDES})
 
file(GLOB develop-src-files
    swap.cpp
	)

add_library(develop-objs OBJECT ${develop-src-files})

#target_link_libraries(store-objs ${GRASPER_EXTERNAL_LIBRARIES})
