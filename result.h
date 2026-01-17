#ifndef RESULT_H
#define RESULT_H

#include <variant>

template <typename T, typename E>
class result
{
private:
	std::variant<T, E> data;

	template <class... Args>
	result( std::in_place_index_t<0>, Args&&... args )
		: data{ std::in_place_index<0>, std::forward<Args>( args )... } {}

	template <class... Args>
	result( std::in_place_index_t<1>, Args&&... args )
		: data{ std::in_place_index<1>, std::forward<Args>( args )... } {}

public:
	static result ok( T v ) { return result{ std::in_place_index<0>, std::move( v ) }; }
	static result err( E e ) { return result{ std::in_place_index<1>, std::move( e ) }; }

	bool is_ok() const { return data.index() == 0; }
	bool is_err() const { return data.index() == 1; }

	T& ok_data() { return std::get<0>( data ); }
	E& err_data() { return std::get<1>( data ); }
};

#endif