#include "huffman_archiver.hpp"
#include "huffman_exceptions.hpp"
#include <iostream>
#include <exception>

namespace {

enum query_type {
	ZIP, 
	UNZIP,
	UNKNOWN
};

void print_info(const huffman::statistics & info) {
	std::cout << info.original_size << std::endl << info.resulting_size << std::endl << info.additional_size << std::endl;
}

}

int main(int argc, char * argv[]) {
	try {
		query_type type = UNKNOWN;
		std::string input_file_name, output_file_name;
		huffman::huffman_archiver archiver;
		for (int i = 1; i < argc; i++) {
			std::string_view current_parameter = argv[i];
			if (current_parameter == "-c") {
				type = ZIP;
			} else if (current_parameter == "-u") {
				type = UNZIP;
			} else if ((current_parameter == "-f" || current_parameter == "--file") && i + 1 < argc) {
				input_file_name = std::string(argv[i + 1]);
			} else if ((current_parameter == "-o" || current_parameter == "--output") && i + 1 < argc) {
				output_file_name = std::string(argv[i + 1]);
			}
		}
		if (input_file_name.empty())
			throw exceptions::wrong_arguments("cannot find name of the input file");
		if (output_file_name.empty()) 
			throw exceptions::wrong_arguments("cannot find name of the output file");
		if (type == ZIP) {
			archiver.zip(input_file_name, output_file_name);
		} else if (type == UNZIP) {
			archiver.unzip(input_file_name, output_file_name);
		} else {
			throw exceptions::wrong_arguments("unknown query type");
		}
		print_info(archiver.get_stat());
	} catch (const exceptions::huffman_exception & e) {	
		std::cout << e.what() << std::endl;
		return 1;
	} catch (const std::bad_alloc & e) {
		std::cout << "Unable to allocate memory" << std::endl;
		return 1;
	} catch (const std::ios_base::failure & e) {
		std::cout << "Failed to read input file" << std::endl;
		return 1;
	}
	return 0;
}