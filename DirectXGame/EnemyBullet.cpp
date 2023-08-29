#include "EnemyBullet.h"
#include <assert.h>

void EnemyBullet::Initialize(Model* model, const Vec3& position, const Vec3& velocity) {
	assert(model);

	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	velocity_ = velocity;

}

void EnemyBullet::Update() {

	worldTransform_.translation_ += velocity_;

	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	worldTransform_.UpdateMatrix();
}

void EnemyBullet::Draw(const ViewProjection& viewprojection) {

	model_->Draw(worldTransform_, viewprojection);

}

void EnemyBullet::OnCollision() {
	isDead_ = true;
}

Vec3 EnemyBullet::GetWorldPos() {
	Vec3 worldPos {};

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}