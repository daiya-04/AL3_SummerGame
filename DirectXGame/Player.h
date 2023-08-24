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
	WorldTransform worldTransformL_Arm_;
	WorldTransform worldTransformR_Arm_;

	Vec3 rotate;

	int coolTime = 60;
	int coolTimer = 0;

	float speed=0.5f;

	GameScene* gameScene_ = nullptr;
	Enemy* enemy_ = nullptr;

public:

	void Initialize(const std::vector<Model*>& models) override;

	void Update() override;

	void Draw(const ViewProjection& viewProjection) override;

	void ApplyGlobalVariables();

	void Attack();

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
	void SetEnemy(Enemy* enemy) { enemy_ = enemy; }

};
