#pragma once

#include <Engine/Game/Behavior/ModelBehavior.h>
#include <Engine/Game/Collider/Collider.h>

class Boss : public ModelBehavior, Collider {
public:
	Boss() = default;
	~Boss() = default;

	Boss(const Boss&) = delete;
	Boss& operator=(const Boss&) = delete;

public:
	void initialize();
	void update();
	void update_matrix();

public:
	const Vector3f& GetColliderPosition() const override;

private:
	Vector3f worldPosition{ kOrigin3 };
};
