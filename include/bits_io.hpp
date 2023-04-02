#pragma once

#include <iosfwd>
#include <vector>
#include <bitset>
#include <cstddef>
#include <limits>
#include <cstdint>

namespace bits_io {

class bit_writer {
public:
	bit_writer(std::ofstream & out);

	~bit_writer();

	void flush();

	void append(bool next_bit);

	void append(const std::vector<bool> & data);

	std::uint32_t printed_bytes_count() const noexcept {
		return printed_bytes;
	}

private:
	std::ofstream & out;
	std::bitset<std::numeric_limits<std::uint8_t>::digits> buffer;
	std::size_t current_position;
	std::uint32_t printed_bytes;
};

class bit_reader {
public:
	bit_reader(std::ifstream & in);

	bool read_bit();

private:
	void receive_next_char();

	std::ifstream & in;
	std::bitset<std::numeric_limits<std::uint8_t>::digits> buffer;
	std::size_t current_position;
};

}