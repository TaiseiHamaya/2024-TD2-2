#pragma once

#include <Engine/System/Performance.h>
#include <Engine/Game/Collider/Collider.h>

#include <memory>

class Boss;

class BaseBossBehavior {
public:
	BaseBossBehavior() = default;
	virtual ~BaseBossBehavior() = default;

public:
	void begin();
	virtual void move() = 0;

public:
	const DeltaTimePoint& get_timer()const { return timer; };
	bool is_end() const { return isEnd; };
	void any_hit_collision() { isHitCollision = true; };

protected:
	bool isHitCollision{ false };
	DeltaTimePoint timer;
	std::unique_ptr<Collider> attackCollider;
	bool isEnd{ false };

public:
	static inline Boss* boss = nullptr;
};
