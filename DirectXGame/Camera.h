#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Vec3.h"




class Camera {
private:

	ViewProjection viewProjection_;
	const WorldTransform* target_;

public:

	void Initialize();

	void Update();

	void SetTarget(const WorldTransform* target) { target_ = target; }
	ViewProjection& GetViewProjection() { return viewProjection_; }

};
