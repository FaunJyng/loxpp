#ifndef CLOX_H
#define CLOX_H

#include "token.h"

#include <string_view>

class lox
{
private:
	void run( std::string_view line );
	static void report( int line, std::string_view where, std::string_view message );

public:
	inline static bool had_error{ false };
	static void sigint_handler( int signal );
	static void error( int line, std::string_view message );
	static void error( const token& token, std::string_view message );

	void run_file( std::string_view path );
	void run_prompt();
};

#endif