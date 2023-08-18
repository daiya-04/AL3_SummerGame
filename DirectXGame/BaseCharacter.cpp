#include "BaseCharacter.h"
#include <assert.h>

void BaseCharacter::Initialize(const std::vector<Model*>& models) {


	models_ = models;
	worldTransformBase_.Initialize();
	worldTransformBody_.Initialize();
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