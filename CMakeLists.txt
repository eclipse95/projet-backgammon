cmake_minimum_required(VERSION 2.8.4)
project(projet-backgammon)

# Testing
enable_testing()
add_test("${PROJECT_NAME}Run" ${PROJECT_NAME})


# Définition du type de compilation et des flags de compilation
set(CMAKE_BUILD_TYPE DEBUG)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -Wshadow -std=c11 -pipe -fdiagnostics-color=auto")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Og -g -fsanitize=address")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s -O2 -DNDEBUG")
# Ajout de propriété pour la règle clean
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES *.o)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES *.so)

set(SOURCE_FILES ia_lib.c pile.c )
# Définition des chemins des .h
# permet de faire #include"truc.h" et non #include"../include/truc.h"

# include_directories("${PROJECT_BINARY_DIR}")
add_executable(dbsg ${SOURCE_FILES})
