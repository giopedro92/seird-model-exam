cmake_minimum_required(VERSION 3.16)
project(population CXX)

# abilita il supporto per i test
include(CTest)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON) #??

# richiede l'uso di C++17, senza estensioni non-standard offerte dal compilatore usato 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# abilita warning
string(APPEND CMAKE_CXX_FLAGS
      " -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion"
      " -Wshadow -Wimplicit-fallthrough -Wextra-semi -Wold-style-cast")
      
# abilita asserzioni di debug (in gcc), l'address sanitizer e l'undefined-behaviour sanitizer in debug mode
string(APPEND CMAKE_CXX_FLAGS_DEBUG " -D_GLIBCXX_ASSERTIONS -fsanitize=address,undefined -fno-omit-frame-pointer")
string(APPEND CMAKE_EXE_LINKER_FLAGS_DEBUG " -fsanitize=address,undefined -fno-omit-frame-pointer")


add_executable(population main.cpp population.cpp)

# aggiunge l'eseguibile population.t
add_executable(population.t test.cpp population.cpp)
# aggiunge l'eseguibile population.t alla lista dei test
add_test(NAME population.t COMMAND population.t)