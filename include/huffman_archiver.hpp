#pragma once

#include "huffman_tree.hpp"
#include <string>
#include <iosfwd>
#include <cstdint>
#include <limits>
#include <memory>

namespace huffman {

struct statistics {
	std::uint32_t original_size = 0, resulting_size = 0, additional_size = 0;
};
	
class huffman_archiver {
public:
	void zip(std::string input_file_name, std::string output_file_name);
	void unzip(std::string input_file_name, std::string output_file_name);

	const statistics & get_stat() const noexcept {
		return huffman_info;
	}

private:
	static const std::unordered_map<std::uint8_t, std::uint32_t> build_encoding_frequency_table(std::ifstream & in, std::uint32_t input_len);
	static const std::unordered_map<std::uint8_t, std::uint32_t> build_decoding_frequency_table(std::ifstream & in);
	void print_encoding_frequency_table(std::ofstream & out, std::unordered_map<std::uint8_t, std::uint32_t> & frequency_table);
	
	void encode(std::ifstream & in, std::ofstream & out, std::uint32_t input_len);
	void decode(std::ifstream & in, std::ofstream & out, std::uint32_t count);

	statistics huffman_info;

	std::unique_ptr<huffman_tree::tree> tree;
};

}