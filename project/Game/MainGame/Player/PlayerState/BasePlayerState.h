#pragma once

#include <memory>

#include <Lib/Geometry/Vector3.h>

class Collider;
class QuaternionTransformBuffer;

class BasePlayerState {
public:
	BasePlayerState();
	virtual ~BasePlayerState();

public:
	virtual void begin() = 0;
	virtual Vector3f velocity() = 0;
	void update_collider(const Vector3f& position);

	//virtual constexpr bool allow_interruption() = 0;

public:
	const std::unique_ptr<Collider>& get_attack_collider() const { return attackCollider; };

protected:
	void create_collider();

public:
	bool is_end() const { return isEnd; };

protected:
	bool isEnd{ false };
	std::unique_ptr<Collider> attackCollider;
};
