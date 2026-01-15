#ifndef LITERAL_H
#define LITERAL_H

#include <string>

class literal
{
private:
	const std::string v;

public:
	literal( std::string_view value ) : v{ value } {};
	const std::string& value() const { return v; }
};

#endif