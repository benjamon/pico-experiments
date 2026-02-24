# Find all source files in a single current directory
# Save the name to DIR_ePaper_SRCS
aux_source_directory(. DIR_ePaper_SRCS)

# Generate the link library
add_library(ePaper ${DIR_ePaper_SRCS})
target_include_directories(ePaper PUBLIC .)
target_link_libraries(ePaper PUBLIC Config)