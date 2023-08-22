#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Vec3.h"
#include "Matrix44.h"

class PlayerBullet {
private:

	WorldTransform worldTransform_;
	Model* model_;

	Vec3 velocity_;

	static const int32_t kLifeTime = 60 * 3;
	int32_t deathTimer_ = kLifeTime;
	bool isDead_ = false;

public:

	void Initialize(Model* model, const Vec3& position, const Vec3& velocity);

	void Update();

	void Draw(const ViewProjection& viewProjection);

	void OnCollision();

	bool isDead() const { return isDead_; }
	Vec3 GetWorldPos();

};
