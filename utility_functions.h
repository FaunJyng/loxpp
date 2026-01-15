#ifndef UTILITY_FUNCTIONS_H
#define UTILITY_FUNCTIONS_H

#include <iostream>

template <typename... Args>
void print( Args&&... args )
{
	( std::cout << ... << std::forward<Args>( args ) );
}

template <typename... Args>
void println( Args&&... args )
{
	( std::cout << ... << std::forward<Args>( args ) ) << '\n';
}

#endif