#pragma once
#include "Avatar.h"
#include "Map.h"
#include <vector>


namespace ef {
	struct Emitter {
		const int m_id;
		const int sound_id;
		ef::Vector3F position;
		float volume;
		bool isActive = false;
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
