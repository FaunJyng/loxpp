#include "scanner.h"
#include "utility_functions.h"
#include "lox.h"

#include <csignal>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

void lox::sigint_handler( int signal )
{
	if ( signal == SIGINT )
	{
		util::println( "keyboard_interuption" );
		util::print( "> " );
	}
	std::signal( SIGINT, sigint_handler );
}

void lox::run_file( std::string_view path )
{
	std::ifstream file{ path.data(), std::ios::binary };
	if ( !file )
	{
		std::string message = "Cannot open file ";
		message.append( path );
		throw new std::runtime_error( message );
	}
	const std::string content{
		std::istreambuf_iterator<char>{ file },
		std::istreambuf_iterator<char>{} };
	run( content );
	if ( had_error )
	{
		std::exit( 65 );
	}
}

void lox::run_prompt()
{
	std::string line;
	util::print( "> " );
	while ( true )
	{
		if ( std::getline( std::cin, line ) )
		{
			if ( line == ".q" || line == ".quit" )
				break;
			run( line );
			had_error = false;
			util::print( "> " );
		}
		else
		{
			std::cin.clear();
		}
	}
}

void lox::error( int line, std::string_view message )
{
	report( line, "", message );
}

void lox::report( int line, std::string_view where, std::string_view message )
{
	util::println( "\x1b[31m[line ", line, "] Error", where, ": ", message, "\x1b[0m" );
	had_error = true;
}

void lox::run( std::string_view source )
{
	if ( source.empty() || source.length() == 0 )
	{
		util::print( "" );
	}
	else
	{
		scanner sc{ std::string{ source }, *this };
		std::vector<token> tokens = sc.scan_tokens();
		for ( const token& token : tokens )
		{
			util::println( "\033[33m", token, "\033[0m" );
		}
	}
}