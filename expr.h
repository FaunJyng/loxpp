#ifndef EXPR_H
#define EXPR_H

#include "literal.h"
#include "token.h"

#include <algorithm>
#include <optional>
#include <variant>
#include <memory>

struct binary_expr;
struct unary_expr;
struct grouping_expr;
struct literal_expr;
struct conditional_expr;

using expr = std::variant<
	std::unique_ptr<binary_expr>,
	std::unique_ptr<unary_expr>,
	std::unique_ptr<grouping_expr>,
	std::unique_ptr<literal_expr>,
	std::unique_ptr<conditional_expr>>;

struct binary_expr
{
	expr m_left;
	token m_operand;
	expr m_right;
	binary_expr( expr left, token operand, expr right )
		: m_left{ std::move( left ) },
		  m_operand( std::move( operand ) ),
		  m_right{ std::move( right ) } {};
};

struct unary_expr
{
	token m_operand;
	expr m_right;
	unary_expr( token operand, expr right )
		: m_operand{ std::move( operand ) },
		  m_right{ std::move( right ) } {};
};

struct grouping_expr
{
	expr m_expr;
	grouping_expr( expr expr )
		: m_expr{ std::move( expr ) } {};
};

struct literal_expr
{
	std::optional<literal> m_literal;
	literal_expr( literal literal )
		: m_literal{ std::move( literal ) } {};
};

struct conditional_expr
{
	expr m_condition;
	expr m_then;
	expr m_else;
	conditional_expr( expr condition, expr then_branch, expr else_branch )
		: m_condition{ std::move( condition ) },
		  m_then{ std::move( then_branch ) },
		  m_else{ std::move( else_branch ) } {};
};

template <typename T>
class expr_visitor
{
public:
	virtual T operator()( const std::unique_ptr<binary_expr>& e ) const = 0;
	virtual T operator()( const std::unique_ptr<unary_expr>& e ) const = 0;
	virtual T operator()( const std::unique_ptr<grouping_expr>& e ) const = 0;
	virtual T operator()( const std::unique_ptr<literal_expr>& e ) const = 0;
	virtual T operator()( const std::unique_ptr<conditional_expr>& e ) const = 0;
};

#endif
