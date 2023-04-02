#include "doctest.h"
#include "huffman_exceptions.hpp"
#include <string>

TEST_CASE("regular huffman exception") {
  std::string_view message = "here we go, again";
  try {
    throw exceptions::huffman_exception("here we go, again"); 
  } catch (const exceptions::huffman_exception & e) {
    std::string_view error = e.what();
    CHECK(error == message);
  }
}

TEST_CASE("huffman tree exception") {
	try {
		throw exceptions::huffman_tree_exception("kek");
	} catch(const exceptions::huffman_tree_exception & e) {
		std::string_view error = e.what();
		CHECK(error == "failed to build huffman tree: kek");
	}
}

TEST_CASE("wrong arguments exception") {
	try {
		throw exceptions::wrong_arguments("abacaba");
	} catch(const exceptions::wrong_arguments & e) {
		std::string_view error = e.what();
		CHECK(error == "failed to parse arguments: abacaba");
	}
}