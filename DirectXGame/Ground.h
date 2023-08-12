#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "Model.h"


class Ground {
private:

	//ワールドトランスフォーム
	WorldTransform worldTransform_;
	//モデルデータ
	Model* model_ = nullptr;

public:

	void Initialize(Model* model);

	void Update();

	void Draw(const ViewProjection& viewProjection);

};
