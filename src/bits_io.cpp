#include "bits_io.hpp"
#include <fstream>
	
namespace bits_io {

bit_writer::bit_writer(std::ofstream & out) : out(out), current_position(0),  printed_bytes(0) {}

bit_writer::~bit_writer() {
	flush();
}

void bit_writer::append(bool next_bit) {
	buffer[current_position++] = next_bit;
	if (current_position == std::numeric_limits<std::uint8_t>::digits)
		flush();
}

void bit_writer::append(const std::vector<bool> & data) {
	for (bool bit : data)
		append(bit);
}

void bit_writer::flush() {
	if (!current_position)
		return;
	char value = static_cast<char>(buffer.to_ulong());
	out.write(&value, sizeof(value));
	current_position = 0;
	printed_bytes++;
}

bit_reader::bit_reader(std::ifstream & in) : in(in) {
	receive_next_char();
}

bool bit_reader::read_bit() {
	if (current_position == std::numeric_limits<std::uint8_t>::digits)
		receive_next_char();
	return buffer[current_position++];
}

void bit_reader::receive_next_char() {
	uint8_t c = 0;
	in.read(reinterpret_cast<char *>(&c), sizeof(c));
	buffer = c;
	current_position = 0;	
}

}