#pragma once

#include <Engine/Game/Behavior/ModelBehavior.h>
#include <Engine/Game/Collider/Collider.h>

class Boss : public ModelBehavior {
public:
	Boss() = default;
	~Boss() { finalize(); }

	Boss(const Boss&) = delete;
	Boss& operator=(const Boss&) = delete;

public:
	void initialize();
	void update();
	void update_matrix();

	void finalize();

	void SetAttributeImGui() override;

private:
	Vector3f worldPosition{ kOrigin3 };

	std::unique_ptr<Collider> collider_;

	Vector3f test_ = { 1.0f, 2.0f, 3.0f };
};
