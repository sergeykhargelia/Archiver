#include "huffman_exceptions.hpp"

namespace exceptions {

huffman_exception::huffman_exception(std::string_view message) : error(message) {}

const char * huffman_exception::what() const throw() {
	return error.c_str();
}

huffman_tree_exception::huffman_tree_exception(std::string_view message) : huffman_exception("failed to build huffman tree: ") {
	error += message;
}

wrong_arguments::wrong_arguments(std::string_view message) : huffman_exception("failed to parse arguments: ") {
	error += message;
}

}