#pragma once
#include "BaseCharacter.h"
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Vec3.h"
#include "Matrix44.h"

class Enemy : public BaseCharacter {
private:

	WorldTransform worldTransformBody2_;

public:

	void Initialize(const std::vector<Model*>& models) override;

	void Update() override;

	void Draw(const ViewProjection& viewProjection) override;

	void ApplyGlobalVariables();

};
