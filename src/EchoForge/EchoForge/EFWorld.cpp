#include "EFWorld.h"

using namespace ef;

EFWorld::EFWorld(): avatar(Avatar()), map(Map()) {

}

EFWorld::EFWorld(Avatar avatar, Map map) : avatar(avatar), map(map) {

}