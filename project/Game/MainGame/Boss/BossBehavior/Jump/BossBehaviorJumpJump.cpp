#include "BossBehaviorJumpJump.h"

#include "../../Boss.h"

BossBehaviorJumpJump::BossBehaviorJumpJump(float JumpHeight_) :
	JumpHeight(JumpHeight_) {
	boss->set_model("enemy_jump.gltf");
	JumpTime = boss->get_animator()->GetAnimationDuration(0).time;
	Sxavenger::PlayAudioOneShot("enemy_jump.wav", 0.2f);
}

void BossBehaviorJumpJump::move() {
	float parametric = std::min(1.0f, timer.time / JumpTime);
	boss->get_transform().transform.translate.y = std::lerp(0.0f, JumpHeight, parametric);

	if (timer.time >= JumpTime) {
		isEnd = true;
		
	}
}
