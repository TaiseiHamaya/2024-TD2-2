#pragma once

#include <Engine/Game/Behavior/ModelBehavior.h>
#include <Engine/Game/Collider/Collider.h>

class Boss : public ModelBehavior {
public:
	Boss() = default;
	~Boss() = default;

	Boss(const Boss&) = delete;
	Boss& operator=(const Boss&) = delete;

public:
	void initialize();
	void update();
	void update_matrix();

private:
	Vector3f worldPosition{ kOrigin3 };

	std::unique_ptr<Collider> collider_;
};
