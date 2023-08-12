#include "Ground.h"
#include <assert.h>

void Ground::Initialize(Model* model) {

	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.scale_ = {50.0f, 50.0f, 50.0f};

}

void Ground::Update() {

	worldTransform_.translation_ = {0.0f, -5.0f, 0.0f};

	worldTransform_.UpdateMatrix();
}

void Ground::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}