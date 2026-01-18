#include "scanner.h"
#include "lox.h"

const std::vector<token>& scanner::scan_tokens()
{
	while ( !is_at_end() )
	{
		m_start = m_current;
		scan_token();
	}

	m_tokens.emplace_back(
		"",
		token::type::k_eof,
		m_line );

	return m_tokens;
}

void scanner::scan_token()
{
	char c = advance();
	switch ( c )
	{
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
		case '?':
			add_token( token::type::k_question );
			break;
		case ':':
			add_token( token::type::k_colon );
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
			if ( match( '=' ) )
			{
				while ( peek() != '\n' && !is_at_end() )
				{
					advance();
				}
			}
			else
			{
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
			if ( is_digit( c ) )
			{
				number();
			}
			else if ( is_alpha( c ) )
			{
				identifier();
			}
			else
			{
				lox::error( m_line, "unexpected character." );
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

char scanner::peek() const
{
	if ( is_at_end() )
		return '\0';
	return m_source[ m_current ];
}

char scanner::peek_next() const
{
	if ( m_current + 1 >= m_source.length() )
		return '\0';
	return m_source.at( m_current + 1 );
}

bool scanner::match( char expected )
{
	if ( is_at_end() || m_source[ m_current ] != expected )
		return false;
	++m_current;
	return true;
}

bool scanner::is_at_end() const
{
	return m_current >= m_source.length();
}

bool scanner::is_digit( char c ) const
{
	return c >= '0' && c <= '9';
}

bool scanner::is_alpha( char c ) const
{
	return ( c >= 'a' && c <= 'z' ) ||
		   ( c >= 'A' && c <= 'Z' ) ||
		   c == '_';
}

bool scanner::is_aplha_numeric( char c ) const
{
	return is_alpha( c ) || is_digit( c );
}

void scanner::string()
{
	while ( peek() != '"' && !is_at_end() )
	{
		if ( peek() == '\n' )
			++m_line;
		advance();
	}

	if ( is_at_end() )
	{
		lox::error( m_line, "unterminated string." );
		return;
	}

	advance();

	add_token(
		token::type::k_string,
		literal{
			m_source.substr( m_start + 1, m_current - ( m_start + 1 ) - 1 ) } );
}

void scanner::number()
{
	while ( is_digit( peek() ) )
		advance();
	if ( peek() == '.' && is_digit( peek_next() ) )
	{
		advance();
		while ( is_digit( peek() ) )
			advance();
	}
	add_token(
		token::type::k_number,
		std::stod(
			m_source.substr( m_start, m_current - m_start ) ) );
}

void scanner::identifier()
{
	while ( is_aplha_numeric( peek() ) )
		advance();

	token::type ttype;

	auto it{
		keywords.find(
			m_source.substr( m_start, m_current - m_start ) ) };

	if ( it == keywords.end() )
		ttype = token::type::k_identifier;
	else
		ttype = it->second;

	add_token( ttype );
}