#ifndef CLOX_H
#define CLOX_H

#include <string_view>

class lox {
	private:
		void run( std::string_view line );

	public:
		static void sigint_handler( int signal );

		bool had_error{ false };
		void run_file( std::string_view path );
		void run_prompt();
		void error( int line, std::string_view message );
		void report( int line, std::string_view where, std::string_view message );
};

#endif