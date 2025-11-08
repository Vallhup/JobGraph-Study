#include "Framework.h"

Framework::Framework(size_t size)
	: _ioCtx(size), _game(size), _network(7777, _ioCtx)
{
}

void Framework::Start()
{
	_network.Start();
}

void Framework::Stop()
{

}