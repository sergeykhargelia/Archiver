#pragma once
#include <exception>
#include <string>

namespace exceptions {

class huffman_exception : public std::exception {
public:
	huffman_exception(std::string_view error);
	virtual const char * what() const throw();

protected:
	std::string error;
};

class huffman_tree_exception : public huffman_exception {
public: 
	huffman_tree_exception(std::string_view error);
};

class wrong_arguments : public huffman_exception {
public:
	wrong_arguments(std::string_view error);
};

}