#include "scanner.h"
#include "literal.h"
#include "lox.h"
#include "token.h"

const std::vector<token>& scanner::scan_tokens()
{
	while ( !is_at_end() ) {
		m_start = m_current;
		scan_token();
	}

	m_tokens.emplace_back(
		"",
		token::type::k_eof,
		m_line );

	return m_tokens;
}

bool scanner::is_at_end() const
{
	return m_current >= m_source.length();
}

void scanner::scan_token()
{
	char c = advance();
	switch ( c ) {
		case '(':
			add_token( token::type::k_left_paren );
			break;
		case ')':
			add_token( token::type::k_right_paren );
			break;
		case '{':
			add_token( token::type::k_left_brace );
			break;
		case '}':
			add_token( token::type::k_right_brace );
			break;
		case ',':
			add_token( token::type::k_comma );
			break;
		case '.':
			add_token( token::type::k_dot );
			break;
		case '-':
			add_token( token::type::k_minus );
			break;
		case '+':
			add_token( token::type::k_plus );
			break;
		case ';':
			add_token( token::type::k_semicolon );
			break;
		case '*':
			add_token( token::type::k_star );
			break;
		case '!':
			add_token( match( '=' ) ? token::type::k_bang_equal : token::type::k_bang );
			break;
		case '=':
			add_token( match( '=' ) ? token::type::k_equal_equal : token::type::k_equal );
			break;
		case '<':
			add_token( match( '=' ) ? token::type::k_less_equal : token::type::k_less );
			break;
		case '>':
			add_token( match( '=' ) ? token::type::k_greater_equal : token::type::k_greater );
			break;
		case '/':
			if ( match( '=' ) ) {
				while ( peek() != '\n' && !is_at_end() ) {
					advance();
				}
			} else {
				add_token( token::type::k_slash );
			}
			break;
		case ' ':
		case '\r':
		case '\t':
			break;
		case '\n':
			++m_line;
			break;
		case '"':
			string();
			break;
		default:
			if ( is_digit( c ) ) {
				number();
			} else {
				m_lox.error( m_line, "unexpected character." );
			}
			break;
	}
}

char scanner::advance()
{
	return m_source[ m_current++ ];
}

void scanner::add_token( token::type ttype )
{
	add_token( ttype, std::nullopt );
}

void scanner::add_token( token::type ttype, std::optional<literal> literal )
{
	std::string text{ m_source.substr( m_start, m_current - m_start ) };
	m_tokens.emplace_back( std::move( text ), std::move( literal ), ttype, m_line );
}

bool scanner::match( char expected )
{
	if ( is_at_end() || m_source[ m_current ] != expected )
		return false;
	++m_current;
	return true;
}

char scanner::peek() const
{
	if ( is_at_end() )
		return '\0';
	return m_source[ m_current ];
}

void scanner::string()
{
	while ( peek() != '"' && !is_at_end() ) {
		if ( peek() == '\n' )
			++m_line;
		advance();
	}

	if ( is_at_end() ) {
		m_lox.error( m_line, "unterminated string." );
		return;
	}

	advance();

	add_token(
		token::type::k_string,
		literal{
			m_source.substr( m_start + 1, m_current - ( m_start + 1 ) - 1 ) } );
}

bool scanner::is_digit( char c )
{
	return c >= '0' && c <= '9';
}

void scanner::number()
{
	while ( is_digit( peek() ) )
		advance();
}