#include "Player.h"
#include "GlobalVariables.h"
#include <cmath>
#include "GameScene.h"

void Player::Initialize(const std::vector<Model*>& models) {

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	//グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);

	BaseCharacter::Initialize(models);
	worldTransformHead_.Initialize();

	worldTransformBase_.translation_ = startPos;
	worldTransformBase_.scale_ = {0.5f, 0.5f, 0.5f};

	rotate = {};

	
	globalVariables->AddItem(groupName, "Head Translation", worldTransformHead_.translation_);
	globalVariables->AddItem(groupName, "Bullet coolTime", coolTime);
	globalVariables->AddItem(groupName, "StartPosition", startPos);
	globalVariables->AddItem(groupName, "Size", size);
	globalVariables->AddItem(groupName, "HP", maxHp);
	

	worldTransformHead_.parent_ = &GetWorldTransformBody();
	
}

void Player::Update() {

	ApplyGlobalVariables();

	XINPUT_STATE joyState;

	Vec3 move{};
	Vec3 zeroVector{};

	if (Input::GetInstance()->GetJoystickState(0, joyState)) {

		move = {(float)joyState.Gamepad.sThumbLX, 0.0f, (float)joyState.Gamepad.sThumbLY};
		move = move / SHRT_MAX * speed;

		move = TransformNormal(move, MakeRotateYMatrix(viewProjection_->rotation_.y));

		worldTransformBase_.translation_ += move;

		if (move != zeroVector) {
			rotate = move;
		}

		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X) {
			speed = 0.3f;
			rotate = (enemy_->GetWorldPos() - GetWorldPos()).Normalize();
			worldTransformBase_.rotation_.y = std::atan2(rotate.x, rotate.z);

		} else {
			worldTransformBase_.rotation_.y = std::atan2(rotate.x, rotate.z);
			speed = 0.5f;
		}
		

	}

	const float kMoveLimitX = 90.0f;
	const float kMoveLimitZ = 90.0f;

	worldTransformBase_.translation_.x = max(worldTransformBase_.translation_.x, -kMoveLimitX);
	worldTransformBase_.translation_.x = min(worldTransformBase_.translation_.x, kMoveLimitX);
	worldTransformBase_.translation_.z = max(worldTransformBase_.translation_.z, -kMoveLimitZ);
	worldTransformBase_.translation_.z = min(worldTransformBase_.translation_.z, kMoveLimitZ);


	if (coolTimer > 0) {
		coolTimer--;
	} else {
		coolTimer = 0;
	}
	Attack();
	
	if (hp_ <= 0) {
		gameScene_->ToGameOverScene();
	}

	BaseCharacter::Update();
	worldTransformHead_.UpdateMatrix();

}

void Player::Draw(const ViewProjection& viewProjection) {

	models_[0]->Draw(worldTransformBody_, viewProjection);
	models_[1]->Draw(worldTransformHead_, viewProjection);
	
	

}

void Player::ApplyGlobalVariables() {

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	worldTransformHead_.translation_ = globalVariables->GetVec3Value(groupName, "Head Translation");
	coolTime = globalVariables->GetIntValue(groupName, "Bullet coolTime");
	startPos = globalVariables->GetVec3Value(groupName, "StartPosition");
	size = globalVariables->GetVec3Value(groupName, "Size");
	maxHp = globalVariables->GetIntValue(groupName, "HP");
	

}

void Player::Attack() {

	XINPUT_STATE joyState;

	if (!Input::GetInstance()->GetJoystickState(0, joyState)) { return; }
	if (coolTimer != 0) { return; }

	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_X) {

		
		const float kBulletSpeed = 3.0f;
		Vec3 velocity{0.0f, 0.0f, kBulletSpeed};

		velocity = enemy_->GetWorldPos() - GetWorldPos();
		velocity = velocity.Normalize() * kBulletSpeed;

		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(models_[2], GetWorldPos(), velocity);
		gameScene_->AddPlayerBullet(newBullet);
		coolTimer = coolTime;
	}

}

void Player::OnCollision() {

	hp_ -= 1;

}

void Player::SceneInitialize() {

	hp_ = maxHp;
	worldTransformBase_.translation_ = startPos;
	rotate = {};
	coolTimer = coolTime;

}