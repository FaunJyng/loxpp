#include "token.h"

// token

// token::type

std::ostream& operator<<( std::ostream& os, const token::type& token_type ) {
	switch ( token_type ) {
		case token::type::k_left_paren:
			return os << "left_paren";
		case token::type::k_right_paren:
			return os << "right_paren";
		case token::type::k_left_brace:
			return os << "left_brace";
		case token::type::k_right_brace:
			return os << "right_brace";
		case token::type::k_comma:
			return os << "comma";
		case token::type::k_dot:
			return os << "dot";
		case token::type::k_minus:
			return os << "minus";
		case token::type::k_plus:
			return os << "plus";
		case token::type::k_semicolon:
			return os << "semicolon";
		case token::type::k_slash:
			return os << "slash";
		case token::type::k_star:
			return os << "star";
		case token::type::k_bang:
			return os << "bang";
		case token::type::k_bang_equal:
			return os << "bang_equal";
		case token::type::k_equal:
			return os << "equal";
		case token::type::k_equal_equal:
			return os << "equal_equal";
		case token::type::k_greater:
			return os << "greater";
		case token::type::k_greater_equal:
			return os << "greater_equal";
		case token::type::k_less:
			return os << "less";
		case token::type::k_less_equal:
			return os << "less_equal";
		case token::type::k_identifier:
			return os << "identifier";
		case token::type::k_string:
			return os << "string";
		case token::type::k_number:
			return os << "number";
		case token::type::k_and:
			return os << "and";
		case token::type::k_class:
			return os << "class";
		case token::type::k_else:
			return os << "else";
		case token::type::k_false:
			return os << "false";
		case token::type::k_fun:
			return os << "fun";
		case token::type::k_for:
			return os << "for";
		case token::type::k_if:
			return os << "if";
		case token::type::k_nil:
			return os << "nil";
		case token::type::k_or:
			return os << "or";
		case token::type::k_print:
			return os << "print";
		case token::type::k_return:
			return os << "return";
		case token::type::k_super:
			return os << "super";
		case token::type::k_this:
			return os << "this";
		case token::type::k_true:
			return os << "true";
		case token::type::k_var:
			return os << "var";
		case token::type::k_while:
			return os << "while";
		case token::type::k_eof:
			return os << "eof";
	}
	return os << "unknown";
}

std::ostream& operator<<( std::ostream& os, const token& token ) {
	os << token.ttype() << " " << token.lexeme() << " ";

	if ( token.literal().has_value() ) {
		std::visit(
			[ & ]( const auto& v ) {
				os << v;
			},
			token.literal()->value() );
	}

	return os;
}
