#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "BaseCharacter.h"
#include "Input.h"
#include "Vec3.h"
#include "Matrix44.h"

class GameScene;
class Enemy;

class Player : public BaseCharacter {
private:

	WorldTransform worldTransformHead_;

	Vec3 size = {1.0f, 2.0f, 1.0f};
	Vec3 rotate;

	int coolTime = 30;
	int coolTimer = 0;

	float speed = 0.5f;

	GameScene* gameScene_ = nullptr;
	Enemy* enemy_ = nullptr;

	const ViewProjection* viewProjection_ = nullptr;

public:

	void Initialize(const std::vector<Model*>& models) override;

	void Update() override;

	void Draw(const ViewProjection& viewProjection) override;

	void ApplyGlobalVariables();

	void Attack();

	void OnCollision();

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
	void SetEnemy(Enemy* enemy) { enemy_ = enemy; }
	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; }

	Vec3 GetSize() { return size; }

};
