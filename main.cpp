#include "ast_printer.h"
#include "utils.h"
#include "lox.h"
#include "expr.h"
#include "token.h"

#include <csignal>
#include <memory>

void test_ast_printer();

int main( int argc, char* argv[] )
{
	lox clox{};
	std::signal( SIGINT, lox::sigint_handler );
	if ( argc > 2 )
	{
		utils::println( "Usage: clox [script]" );
		std::exit( 64 );
	}
	else if ( argc == 2 )
	{
		clox.run_file( argv[ 1 ] );
	}
	else
	{
		utils::println( "Welcome to Clox v0.0.1" );
		utils::println( "Type \".help\" for more information" );
		clox.run_prompt();
	}
}

void test_ast_printer()
{
	expr unary{
		std::make_unique<unary_expr>(
			token{ "-", std::nullopt, token::type::k_minus, 1 },
			expr{ std::make_unique<literal_expr>( literal{ 123.0 } ) } ) };

	expr group{
		std::make_unique<grouping_expr>(
			expr{ std::make_unique<literal_expr>( literal{ 45.67 } ) } ) };

	expr expression{
		std::make_unique<binary_expr>(
			std::move( unary ),
			token{ "*", token::type::k_star, 1 },
			std::move( group ) ) };

	ast_printer printer;
	std::string result{ printer.print( expression ) };
	utils::print( result );
}
