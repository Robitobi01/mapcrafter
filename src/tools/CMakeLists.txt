add_executable(nbtdump nbtdump.cpp)
target_link_libraries(nbtdump mapcraftercore)

add_executable(testconfig testconfig.cpp)
target_link_libraries(testconfig mapcraftercore)

add_executable(testtextures testtextures.cpp)
target_link_libraries(testtextures mapcraftercore "${Boost_PROGRAM_OPTIONS_LIBRARY}")

add_executable(mapcraftertest mapcraftertest.cpp)
target_link_libraries(mapcraftertest mapcraftercore)

install(PROGRAMS "${CMAKE_CURRENT_SOURCE_DIR}/mapcrafter_textures.py" DESTINATION ${CMAKE_INSTALL_BINDIR})
install(PROGRAMS "${CMAKE_CURRENT_SOURCE_DIR}/mapcrafter_png-it.py" DESTINATION ${CMAKE_INSTALL_BINDIR})
