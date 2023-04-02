#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <vector>
#include <limits>
#include <unordered_map>
#include "huffman_exceptions.hpp"

namespace huffman_tree {

class tree_node;
using node_ptr = std::shared_ptr<tree_node>;

enum DIRECTIONS { 
	LEFT,
	RIGHT
};

class tree_node {
public:
	tree_node(std::uint32_t frequency, std::uint8_t symbol);
	
	tree_node(const node_ptr & left_node, const node_ptr & right_node);

	const node_ptr & go(bool direction) const noexcept {
		return (direction == RIGHT ? right_child : left_child);
	}

	std::optional<std::uint8_t> get_symbol() const noexcept {
		return symbol;	
	}

	std::uint32_t get_frequency() const noexcept {
		return frequency;
	}

	bool operator >(const tree_node & other) const noexcept;

private:
	std::uint32_t frequency;
	std::optional<std::uint8_t> symbol;
	node_ptr left_child, right_child; 
};

class tree {
public: 
	tree() = default;
	tree(const std::unordered_map<std::uint8_t, std::uint32_t> & frequency_table);

	std::optional<std::uint8_t> go(bool direction);

	const std::vector<bool> & get_code(std::uint8_t c) const {
		if (code_by_symbol[c].empty())
			throw exceptions::huffman_tree_exception("sybmol has empty code");
		return code_by_symbol[c];
	}
	
private:
	void build_tree(const std::unordered_map<std::uint8_t, std::uint32_t> & frequency_table);

	void build_symbol_codes(const node_ptr & node, std::vector<bool> current_code);
	node_ptr root, current_node;

	std::vector<bool> code_by_symbol[std::numeric_limits<std::uint8_t>::max() + 1];
};  

}	