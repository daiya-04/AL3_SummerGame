#include "BaseCharacter.h"
#include <assert.h>

void BaseCharacter::Initialize(const std::vector<Model*>& models) {


	models_ = models;
	worldTransformBase_.Initialize();
	worldTransformBody_.Initialize();

	worldTransformBody_.parent_ = &GetWorlfTransformBase();
}

void BaseCharacter::Update() {


	worldTransformBase_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();

}

void BaseCharacter::Draw(const ViewProjection& viewProjection) {

	for (Model* model : models_) {
		model->Draw(worldTransformBase_, viewProjection);
	}

}

Vec3 BaseCharacter::GetWorldPos() {

	Vec3 worldPos{};

	worldPos.x = worldTransformBase_.matWorld_.m[3][0];
	worldPos.y = worldTransformBase_.matWorld_.m[3][1] + 4.0f;
	worldPos.z = worldTransformBase_.matWorld_.m[3][2];

	return worldPos;
}