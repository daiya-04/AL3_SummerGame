#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Vec3.h"
#include "Matrix44.h"

class EnemyBullet {
private:

	Model* model_;
	WorldTransform worldTransform_;

	Vec3 velocity_;

	bool isDead_ = false;

	static const int32_t kLifeTime_ = 60 * 6;
	int32_t deathTimer_ = kLifeTime_;

public:

	void Initialize(Model* model, const Vec3& position, const Vec3& velocity);

	void Update();

	void Draw(const ViewProjection& viewprojection);

	void OnCollision();

	bool IsDead() const { return isDead_; }
	const WorldTransform &GetWorldTransform() { return worldTransform_; }
	Vec3 GetWorldPos();

};
