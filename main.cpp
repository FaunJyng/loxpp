#include "main.h"
#include "utility_functions.h"

int main( int argc, char* argv[] )
{
	lox clox{};
	std::signal( SIGINT, lox::sigint_handler );
	if ( argc > 2 ) {
		println( "Usage: clox [script]" );
		std::exit( 64 );
	} else if ( argc == 2 ) {
		clox.run_file( argv[ 1 ] );
	} else {
		println( "Welcome to Clox v0.0.1" );
		println( "Type \".help\" for more information" );
		clox.run_prompt();
	}
}