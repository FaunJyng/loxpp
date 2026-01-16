#include "literal.h"

bool literal::is_number() const
{
	return std::holds_alternative<double>( v );
};

bool literal::is_string() const
{
	return std::holds_alternative<std::string>( v );
};