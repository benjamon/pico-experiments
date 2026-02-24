add_subdirectory(Config)
add_subdirectory(e-Paper)
add_subdirectory(Fonts)
add_subdirectory(GUI)

add_library(pico_libs INTERFACE)

target_link_libraries(pico_libs INTERFACE Config ePaper GUI Fonts)