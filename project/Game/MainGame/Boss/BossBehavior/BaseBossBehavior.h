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
	void update_collider(const Vector3f& position);

public:
	const DeltaTimePoint& get_timer()const { return timer; };
	bool is_end() const { return isEnd; };
	void any_hit_collision() { isHitCollision = true; };
	const std::unique_ptr<Collider>& get_attack_collider() const { return attackCollider; };

public:
	virtual constexpr bool allow_interruption() = 0;

protected:
	bool isHitCollision{ false };
	DeltaTimePoint timer;
	std::unique_ptr<Collider> attackCollider;
	bool isEnd{ false };

public:
	static inline Boss* boss = nullptr;
};
