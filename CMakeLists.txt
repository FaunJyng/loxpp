cmake_minimum_required(VERSION 3.30)

project(clox 
	VERSION 0.0.1
	DESCRIPTION "Clox"
	LANGUAGES CXX
)

add_executable(
    main
    literal.cpp
    lox.cpp
    main.cpp
    scanner.cpp
    token.cpp
)

target_include_directories(main PRIVATE "./")
target_compile_features(main PRIVATE cxx_std_17)
