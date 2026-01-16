#include "ast_printer.h"

std::string ast_printer::operator()( const std::unique_ptr<binary_expr>& e ) const
{
	return parenthesize( e->m_operand.lexeme(), e->m_left, e->m_right );
}

std::string ast_printer::operator()( const std::unique_ptr<unary_expr>& e ) const
{
	return parenthesize( e->m_operand.lexeme(), e->m_right );
}

std::string ast_printer::operator()( const std::unique_ptr<grouping_expr>& e ) const
{
	return parenthesize( "group", e->m_expr );
}

std::string ast_printer::operator()( const std::unique_ptr<literal_expr>& e ) const
{
	if ( !e->m_literal.has_value() )
		return "nil";
	return std::visit(
		[]( const auto& v )
		{
			using T = std::decay_t<decltype( v )>;
			if constexpr ( std::is_same_v<T, double> )
				return std::to_string( v );
			else
				return v;
		},
		e->m_literal->value() );
}