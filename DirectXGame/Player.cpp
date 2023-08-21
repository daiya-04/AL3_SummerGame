#include "Player.h"
#include "GlobalVariables.h"
#include <cmath>


void Player::Initialize(const std::vector<Model*>& models) {

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	//グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);

	BaseCharacter::Initialize(models);
	worldTransformHead_.Initialize();
	worldTransformL_Arm_.Initialize();
	worldTransformR_Arm_.Initialize();

	rotate = {};

	globalVariables->AddItem(groupName, "Head Translation", worldTransformHead_.translation_);
	globalVariables->AddItem(groupName, "ArmL Translation", worldTransformL_Arm_.translation_);
	globalVariables->AddItem(groupName, "ArmR Translation", worldTransformR_Arm_.translation_);

	worldTransformHead_.parent_ = &GetWorldTransformBody();
	worldTransformL_Arm_.parent_ = &GetWorldTransformBody();
	worldTransformR_Arm_.parent_ = &GetWorldTransformBody();
	
}

void Player::Update() {

	ApplyGlobalVariables();

	XINPUT_STATE joyState;

	Vec3 move{};
	const float kSpeed = 0.3f;
	Vec3 zeroVector{};

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		move = {(float)joyState.Gamepad.sThumbLX, 0.0f, (float)joyState.Gamepad.sThumbLY};
		move = move / SHRT_MAX * kSpeed;

		worldTransformBase_.translation_ += move;

		if (move != zeroVector) {
			rotate = move;
		}

		worldTransformBase_.rotation_.y = std::atan2(rotate.x, rotate.z);

	}


	BaseCharacter::Update();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_Arm_.UpdateMatrix();
	worldTransformR_Arm_.UpdateMatrix();

}

void Player::Draw(const ViewProjection& viewProjection) {

	models_[0]->Draw(worldTransformBody_, viewProjection);
	models_[1]->Draw(worldTransformHead_, viewProjection);
	models_[2]->Draw(worldTransformL_Arm_, viewProjection);
	models_[3]->Draw(worldTransformR_Arm_, viewProjection);
	

}

void Player::ApplyGlobalVariables() {

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	worldTransformHead_.translation_ = globalVariables->GetVec3Value(groupName, "Head Translation");
	worldTransformL_Arm_.translation_ = globalVariables->GetVec3Value(groupName, "ArmL Translation");
	worldTransformR_Arm_.translation_ = globalVariables->GetVec3Value(groupName, "ArmR Translation");

}