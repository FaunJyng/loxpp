#include "lox.h"

#include "ast_printer.h"
#include "parser.h"
#include "scanner.h"
#include "utils.h"

#include <csignal>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

void lox::sigint_handler( int signal )
{
	if ( signal == SIGINT )
	{
		utils::println( "keyboard_interuption" );
		utils::print( "> " );
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
	utils::print( "> " );
	while ( true )
	{
		if ( std::getline( std::cin, line ) )
		{
			if ( line == ".q" || line == ".quit" )
				break;
			run( line );
			had_error = false;
			utils::print( "> " );
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

void lox::error( const token& token, std::string_view message )
{
	if ( token.ttype() == token::type::k_eof )
		report( token.line(), " at end", message );
	else
	{
		std::ostringstream where{};
		where << " at '" << token.lexeme() << "'";
		report( token.line(), where.str(), message );
	}
}

void lox::report( int line, std::string_view where, std::string_view message )
{
	utils::println( "\x1b[31m[line ", line, "] Error", where, ": ", message, "\x1b[0m" );
	had_error = true;
}

void lox::run( std::string_view source )
{
	if ( source.empty() || source.length() == 0 )
		utils::print( "" );
	else
	{
		scanner sc{ std::string{ source } };
		std::vector<token> tokens = sc.scan_tokens();
		parser parser{ tokens };

		std::optional<expr> expr_opt{ parser.parse() };
		if ( expr_opt.has_value() )
		{
			ast_printer ast_printer{};
			utils::println( ast_printer.print( expr_opt.value() ) );
		}
		else
			return;
	}
}