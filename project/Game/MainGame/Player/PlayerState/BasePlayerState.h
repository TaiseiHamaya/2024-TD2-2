#pragma once

#include <Lib/Geometry/Vector3.h>

class Player;
class QuaternionTransformBuffer;

class BasePlayerState {
public:
	BasePlayerState() = default;
	virtual ~BasePlayerState() = default;

public:
	virtual void initialize(Player* target);

	virtual void begin() = 0;
	virtual Vector3f velocity() = 0;

protected:
	Player* player;
};
