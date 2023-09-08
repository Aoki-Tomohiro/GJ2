#include "FollowCamera.h"
#include "Input/Input.h"
#include "ImGuiManager/ImGuiManager.h"

void FollowCamera::Update() {
	//追従対象がいれば
	if (target_) {
		//追従対象の補間
		interTarget_ = Lerp(interTarget_, target_->translation_, 0.1f);
	}

	//追従対象からのオフセット
	Vector3 offset = Offset();
	//カメラ座標
	viewProjection_.translation_ = Add(interTarget_, offset);

	//旋回操作
	XINPUT_STATE joyState{};
	if (Input::GetInstance()->GetJoystickState(joyState)) {
		//しきい値
		const float threshold = 0.7f;
		//回転フラグ
		bool isRotation = false;
		//回転量
		Vector3 rotation = {
			(float)joyState.Gamepad.sThumbRY / SHRT_MAX,
			(float)joyState.Gamepad.sThumbRX / SHRT_MAX,
			0.0f
		};

		//スティックの押し込みが遊び範囲を超えていたら回転フラグをtureにする
		if (Length(rotation) > threshold) {
			isRotation = true;
		}

		if (isRotation) {
			//回転速度
			const float kRotSpeedX = 0.04f;
			const float kRotSpeedY = 0.06f;
			//目標角度の算出
			destinationAngleX_ -= rotation.x * kRotSpeedX;
			destinationAngleY_ += rotation.y * kRotSpeedY;
		}
	}

	if (isRockon_) {
		if (rockonTarget_ != nullptr && target_ != nullptr) {
			Vector3 move = Normalize(Subtract(rockonTarget_->translation_, target_->translation_));
			destinationAngleY_ = std::atan2(move.x, move.z);
			float length = Length(Vector3{ move.x, 0, move.z });
			destinationAngleX_ = std::atan2(-move.y, length);
		}
	}
	else {
		if (destinationAngleX_ >= maxRotateX) {
			destinationAngleX_ = maxRotateX;
		}
		else if (destinationAngleX_ <= minRotateX) {
			destinationAngleX_ = minRotateX;
		}
	}

	//最短角度補間
	viewProjection_.rotation_.x = LerpShortAngle(viewProjection_.rotation_.x, destinationAngleX_, 0.02f);
	viewProjection_.rotation_.y = LerpShortAngle(viewProjection_.rotation_.y, destinationAngleY_, 0.04f);

	//ビュー行列の計算
	viewProjection_.UpdateMatrix();

	ImGui::Begin("Rockon");
	ImGui::Checkbox("isRockon", &isRockon_);
	ImGui::DragFloat("maxRotate", &maxRotateX, 0.01f);
	ImGui::DragFloat("minRotate", &minRotateX, 0.01f);
	ImGui::End();
}

Vector3 FollowCamera::Offset() const{
	//追従対象からのオフセット
	Vector3 offset = { 0.0f,2.0f,-30.0f };
	//回転行列の合成
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(viewProjection_.rotation_.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(viewProjection_.rotation_.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(viewProjection_.rotation_.z);
	Matrix4x4 rotateMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));
	//オフセットをカメラの回転に合わせて回転させる
	offset = TransformNormal(offset, rotateMatrix);

	return offset;
}

void FollowCamera::Reset() {
	//追従対象がいれば
	if (target_) {
		//追従座標・角度の初期化
		interTarget_ = target_->translation_;
		viewProjection_.rotation_.x = target_->rotation_.x;
		viewProjection_.rotation_.y = target_->rotation_.y;
	}
	destinationAngleX_ = viewProjection_.rotation_.x;
	destinationAngleY_ = viewProjection_.rotation_.y;

	//追従対象からのオフセット
	Vector3 offset = FollowCamera::Offset();
	viewProjection_.translation_ = Add(interTarget_, offset);
}

void FollowCamera::SetTarget(const WorldTransform* target) {
	target_ = target;
	FollowCamera::Reset();
}

void FollowCamera::SetRockonTarget(const WorldTransform* rockonTarget) {
	rockonTarget_ = rockonTarget;
	FollowCamera::Reset();
}