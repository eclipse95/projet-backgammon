# CMake non fonctionnel

cmake_minimum_required(VERSION 2.8.4)
project(projet-backgammon)

# set(CMAKE_PROJECT_EXT "*.h *.c")
set(CMAKE_PROJECT_NAME ${PROJECT_NAME})

# Définition du type de compilation et des flags de compilation
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -Wshadow -std=c11 -pipe -fdiagnostics-color=auto")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Og -g -fsanitize=address")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s -O2 -DNDEBUG")
# Ajout de propriété pour la règle clean
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES *.o)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES *.so)

add_executable(myexe strategie/librairy.h strategie/librairy.c)

