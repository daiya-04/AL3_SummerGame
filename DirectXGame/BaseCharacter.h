#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include <vector>

class BaseCharacter {
protected:
	//モデルデータ
	std::vector<Model*> models_;
	//ワールド変換データ
	WorldTransform worldTransformBase_;
	WorldTransform worldTransformBody_;

public:

	virtual void Initialize(const std::vector<Model*>& models);

	virtual void Update();

	virtual void Draw(const ViewProjection& viewProjection);


	const WorldTransform& GetWorlfTransformBase() { return worldTransformBase_; }
	const WorldTransform& GetWorldTransformBody() { return worldTransformBody_; }

};
