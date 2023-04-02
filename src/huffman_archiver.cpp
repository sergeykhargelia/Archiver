#include "huffman_archiver.hpp"
#include "bits_io.hpp"
#include <fstream>

namespace huffman {

static std::uint32_t calculate_len(std::ifstream & in) {
	in.seekg(0, std::ios::end);
	std::uint32_t len = in.tellg();
	in.seekg(0);
	return len;
}

const std::unordered_map<std::uint8_t, std::uint32_t> huffman_archiver::build_encoding_frequency_table(std::ifstream & in, std::uint32_t input_len) {
	std::unordered_map<std::uint8_t, std::uint32_t> frequency_table;
	for (std::uint32_t i = 0; i < input_len; i++) {
		std::uint8_t symbol;
		in.read(reinterpret_cast<char *>(&symbol), sizeof(symbol));
		frequency_table[symbol]++;
	}
	in.seekg(0);
	return frequency_table;
}

void huffman_archiver::print_encoding_frequency_table(std::ofstream & out, std::unordered_map<std::uint8_t, std::uint32_t> & frequency_table) {
	for (int c = 0; c <= std::numeric_limits<std::uint8_t>::max(); c++) {
		std::uint32_t frequency = (frequency_table.count(c) ? frequency_table[c] : 0);
		out.write(reinterpret_cast<char *>(&frequency), sizeof(frequency));
		huffman_info.additional_size += sizeof(frequency);
	}
} 

void huffman_archiver::encode(std::ifstream & in, std::ofstream & out, std::uint32_t input_len) {
	bits_io::bit_writer writer(out);
	for (std::uint32_t i = 0; i < input_len; i++) {
		std::uint8_t symbol = 0;
		in.read(reinterpret_cast<char *>(&symbol), sizeof(symbol));
		writer.append(tree->get_code(symbol));
	}
	writer.flush();
	huffman_info.resulting_size = writer.printed_bytes_count();	
} 

void huffman_archiver::zip(std::string input_file_name, std::string output_file_name) {
	huffman_info = statistics();
	std::ifstream in(input_file_name, std::ios::binary);
	in.exceptions(std::ifstream::failbit | std::ifstream::eofbit);
	std::uint32_t len = calculate_len(in);
	huffman_info.original_size = len;
	auto frequency_table = build_encoding_frequency_table(in, len);
	tree = std::make_unique<huffman_tree::tree>(frequency_table);	
	std::ofstream out(output_file_name, std::ios::binary);
	print_encoding_frequency_table(out, frequency_table);
	encode(in, out, len);
}

const std::unordered_map<std::uint8_t, std::uint32_t> huffman_archiver::build_decoding_frequency_table(std::ifstream & in) {
	std::unordered_map<std::uint8_t, std::uint32_t> frequency_table;
	for (int c = 0; c <= std::numeric_limits<std::uint8_t>::max(); c++) {
		std::uint32_t frequency = 0;
		in.read(reinterpret_cast<char *>(&frequency), sizeof(frequency));
		if (frequency)
			frequency_table[c] = frequency;
	}
	return frequency_table;
}
	
void huffman_archiver::decode(std::ifstream & in, std::ofstream & out, std::uint32_t count) {
	bits_io::bit_reader reader(in);
	for (std::uint32_t i = 0; i < count; i++) {
		std::optional<std::uint8_t> symbol = std::nullopt;
		while (!symbol.has_value())
			symbol = tree->go(reader.read_bit());
		out.write(reinterpret_cast<char *>(&symbol.value()), sizeof(symbol.value()));
		huffman_info.resulting_size += sizeof(symbol.value());
	}
}

void huffman_archiver::unzip(std::string input_file_name, std::string output_file_name) {
	huffman_info = statistics();
	std::ifstream in(input_file_name, std::ios::binary);
	in.exceptions(std::ifstream::failbit | std::ifstream::eofbit);
	huffman_info.original_size = calculate_len(in);
	auto frequency_table = build_decoding_frequency_table(in);
	huffman_info.additional_size = in.tellg();
	huffman_info.original_size -= huffman_info.additional_size;
	tree = std::make_unique<huffman_tree::tree>(frequency_table);	
	std::ofstream out(output_file_name, std::ios::binary);
	std::uint32_t count = 0;
	for (const auto & [symbol, frequency] : frequency_table)
		count += frequency;
	if (count) 
		decode(in, out, count);
}

}	