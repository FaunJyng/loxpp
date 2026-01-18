#ifndef PARSER_H
#define PARSER_H

#include "expr.h"
#include "result.h"
#include "token.h"

#include <optional>
#include <span>
#include <vector>

class parser
{
private:
	const std::vector<token>& m_tokens;

	int m_current{ 0 };

	bool match( std::span<const token::type> types );
	bool match( token::type types );
	bool check( token::type ttype ) const;
	bool is_at_end() const;
	const token& advance();
	const token& previous() const;
	const token& peek() const;

	using parse_ok = expr;
	using parse_err = std::string;
	using parse_result = result<parse_ok, parse_err>;

	using consume_result = result<std::reference_wrapper<const token>, parse_err>;
	consume_result consume( token::type ttype, std::string_view message );
	parse_err error( const token& token, std::string_view message );
	void synchronize();

	parse_result expression();
	parse_result comma();
	parse_result equality();
	parse_result comparision();
	parse_result term();
	parse_result factor();
	parse_result unary();
	parse_result primary();

public:
	parser( const std::vector<token>& tokens )
		: m_tokens{ tokens } {};
	std::optional<expr> parse();
};

#endif