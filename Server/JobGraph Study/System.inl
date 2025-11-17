#pragma once

#include "System.h"
#include "Framework.h"

//template<typename EventT>
//void EventSystem<EventT>::ConsumeEvents()
//{
//	GameEvent event;
//	while (Framework::Get().eventQueue.try_pop(event))
//	{
//		if (auto* e = std::get_if<EventT>(&event))
//			ProcessEvent(*e);
//	}
//}