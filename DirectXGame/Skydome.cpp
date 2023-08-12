#include "Skydome.h"
#include <assert.h>

void Skydome::Initialize(Model* model) {

	assert(model);
	model_ = model;
	worldTransform_.Initialize();


}

void Skydome::Update() {

	worldTransform_.translation_ = {};
	worldTransform_.UpdateMatrix();
}

void Skydome::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}