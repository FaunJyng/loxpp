#include "parser.h"
#include "lox.h"
#include "token.h"
#include <optional>

bool parser::match( std::span<const token::type> ttypes )
{
	for ( token::type t : ttypes )
	{
		if ( check( t ) )
		{
			advance();
			return true;
		}
	}
	return false;
}

bool parser::match( token::type ttype )
{
	if ( check( ttype ) )
	{
		advance();
		return true;
	}
	return false;
}

bool parser::is_at_end() const
{
	return peek().ttype() == token::type::k_eof;
}

// The token at current index
const token& parser::peek() const
{
	return m_tokens[ m_current ];
}

// Check the token type at the previous index
const token& parser::previous() const
{
	return m_tokens[ m_current - 1 ];
}

// Check the token type at the current index
bool parser::check( token::type ttype ) const
{
	if ( is_at_end() )
		return false;
	return peek().ttype() == ttype;
}

const token& parser::advance()
{
	if ( !is_at_end() )
		++m_current;
	return previous();
}

// expression -> comma
parser::parse_result parser::expression()
{
	return comma();
}

// comma -> equality ( "," equality )*
// NOTE: In future, when implement function, there will be comma in foo(a, b), seperating parameters, not comma operator.
//       In that case, parse logic for function parameters will call assignment() or equality() to avoid comma being misunderstood
//            as a single expression.
parser::parse_result parser::comma()
{
	parse_result equality_res{ equality() };
	if ( equality_res.is_err() )
		return equality_res;

	expr ex{ std::move( equality_res.ok_data() ) };

	while ( match( token::type::k_comma ) )
	{
		token op{ previous() };

		parse_result right_res{ equality() };
		if ( right_res.is_err() )
			return right_res;
		expr right{ std::move( right_res.ok_data() ) };

		ex = expr{
			std::make_unique<binary_expr>(
				std::move( ex ),
				std::move( op ),
				std::move( right ) ) };
	}

	return parse_result::ok( std::move( ex ) );
}

// equality -> comparision ( ( "!=" | "==" ) comparision )*
parser::parse_result parser::equality()
{
	parse_result comparision_res{ comparision() };
	if ( comparision_res.is_err() )
		return comparision_res;

	expr ex{ std::move( comparision_res.ok_data() ) };

	token::type equality_ttypes[]{
		token::type::k_bang_equal, token::type::k_equal_equal };

	while ( match( equality_ttypes ) )
	{
		token op{ previous() };

		parse_result right_res{ comparision() };
		if ( right_res.is_err() )
			return right_res;
		expr right{ std::move( right_res.ok_data() ) };

		ex = expr{
			std::make_unique<binary_expr>(
				std::move( ex ),
				std::move( op ),
				std::move( right ) ) };
	}

	return parse_result::ok( std::move( ex ) );
}

// comparison → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
parser::parse_result parser::comparision()
{
	parse_result term_res{ term() };
	if ( term_res.is_err() )
		return term_res;

	expr ex{ std::move( term_res.ok_data() ) };

	token::type comparision_ttypes[]{
		token::type::k_greater, token::type::k_greater_equal, token::type::k_less, token::type::k_less_equal };

	while ( match( comparision_ttypes ) )
	{
		token op{ previous() };

		parse_result right_res{ term() };
		if ( right_res.is_err() )
			return right_res;
		expr right{ std::move( right_res.ok_data() ) };

		ex = expr{
			std::make_unique<binary_expr>(
				std::move( ex ),
				std::move( op ),
				std::move( right ) ) };
	}

	return parse_result::ok( std::move( ex ) );
}

// term → factor ( ( "-" | "+" ) factor )* ;
parser::parse_result parser::term()
{
	parse_result factor_res{ factor() };
	if ( factor_res.is_err() )
		return factor_res;

	expr ex{ std::move( factor_res.ok_data() ) };

	token::type term_ttypes[]{
		token::type::k_minus, token::type::k_plus };

	while ( match( term_ttypes ) )
	{
		token op{ previous() };

		parse_result right_res{ factor() };
		if ( right_res.is_err() )
			return parse_result::err(
				std::move( right_res.err_data() ) );
		expr right{ std::move( right_res.ok_data() ) };

		ex = expr{
			std::make_unique<binary_expr>(
				std::move( ex ),
				std::move( op ),
				std::move( right ) ) };
	}

	return parse_result::ok( std::move( ex ) );
}

