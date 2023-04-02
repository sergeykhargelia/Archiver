#include <fstream>
#include <cstddef>
#include <string>
#include <random>
#include <algorithm>
#include <climits>
#include <iostream>

static const std::size_t tests_count = 25;
static const std::size_t len = 100'000;
std::mt19937 rnd(239);

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

int main() {
	for (std::size_t i = 0; i < tests_count; i++) {
		std::string file_name = to_string(i) + ".txt";
		std::ofstream out(file_name);
		for (int j = 0; j < len; j++) {
			char value = char(rnd() % (1 << CHAR_BIT) + CHAR_MIN);
			out << value;
		}
	} 
	return 0;
}     