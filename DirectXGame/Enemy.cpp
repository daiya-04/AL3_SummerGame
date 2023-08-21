#include "Enemy.h"
#include "GlobalVariables.h"


void Enemy::Initialize(const std::vector<Model*>& models) {

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Enemy";
	GlobalVariables::GetInstance()->CreateGroup(groupName);

	BaseCharacter::Initialize(models);

	globalVariables->AddItem(groupName, "Base Translation", worldTransformBase_.translation_);


}

void Enemy::Update() {

	ApplyGlobalVariables();


	BaseCharacter::Update();
}

void Enemy::Draw(const ViewProjection& viewProjection) {

	models_[0]->Draw(worldTransformBody_, viewProjection);

}

void Enemy::ApplyGlobalVariables() {

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Enemy";

	worldTransformBase_.translation_ = globalVariables->GetVec3Value(groupName, "Base Translation");

}