#include "Enemy.h"
#include "GlobalVariables.h"
#include <cmath>
#include <numbers>
#include "GameScene.h"


void Enemy::Initialize(const std::vector<Model*>& models) {

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Enemy";
	GlobalVariables::GetInstance()->CreateGroup(groupName);

	BaseCharacter::Initialize(models);
	worldTransformBody2_.Initialize();
	worldTransformBody3_.Initialize();
	worldTransformHead_.Initialize();

	globalVariables->AddItem(groupName, "Base Translation", worldTransformBase_.translation_);
	globalVariables->AddItem(groupName, "HP", maxHp);
	globalVariables->AddItem(groupName, "Size", size);
	globalVariables->AddItem(groupName, "Bullet coolTime", bulletCoolTime);
	globalVariables->AddItem(groupName, "Attack coolTime", attackCoolTime);
	globalVariables->AddItem(groupName, "AttackTime", attackTime);
	
	worldTransformBody2_.translation_ = {0.0f, 6.0f, 0.0f};
	worldTransformBody3_.translation_ = {0.0f, 6.0f, 0.0f};
	worldTransformHead_.translation_ = {0.0f, 6.0f, 0.0f};

	worldTransformBody3_.parent_ = &worldTransformBody2_;
	worldTransformHead_.parent_ = &worldTransformBody3_;

}

void Enemy::Update() {

	ApplyGlobalVariables();

	if (stageRequest_) {
		stage_ = stageRequest_.value();

		switch (stage_) {
			case Stage::Stage1:

				break;
		    case Stage::Stage2:
			    FallingInitialize(worldTransformBody2_.translation_);
				break;
		    case Stage::Stage3:
			    FallingInitialize(worldTransformBody3_.translation_);
				break;
		    case Stage::Stage4:
			    FallingInitialize(worldTransformHead_.translation_);
				break;
		}
		behaviorRequest_ = Behavior::kBreak;
		stageRequest_ = std::nullopt;
	}

	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();

		switch (behavior_) {
		case Behavior::kRoot:
		default:
			    RootInitialize();

			    break;
		case Behavior::kAttack:
			    AttackInitialize();
			    break;
		case Behavior::kBreak:

			    break;
		}
		behaviorRequest_ = std::nullopt;
	}

	switch (behavior_) {
		case Behavior::kRoot:
	    default:
		    RootUpdate();
			break;
	    case Behavior::kAttack:
		    switch (stage_) {
		    case Stage::Stage1:
			    AttackUpdate(worldTransformBody_.matWorld_);
			    break;
		    case Stage::Stage2:
			    AttackUpdate(worldTransformBody2_.matWorld_);
			    break;
		    case Stage::Stage3:
			    AttackUpdate(worldTransformBody3_.matWorld_);
			    break;
		    case Stage::Stage4:
			    AttackUpdate(worldTransformHead_.matWorld_);
			    break;
		    }
			break;
	    case Behavior::kBreak:

			switch (stage_) {
				case Stage::Stage1:

					break;
		        case Stage::Stage2:
			        Falling(worldTransformBody2_.translation_);
					break;
		        case Stage::Stage3:
			        Falling(worldTransformBody3_.translation_);
					break;
		        case Stage::Stage4:
			        Falling(worldTransformHead_.translation_);
					break;
			}

			break;
	}

	if (bulletCoolTimer > 0) {
		    bulletCoolTimer--;
	} else {
		    bulletCoolTimer = 0;
	}

	worldTransformBody_.rotation_.y += 0.01f;
	worldTransformBody2_.rotation_.y -= 0.01f;
	worldTransformBody3_.rotation_.y += 0.02f;
	worldTransformHead_.rotation_.y -= 0.02f;



	if (hp_ == 150 && stage_ != Stage::Stage2) {
		    stageRequest_ = Stage::Stage2;
	}
	if (hp_ == 100 && stage_ != Stage::Stage3) {
		    stageRequest_ = Stage::Stage3;
	}
	if (hp_ == 50 && stage_ != Stage::Stage4) {
		    stageRequest_ = Stage::Stage4;
	}
	if (hp_ <= 0) {
		gameScene_->ToClearScene();
	}

	BaseCharacter::Update();
	worldTransformBody2_.UpdateMatrix();
	worldTransformBody3_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
}

