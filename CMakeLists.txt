project(htmlcxx)
cmake_minimum_required(VERSION 2.8)
aux_source_directory(html HTML)
aux_source_directory(css CSS)
list(APPEND FULL_SRC
	${HTML})

if(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /EHa")
else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")
endif()

add_library(${PROJECT_NAME} ${FULL_SRC})

install(DIRECTORY html
 DESTINATION include/${PROJECT_NAME}
 FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp" PATTERN "*.tcc"
)
INSTALL(TARGETS ${PROJECT_NAME} DESTINATION "lib")
