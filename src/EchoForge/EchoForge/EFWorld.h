#pragma once
#include "Avatar.h"
#include "Map.h"


namespace ef {
	struct EFWorld
	{	
		Avatar avatar;
		Map map;

		EFWorld();
		EFWorld(Avatar avatar, Map map);

	};
}
