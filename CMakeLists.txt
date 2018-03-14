cmake_minimum_required(VERSION 3.1)

project(lua_rapidjson CXX)

set(CMAKE_CXX_STANDARD 11)

option(BUILD_SHARED_LIBS "Build shared libraries" ON)

set(SOURCES	src/rapidjson.cpp)

include_directories(
	"${CMAKE_SOURCE_DIR}/lua-prebuilt/lua-5.3/include"
	"${CMAKE_SOURCE_DIR}/${LUA_LIB_DIR}"
  	"${CMAKE_SOURCE_DIR}/rapidjson/include")
link_directories("${CMAKE_SOURCE_DIR}/${LUA_LIB_DIR}")

if(WIN32)
	add_definitions(-DLUA_BUILD_AS_DLL -DLUA_LIB)
endif()

add_library(librapidjson ${SOURCES})
target_link_libraries(librapidjson lua)
set_target_properties(librapidjson PROPERTIES OUTPUT_NAME rapidjson CLEAN_DIRECT_OUTPUT 1)

if(IPHONEOS_DEPLOYMENT_TARGET)
	set_xcode_property(librapidjson IPHONEOS_DEPLOYMENT_TARGET ${IPHONEOS_DEPLOYMENT_TARGET})
endif()

# install
set(LUA_VDIR "5.3")
install(TARGETS librapidjson DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/lua/${LUA_VDIR})
