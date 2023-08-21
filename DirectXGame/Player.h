#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "BaseCharacter.h"
#include "Input.h"

class Player : public BaseCharacter {
private:

	WorldTransform worldTransformHead_;
	WorldTransform worldTransformL_Arm_;
	WorldTransform worldTransformR_Arm_;

	Vec3 rotate;

public:

	void Initialize(const std::vector<Model*>& models) override;

	void Update() override;

	void Draw(const ViewProjection& viewProjection) override;

	void ApplyGlobalVariables();

};