void Enemy::Draw(const ViewProjection& viewProjection) {

	if (hp_ > 150) {
		models_[0]->Draw(worldTransformBody_, viewProjection);
	}
	if (hp_ > 100) {
		models_[1]->Draw(worldTransformBody2_, viewProjection);
	}
	if (hp_ > 50) {
		models_[2]->Draw(worldTransformBody3_, viewProjection);
	}
	if (hp_ > 0) {
		models_[3]->Draw(worldTransformHead_, viewProjection);
	}
	

}

void Enemy::OnCollision() {
	hp_ -= 1;
}

void Enemy::ApplyGlobalVariables() {

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Enemy";

	worldTransformBase_.translation_ = globalVariables->GetVec3Value(groupName, "Base Translation");
	maxHp = globalVariables->GetIntValue(groupName, "HP");
	size = globalVariables->GetVec3Value(groupName, "Size");
	bulletCoolTime = globalVariables->GetIntValue(groupName, "Bullet coolTime");
	attackCoolTime = globalVariables->GetIntValue(groupName, "Attack coolTime");
	attackTime = globalVariables->GetIntValue(groupName, "AttackTime");

}

void Enemy::RootInitialize() {

	attackCoolTimer = attackCoolTime;

}

void Enemy::RootUpdate() {

	if (--attackCoolTimer == 0) {
		behaviorRequest_ = Behavior::kAttack;
	}

}

void Enemy::AttackInitialize() {

	attackTimer = attackTime;

}

void Enemy::AttackUpdate(const Matrix44& worldMatrix) {

	if (--attackTimer <= 0) {
		behaviorRequest_ = Behavior::kRoot;
		return;
	}

	if (bulletCoolTimer != 0) {
		return;
	}

	
	const int shotNum = 6;
	Vec3 shotPos[shotNum] = {};
	Matrix44 worldmatrix[shotNum] = {};
	float degree = 0.0f;

	for (size_t i = 0; i < shotNum; i++) {
		shotPos[i] = {1.0f, 0.0f, 0.0f};
		degree = (360.0f / (float)shotNum) * (float)i;
		float radian = degree * ((float)std::numbers::pi / 180.0f);
		shotPos[i] = TransformNormal(shotPos[i], MakeRotateYMatrix(radian));
	}

	for (size_t i = 0; i < shotNum; i++) {
		worldmatrix[i] = MakeAffineMatrix({1.0f, 1.0f, 1.0f}, {}, shotPos[i]) * worldMatrix;
		shotPos[i] = {worldmatrix[i].m[3][0], worldmatrix[i].m[3][1], worldmatrix[i].m[3][2]};
	}

	const float kBulletSpeed = 0.5f;
	Vec3 velocity = {0.0f, 0.0f, kBulletSpeed};

	for (size_t i = 0; i < shotNum; i++) {

		velocity = shotPos[i].Normalize() * kBulletSpeed;
		velocity.y = 0.0f;

		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(models_[4], GetWorldPos() + shotPos[i], velocity);
		gameScene_->AddEnemyBullet(newBullet);
	}
	bulletCoolTimer = bulletCoolTime;
}

void Enemy::FallingInitialize(const Vec3& translation) {
	fallStart = translation;
	fallEnd = {};
	num = 0.0f;
}

void Enemy::Falling(Vec3& translation) {

	num += 0.05f;
	
	translation = Lerp(num, fallStart, fallEnd);

	if (num >= 1.0f) {
		num = 1.0f;
	}

	if (translation == fallEnd) {
		behaviorRequest_ = Behavior::kRoot;
	}
}

void Enemy::SceneInitialize() {

	hp_ = maxHp;
	behaviorRequest_ = Behavior::kRoot;
	worldTransformBody2_.translation_ = {0.0f, 6.0f, 0.0f};
	worldTransformBody3_.translation_ = {0.0f, 6.0f, 0.0f};
	worldTransformHead_.translation_ = {0.0f, 6.0f, 0.0f};

	worldTransformBody_.rotation_.y = 0.0f;
	worldTransformBody2_.rotation_.y = 0.0f;
	worldTransformBody3_.rotation_.y = 0.0f;
	worldTransformHead_.rotation_.y = 0.0f;

	stage_ = Stage::Stage1;

}