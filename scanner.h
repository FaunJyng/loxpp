#ifndef SCANNER_H
#define SCANNER_H

#include "literal.h"
#include "token.h"
#include "lox.h"

#include <optional>
#include <string>
#include <vector>

class scanner
{
private:
	const std::string m_source;
	lox& m_lox;
	std::vector<token> m_tokens;
	int m_start{ 0 };
	int m_current{ 0 };
	int m_line{ 0 };

	bool is_at_end() const;
	void scan_token();
	char advance();
	void add_token( token::type type );
	void add_token( token::type, std::optional<literal> literal );
	bool match( char expected );
	char peek() const;
	bool is_digit( char c );

	void string();
	void number();

public:
	scanner( std::string source, lox& l )
		: m_source{ std::move( source ) }, m_lox{ l } {};
	const std::string& source() const { return m_source; };
	const std::vector<token>& scan_tokens();
};

#endif