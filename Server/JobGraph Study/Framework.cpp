#include "Framework.h"

Framework::Framework(size_t size)
	: _game(size), _network(7777, size), _running(false)
{
}

void Framework::Start()
{
	using namespace std::chrono;

	SetConsoleCtrlHandler(ConsoleHandler, TRUE);

	_running = true;
	_network.Start();
	
	const float dT = 1.0f / 60.0f;
	auto prev = steady_clock::now();
	while (_running)
	{
		auto now = steady_clock::now();
		float elapsed = duration<float>(now - prev).count();

		if (elapsed >= dT)
		{
			prev = now;
			_game.Update(dT);
		}
	}

	_network.Stop();
	_game.threadPool.Stop();
}

void Framework::Stop()
{
	_running = false;
}

BOOL __stdcall Framework::ConsoleHandler(DWORD ctrlType)
{
	switch (ctrlType) {
	case CTRL_C_EVENT:
	case CTRL_BREAK_EVENT:
	case CTRL_CLOSE_EVENT:
	case CTRL_LOGOFF_EVENT:
	case CTRL_SHUTDOWN_EVENT:
		Framework::Get().Stop();
		return TRUE;
	}

	return FALSE;
}