#ifndef TOKEN_H
#define TOKEN_H

#include "literal.h"

#include <optional>
#include <ostream>
#include <utility>

class token
{
public:
	enum class type
	{
		k_left_paren,
		k_right_paren,
		k_left_brace,
		k_right_brace,
		k_comma,
		k_dot,
		k_minus,
		k_plus,
		k_semicolon,
		k_slash,
		k_star,
		// one or two character tokens
		k_bang,
		k_bang_equal,
		k_equal,
		k_equal_equal,
		k_greater,
		k_greater_equal,
		k_less,
		k_less_equal,
		// literals
		k_identifier,
		k_string,
		k_number,
		// keywords
		k_and,
		k_class,
		k_else,
		k_false,
		k_fun,
		k_for,
		k_if,
		k_nil,
		k_or,
		k_print,
		k_return,
		k_super,
		k_this,
		k_true,
		k_var,
		k_while,
		k_eof,
	};

private:
	const std::string m_lexeme;
	const std::optional<literal> m_literal;
	const token::type m_ttype;
	const int m_line;

public:
	token( std::string lexeme, std::optional<literal> literal, token::type ttype, int line )
		: m_lexeme{ std::move( lexeme ) }, m_literal{ std::move( literal ) }, m_ttype{ ttype }, m_line{ line } {};
	token( std::string lexeme, token::type ttype, int line )
		: token{ std::move( lexeme ), std::nullopt, ttype, line } {};

	const std::string& lexeme() const { return m_lexeme; }
	const std::optional<literal>& literal() const { return m_literal; }
	token::type ttype() const { return m_ttype; }
	int line() const { return m_line; }

	friend std::ostream& operator<<( std::ostream& os, const token::type& token_type );
	friend std::ostream& operator<<( std::ostream& os, const token& token );
};

std::ostream& operator<<( std::ostream& os, const token& token );
std::ostream& operator<<( std::ostream& os, const token::type& token_type );

#endif