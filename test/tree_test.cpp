#include "doctest.h"
#include "huffman_tree.hpp"
#include <cstddef>
#include <cstdint>
#include <limits>
#include <random> 

static void check_tree_building(const std::unordered_map<std::uint8_t, std::uint32_t> & frequency_table, std::unordered_map<std::uint8_t, std::vector<bool>> & expected_code) {
	huffman_tree::tree tree(frequency_table);
	SUBCASE("check codes") {
		for (const auto & [symbol, _] : frequency_table) {
			auto & code = tree.get_code(symbol);
			CHECK(expected_code[symbol] == code);
		}	
	}
	SUBCASE("check tree structure") {
		for (const auto & [symbol, _] : frequency_table) {
			for (std::size_t i = 0; i < expected_code[symbol].size(); i++) {
				std::optional<std::uint8_t> expected_value = (i + 1 == expected_code[symbol].size() ? std::optional<std::uint8_t>(symbol) : std::nullopt),
				actual_value = tree.go(expected_code[symbol][i]);
				CHECK(actual_value == expected_value);
			}
		}
	}
}

TEST_CASE("tree building") {
	SUBCASE("one vertex") {
		std::unordered_map<std::uint8_t, std::uint32_t> frequency_table = {{'a', 10}};
		std::unordered_map<std::uint8_t, std::vector<bool>> expected_code = {{'a', {1}}};
		check_tree_building(frequency_table, expected_code);
	}
	SUBCASE("all frequencies are equal") {
		std::unordered_map<std::uint8_t, std::uint32_t> frequency_table = {{'a', 5}, {'e', 5}, {'z', 5}, {'k', 5}, {'b', 5}};
		std::unordered_map<std::uint8_t, std::vector<bool>> expected_code = {{'a', {1, 1, 0}}, {'b', {1, 1, 1}}, {'e', {0, 0}}, {'k', {0, 1}}, {'z', {1, 0}}}; 
		check_tree_building(frequency_table, expected_code);	
	}
	SUBCASE("regular tree") {
		std::unordered_map<std::uint8_t, std::uint32_t> frequency_table = {{'a', 10}, {'e', 2}, {'z', 239}, {'k', 60}, {'b', 10}};
		std::unordered_map<std::uint8_t, std::vector<bool>> expected_code = {{'a', {0, 0, 1, 1}}, {'b', {0, 0, 0}}, {'e', {0, 0, 1, 0}}, {'k', {0, 1}}, {'z', {1}}};
		check_tree_building(frequency_table, expected_code);	
	}
}