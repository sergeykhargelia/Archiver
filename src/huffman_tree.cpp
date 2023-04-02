#include "huffman_tree.hpp"
#include <vector>

namespace huffman_tree {

tree_node::tree_node(std::uint32_t frequency = 0, std::uint8_t symbol = 0) : 
	frequency(frequency), symbol(symbol), left_child(nullptr), right_child(nullptr) {}

tree_node::tree_node(const node_ptr & left_node, const node_ptr & right_node) : symbol(std::nullopt), left_child(left_node), right_child(right_node) {
	if (left_node == nullptr || right_node == nullptr)
		throw exceptions::huffman_tree_exception("wrong parameters for constructing the node");
	frequency = left_node->frequency + right_node->frequency;
}

bool tree_node::operator >(const tree_node & other) const noexcept {
	if (frequency != other.frequency) 
		return frequency > other.frequency;
	return symbol > other.symbol;
}

tree::tree(const std::unordered_map<std::uint8_t, std::uint32_t> & frequency_table) {
	build_tree(frequency_table);
	build_symbol_codes(root, std::vector<bool>());
}

static void insert_node(std::vector<node_ptr> & nodes, const node_ptr & new_node) {
	if (new_node == nullptr) 
		throw exceptions::huffman_tree_exception("pointer to the tree node is actually nullptr");
	std::size_t position = 0;
	while (position < nodes.size() && *nodes[position] > *new_node) 
		position++;
	nodes.insert(nodes.begin() + position, new_node);
}

void tree::build_tree(const std::unordered_map<std::uint8_t, std::uint32_t> & frequency_table) {
	std::vector<node_ptr> nodes;
	
	//to avoid a case when symbol has empty code
	if (frequency_table.size() == 1) 
		insert_node(nodes, std::make_shared<tree_node>());
	
	for (const auto & [symbol, frequency] : frequency_table) 
		insert_node(nodes, std::make_shared<tree_node>(frequency, symbol));
	
	while (nodes.size() > 1) {
		node_ptr left_node = nodes.back();
		nodes.pop_back();
		node_ptr right_node = nodes.back();
		nodes.pop_back();
		insert_node(nodes, std::make_shared<tree_node>(left_node, right_node));
	}
	root = (nodes.empty() ? nullptr : nodes.back());
	current_node = root;
}

void tree::build_symbol_codes(const node_ptr & node, std::vector<bool> code_for_current_node) {
	if (node == nullptr)
		return;
	std::optional<std::uint8_t> symbol = node->get_symbol();
	if (symbol.has_value())
		code_by_symbol[symbol.value()] = code_for_current_node;		
	for (const bool & direction : {LEFT, RIGHT}) {
		code_for_current_node.push_back(direction);
		build_symbol_codes(node->go(direction), code_for_current_node);
		code_for_current_node.pop_back();	
	}
}

std::optional<std::uint8_t> tree::go(bool direction) {
	current_node = current_node->go(direction);
	if (current_node == nullptr) 
		throw exceptions::huffman_tree_exception("cannot find symbol corresponding the given code");
	std::optional<std::uint8_t> result = current_node->get_symbol();
	if (result.has_value()) 
		current_node = root;
	return result;
}

}