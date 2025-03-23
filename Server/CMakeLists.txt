project(Server)

add_executable(Server src/main.cpp)

target_include_directories(Server PUBLIC 
						   ${CMAKE_SOURCE_DIR}/include/
						   ${CMAKE_SOURCE_DIR}/Server/include/ )

install(TARGETS Server DESTINATION ${CMAKE_SOURCE_DIR}/Server/bin)
					  
message("")
message("!!!!!!!!!!Server!!!!!!!!!!!!!!!!")
message("")