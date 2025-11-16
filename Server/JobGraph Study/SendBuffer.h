#pragma once

#include <vector>
#include "Constants.h"

class SendBuffer {
public:
	SendBuffer(int size = BUFFER_SIZE);

	void Write(const void* data, int size);

	char* data() { return _buffer.data(); }
	int   size() const { return _writePos; }
	void  clear() { _writePos = 0; }

private:
	std::vector<char> _buffer;
	int _writePos;
};

