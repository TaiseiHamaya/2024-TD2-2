#pragma once

#include <string>

#include <Lib/Geometry/Vector3.h>

class QuaternionTransformBuffer;

class BasePlayerState {
public:
	BasePlayerState() = default;
	virtual ~BasePlayerState() = default;

public:
	virtual void begin() = 0;
	virtual Vector3f velocity() = 0;

	//virtual constexpr bool allow_interruption() = 0;

public:
	bool is_end() const { return isEnd; };

protected:
	bool isEnd{ false };
};
