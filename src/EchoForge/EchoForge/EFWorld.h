#pragma once
#include "Avatar.h"
#include "Map.h"
#include <vector>
#include <string>


namespace ef {
	enum class EventType { COLLISION, TIME };

	struct Emitter {
		const int m_id;
		const int sound_id;
		std::string filename;
		ef::Vector3F position;
		float volume             = 1.0f;
		bool loop                = false;
		bool isActive            = false;
		EventType eventType      = EventType::COLLISION;
		float triggerTime        = 0.0f;       // secondes (EventType::TIME)
		ef::Vector3F triggerDelta;             // dx,dy,dz (EventType::COLLISION)
	};

	struct EFWorld
	{	
		Avatar avatar;
		Map map;
		std::vector<Emitter> emitters;
		EFWorld();
		EFWorld(Avatar avatar, Map map);

	};
}
