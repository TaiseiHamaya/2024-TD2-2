#pragma once

#include <Engine/Game/Behavior/ModelBehavior.h>
#include <Engine/Game/Collider/Collider.h>

#include <Lib/Geometry/Vector2.h>

class Player : public ModelBehavior, Collider {
public:
	void initialize();

	void input();
	void update();
	void update_matrix();

public:
	const Vector3f& GetColliderPosition() const override;

private:
	Vector3f worldPosition{ kOrigin3 };
	Vector2f inputStick;
};
