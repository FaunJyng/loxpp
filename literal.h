#ifndef LITERAL_H
#define LITERAL_H

#include <string>
#include <variant>

class literal {
	public:
		using literal_t = std::variant<double, std::string>;

	private:
		const literal_t v;

	public:
		literal( double value ) : v{ value } {};
		literal( std::string_view value ) : v{ std::string{ value } } {};
		const literal_t& value() const { return v; }

		bool is_number() const;
		bool is_string() const;
};

#endif