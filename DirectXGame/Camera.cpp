#include "Camera.h"
#include <cmath>

void Camera::Initialize() {

	viewProjection_.Initialize();

}

void Camera::Update() {

	if (target_) {

		Vec3 offset = {0.0f, 15.0f, -40.0f};
		Vec3 origin{};

		Vec3 targetWorldPos = { target_->matWorld_.m[3][0], target_->matWorld_.m[3][1], target_->matWorld_.m[3][2] };

		Vec3 rotate = (origin - targetWorldPos).Normalize();

		viewProjection_.rotation_.y = std::atan2(rotate.x, rotate.z);

		offset = TransformNormal(offset, MakeRotateYMatrix(viewProjection_.rotation_.y));

		viewProjection_.translation_ = target_->translation_ + offset;

	}


	viewProjection_.UpdateViewMatrix();
}