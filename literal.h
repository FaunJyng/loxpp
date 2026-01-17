#ifndef LITERAL_H
#define LITERAL_H

#include <string>
#include <variant>

class literal
{
public:
	using literal_t = std::variant<double, std::string>;

private:
	const literal_t m_value;

public:
	literal( double value ) : m_value{ value } {};
	literal( std::string_view value ) : m_value{ std::string{ value } } {};
	const literal_t& value() const { return m_value; }
};

#endif