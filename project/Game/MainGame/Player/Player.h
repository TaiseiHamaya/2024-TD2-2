#pragma once

#include <Engine/Game/Behavior/ModelBehavior.h>
#include <Engine/Game/Collider/Collider.h>

#include <Lib/Geometry/Vector2.h>

class Player : public ModelBehavior {
public:
	void initialize();

	void input();
	void update();
	void update_matrix();

private:
	Vector3f worldPosition{ kOrigin3 };
	Vector2f inputStick;

	std::unique_ptr<Collider> collider_;
};
