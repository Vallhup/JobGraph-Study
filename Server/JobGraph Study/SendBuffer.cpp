#include "SendBuffer.h"

SendBuffer::SendBuffer(int size) : _writePos(0)
{
	_buffer.resize(size);
}

void SendBuffer::Write(const void* data, int size)
{
	if (size <= 0) return;
	if (size > _buffer.size()) _buffer.resize(size);

	memcpy(_buffer.data(), data, size);
	_writePos = size;
}
