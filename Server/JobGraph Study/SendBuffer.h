#pragma once

#include <vector>

extern short BUFFER_SIZE;

class SendBuffer {
public:
	SendBuffer(int size = BUFFER_SIZE);

	void Write(const void* data, int size);

	const char* GetBuffer() const { return _buffer.data(); }
	int GetSize() const { return _writePos; }
	void Clear() { _writePos = 0; }

private:
	std::vector<char> _buffer;
	int _writePos;
};

