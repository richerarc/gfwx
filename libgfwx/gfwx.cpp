#include <gfwx/gfwx.hpp>

using namespace GFWX;

uint32_t GFWX::Bits::getBits(int bits) {
	int newBits = indexBits + bits;
	if (buffer == bufferEnd)
		return indexBits = -1;        // signify overflow
	uint32_t x = *buffer << indexBits;
	if (newBits >= 32) {
		++buffer;
		if ((newBits -= 32) > 0) {
			if (buffer == bufferEnd)
				return indexBits = -1;        // signify overflow
			x |= *buffer >> (32 - indexBits);
		}
	}
	indexBits = newBits;
	return x >> (32 - bits);
}

void GFWX::Bits::putBits(uint32_t x, int bits) {
	int newBits = indexBits + bits;
	if (buffer == bufferEnd)
		newBits = -1;        // signify overflow
	else if (newBits < 32)
		(writeCache <<= bits) |= x;
	else if (bits == 32 && newBits == 32) {
		newBits = 0;
		*(buffer++) = x;
	}
	else {
		newBits -= 32;
		*(buffer++) = (writeCache << (bits - newBits)) | (x >> newBits);
		writeCache = x;
	}
	indexBits = newBits;
}

uint32_t GFWX::Bits::getZeros(uint32_t maxZeros) {
	int newBits = indexBits;
	if (buffer == bufferEnd)
		return indexBits = -1;        // signify overflow
	uint32_t b = *buffer;
	uint32_t x = 0;
	while (true) {
		if (newBits == 31) {
			++buffer;
			if ((b & 1u) || (++x == maxZeros)) {
				indexBits = 0;
				return x;
			}
			if (buffer == bufferEnd)
				return indexBits = -1;        // signify overflow
			b = *buffer;
			newBits = 0;
			continue;
		}
		if (((b << newBits) & (1u << 31)) || (++x == maxZeros)) {
			indexBits = newBits + 1;
			return x;
		}
		++newBits;
	}
}

void GFWX::Bits::flushWriteWord() {        // [NOTE] does not clear overflow
	putBits(0, (32 - indexBits) % 32);
}

void GFWX::Bits::flushReadWord() {        // [NOTE] does not clear overflow
	if (indexBits <= 0)
		return;
	++buffer;
	indexBits = 0;
}