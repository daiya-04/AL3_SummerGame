#include "Enemy.h"
#include "GlobalVariables.h"


void Enemy::Initialize(const std::vector<Model*>& models) {

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Enemy";
	GlobalVariables::GetInstance()->CreateGroup(groupName);

	BaseCharacter::Initialize(models);
	worldTransformBody2_.Initialize();

	globalVariables->AddItem(groupName, "Base Translation", worldTransformBase_.translation_);
	
	worldTransformBody2_.translation_ = {0.0f, 7.0f, 0.0f};

}

void Enemy::Update() {

	ApplyGlobalVariables();


	BaseCharacter::Update();
	worldTransformBody2_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {

	models_[0]->Draw(worldTransformBody_, viewProjection);
	models_[1]->Draw(worldTransformBody2_, viewProjection);

}

void Enemy::ApplyGlobalVariables() {

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Enemy";

	worldTransformBase_.translation_ = globalVariables->GetVec3Value(groupName, "Base Translation");

}