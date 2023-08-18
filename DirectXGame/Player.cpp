#include "Player.h"

void Player::Initialize(const std::vector<Model*>& models) {

	BaseCharacter::Initialize(models);
	worldTransformHead_.Initialize();
	worldTransformL_Arm_.Initialize();
	worldTransformR_Arm_.Initialize();

	
	
}

void Player::Update() {




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
	//models_[4]->Draw(worldTransform_[4], viewProjection);

}