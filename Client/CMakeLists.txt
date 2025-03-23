project(Client)

add_executable(Client src/main.cpp)

target_include_directories(Client PUBLIC 
						   ${CMAKE_SOURCE_DIR}/include/
						   ${CMAKE_SOURCE_DIR}/Client/include/ )

include(FetchContent)
FetchContent_Declare(
    SFML
    URL https://github.com/SFML/SFML/archive/refs/tags/2.5.1.zip
)

FetchContent_MakeAvailable(SFML)

target_include_directories(Client PUBLIC 
						   lib )

target_link_libraries(Client 
                      sfml-graphics
                      sfml-system
                      sfml-window
                      )					  
					  
install(TARGETS Client DESTINATION ${CMAKE_SOURCE_DIR}/Client/bin)
					  
message("")
message("!!!!!!!!!!Client!!!!!!!!!!!!!!!!")
message("")