#ifndef SCANNER_H
#define SCANNER_H

#include "literal.h"
#include "token.h"

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

class scanner
{
public:
	inline static const std::unordered_map<std::string, token::type> keywords{
		{ "and", token::type::k_and },
		{ "class", token::type::k_class },
		{ "else", token::type::k_else },
		{ "false", token::type::k_false },
		{ "for", token::type::k_for },
		{ "fun", token::type::k_fun },
		{ "if", token::type::k_if },
		{ "nil", token::type::k_nil },
		{ "or", token::type::k_or },
		{ "print", token::type::k_print },
		{ "question", token::type::k_question },
		{ "return", token::type::k_return },
		{ "super", token::type::k_super },
		{ "this", token::type::k_this },
		{ "true", token::type::k_true },
		{ "var", token::type::k_var },
		{ "while", token::type::k_while } };

private:
	const std::string m_source;
	std::vector<token> m_tokens;
	int m_start{ 0 };
	int m_current{ 0 };
	int m_line{ 0 };

	void scan_token();
	void add_token( token::type type );
	void add_token( token::type, std::optional<literal> literal );
	char advance();
	char peek() const;
	char peek_next() const;
	bool match( char expected );
	bool is_at_end() const;
	bool is_digit( char c ) const;
	bool is_alpha( char c ) const;
	bool is_aplha_numeric( char c ) const;

	void string();
	void number();
	void identifier();

public:
	scanner( std::string source )
		: m_source{ std::move( source ) } {};
	const std::string& source() const { return m_source; };
	const std::vector<token>& scan_tokens();
};

#endif