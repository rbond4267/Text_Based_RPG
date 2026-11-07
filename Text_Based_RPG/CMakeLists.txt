cmake_minimum_required(VERSION 3.20)
project(adventure)

set(CMAKE_CXX_STANDARD 20)

add_executable(adventure main.cpp Adventure.cpp)

# Copy the story files next to the program so ./adventure can find them.
configure_file(bee_movie.txt bee_movie.txt COPYONLY)
configure_file(bad_story.txt bad_story.txt COPYONLY)
configure_file(my_story.txt my_story.txt COPYONLY)
