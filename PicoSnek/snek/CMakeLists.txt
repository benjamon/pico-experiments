aux_source_directory(. DIR_SNEK_SRCS)
add_library(snek ${DIR_SNEK_SRCS})
target_include_directories(snek PUBLIC .)
target_link_libraries(snek PUBLIC GUI ePaper Config)