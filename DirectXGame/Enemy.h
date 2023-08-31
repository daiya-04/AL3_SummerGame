#pragma once
#include "BaseCharacter.h"
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Vec3.h"
#include "Matrix44.h"
#include <optional>

class GameScene;

class Enemy : public BaseCharacter {
private:

	enum class Stage{
		Stage1,
		Stage2,
		Stage3,
		Stage4,
	};

	enum class Behavior {
		kRoot,
		kAttack,
		kBreak,
	};

	WorldTransform worldTransformBody2_;
	WorldTransform worldTransformBody3_;
	WorldTransform worldTransformHead_;

	Stage stage_ = Stage::Stage1;
	std::optional<Stage> stageRequest_ = std::nullopt;

	Behavior behavior_ = Behavior::kRoot;
	std::optional<Behavior> behaviorRequest_ = std::nullopt;

	int maxHp = 200;
	int hp_ = maxHp;

	Vec3 size = {1.5f, 1.0f, 1.5f};

	float num = 0.0f;
	Vec3 fallStart{};
	Vec3 fallEnd{};

	int bulletCoolTime = 15;
	int bulletCoolTimer = 0;

	int attackCoolTime = 60 * 5;
	int attackCoolTimer = attackCoolTime;

	int attackTime = 60 * 5;
	int attackTimer = attackTime;

	GameScene* gameScene_ = nullptr;

public:

	void Initialize(const std::vector<Model*>& models) override;

	void Update() override;

	void Draw(const ViewProjection& viewProjection) override;

	void OnCollision();

	void ApplyGlobalVariables();

	void RootInitialize();

	void RootUpdate();

	void AttackInitialize();

	void AttackUpdate(const Matrix44& worldMatrix);

	void FallingInitialize(const Vec3& translation);

	void Falling(Vec3& translation);

	void SceneInitialize();

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	Vec3 GetSize() { return size; }
	int GetHP() { return hp_; }

};
