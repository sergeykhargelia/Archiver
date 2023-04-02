#include "doctest.h"
#include "huffman_archiver.hpp"
#include <string> 
#include <cstdlib>
#include <cstddef>
#include <algorithm>
#include <fstream>

static std::uint32_t calculate_size(std::string file_name) {
	std::ifstream in(file_name);
	in.seekg(0, std::ios::end);
	return in.tellg();
}

static const std::string path_to_test_files = "test/samples/integration_tests/";

static void run_test(std::string test_name, std::string extension) {
	std::string input_file_name = path_to_test_files + test_name + "." + extension,
	zipped_file_name = path_to_test_files + test_name + "_zipped" + "." + extension,
	output_file_name = path_to_test_files + test_name + "_answer" + "." + extension;
	huffman::huffman_archiver archiver;
	archiver.zip(input_file_name, zipped_file_name);
	huffman::statistics zip_stat = archiver.get_stat(); 
	archiver.unzip(zipped_file_name, output_file_name);
	huffman::statistics unzip_stat = archiver.get_stat();
	CHECK(zip_stat.resulting_size <= zip_stat.original_size);
	CHECK(zip_stat.resulting_size == unzip_stat.original_size);
	CHECK(zip_stat.original_size == unzip_stat.resulting_size);
	CHECK(zip_stat.additional_size == unzip_stat.additional_size);
	std::string diff_cmd = "diff " + input_file_name + " " + output_file_name;
	CHECK(system(diff_cmd.c_str()) == 0);

	CHECK(zip_stat.additional_size == 1024);
	CHECK(zip_stat.original_size == calculate_size(input_file_name));
	CHECK(zip_stat.resulting_size == calculate_size(zipped_file_name) - zip_stat.additional_size);
}

static const std::size_t tests_count = 25;

std::string to_string(std::size_t n) {
	if (n == 0)
		return "0";
	std::string result;
	while (n > 0) {
		result.push_back('0' + n % 10);
		n /= 10;
	}
	std::reverse(result.begin(), result.end());
	return result;
}

TEST_CASE("auto-generated test cases") {
	for (std::size_t i = 0; i < tests_count; i++) {
		std::string test_name = to_string(i);
		run_test(test_name, "txt");
	}
}

TEST_CASE("empty file") {
	run_test("empty", "txt");
}

TEST_CASE("all symbols are equal") {
	run_test("equal_symbols", "txt");
}

TEST_CASE("bmp file") {
	run_test("lena_512", "bmp");
}

TEST_CASE("all frequencies are equal") {
	run_test("equal_frequencies", "txt");
}