#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include "expr.h"

#include <memory>
#include <sstream>
#include <string>

class ast_printer : public expr_visitor<std::string>
{
private:
	template <typename... exprs_t>
	inline std::string parenthesize(
		std::string_view name, const exprs_t&... exprs ) const
	{
		std::ostringstream out;
		out << "( " << name;
		( ( out << " " << print( exprs ) ), ... );
		out << " )";
		return out.str();
	}

public:
	std::string operator()( const std::unique_ptr<binary_expr>& e ) const override;
	std::string operator()( const std::unique_ptr<unary_expr>& e ) const override;
	std::string operator()( const std::unique_ptr<grouping_expr>& e ) const override;
	std::string operator()( const std::unique_ptr<literal_expr>& e ) const override;
	std::string operator()( const std::unique_ptr<conditional_expr>& e ) const override;

	static std::string print( const expr& e )
	{
		return std::visit( ast_printer{}, e );
	}
};

#endif