// factor → unary ( ( "/" | "*" ) unary )* ;
parser::parse_result parser::factor()
{
	parse_result unary_res{ unary() };
	if ( unary_res.is_err() )
		return unary_res;

	expr ex{ std::move( unary_res.ok_data() ) };

	token::type factor_ttypes[]{
		token::type::k_slash, token::type::k_star };

	while ( match( factor_ttypes ) )
	{
		token op{ previous() };

		parse_result right_res{ unary() };
		if ( right_res.is_err() )
			return parse_result::err(
				std::move( right_res.err_data() ) );
		expr right{ std::move( right_res.ok_data() ) };

		ex = expr{
			std::make_unique<binary_expr>(
				std::move( ex ),
				std::move( op ),
				std::move( right ) ) };
	}

	return parse_result::ok( std::move( ex ) );
}

// unary → ( "!" | "-" ) unary
// 			| primary
parser::parse_result parser::unary()
{
	token::type unary_ttypes[]{
		token::type::k_bang, token::type::k_minus };

	while ( match( unary_ttypes ) )
	{
		token op{ previous() };

		parse_result right_result{ unary() };
		if ( right_result.is_err() )
			return parse_result::err( right_result.err_data() );
		expr right{ std::move( right_result.ok_data() ) };

		return parse_result::ok( expr{
			std::make_unique<unary_expr>(
				std::move( op ),
				std::move( right ) ) } );
	}

	return primary();
}

// primary → NUMBER | STRING | "true" | "false" | "nil"
// 			  | "(" expression ")";
parser::parse_result parser::primary()
{
	if ( match( token::type::k_false ) )
		return parse_result::ok( expr{
			std::make_unique<literal_expr>( literal{ "false" } ) } );

	if ( match( token::type::k_true ) )
		return parse_result::ok( expr{
			std::make_unique<literal_expr>( literal{ "true" } ) } );

	if ( match( token::type::k_nil ) )
		return parse_result::ok( expr{
			std::make_unique<literal_expr>( literal{ "nil" } ) } );

	if ( match( token::type::k_number ) )
		return parse_result::ok( expr{
			std::make_unique<literal_expr>( previous().literal().value() ) } );

	if ( match( token::type::k_string ) )
		return parse_result::ok( expr{
			std::make_unique<literal_expr>( previous().literal().value() ) } );

	if ( match( token::type::k_left_paren ) )
	{
		parse_result expr_result{ expression() };
		if ( expr_result.is_err() )
			return expr_result;

		consume_result consume_result{
			consume( token::type::k_right_paren, "Expect ')' after expression" ) };
		if ( consume_result.is_err() )
			return parse_result::err(
				std::move( consume_result.err_data() ) );

		return parse_result::ok( expr{
			std::make_unique<grouping_expr>(
				std::move( expr_result.ok_data() ) ) } );
	}

	return parse_result::err(
		error( peek(), "Expect expression." ) );
}

parser::consume_result parser::consume( token::type ttype, std::string_view message )
{
	if ( check( ttype ) )
		return consume_result::ok( advance() );
	return consume_result::err( error( peek(), message ) );
}

parser::parse_err parser::error( const token& token, std::string_view message )
{
	lox::error( token, message );
	return parse_err{ message };
}

void parser::synchronize()
{
	advance();

	while ( !is_at_end() )
	{
		if ( previous().ttype() == token::type::k_semicolon )
			return;

		switch ( peek().ttype() )
		{
			case token::type::k_class:
			case token::type::k_fun:
			case token::type::k_var:
			case token::type::k_for:
			case token::type::k_if:
			case token::type::k_while:
			case token::type::k_print:
			case token::type::k_return:
				return;
		}

		advance();
	}
}

std::optional<expr> parser::parse()
{
	parse_result expression_result{ expression() };
	if ( expression_result.is_err() )
		return std::nullopt;
	else
		return expr{
			std::move( expression_result.ok_data() ) };
}