#include "FollowCamera.h"
#include "Input/Input.h"
#include "ImGuiManager/ImGuiManager.h"

void FollowCamera::Initialize(const Vector3& offset) {
	//オフセットの初期化
	normalOffset_ = offset;
}

void FollowCamera::Update(Behavior behavior) {
	//自キャラの状態を取得
	playerBehavior_ = behavior;

	//追従対象がいれば
	if (target_) {
		//追従対象の補間
		if (playerBehavior_ != Behavior::kAttack) {
			interTarget_ = Lerp(interTarget_, target_->translation_, 0.1f);
		}
		else {
			interTarget_ = Lerp(interTarget_, target_->translation_, 0.4f);
		}
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
		const float thresholdAttack = 0.2f;
		//回転フラグ
		bool isRotation = false;
		//回転量
		Vector3 rotation = {
			(float)joyState.Gamepad.sThumbRY / SHRT_MAX,
			(float)joyState.Gamepad.sThumbRX / SHRT_MAX,
			0.0f
		};

		//自キャラが攻撃状態ではないときにデッドゾーンを有効にする
		if (playerBehavior_ != Behavior::kAttack) {
			//スティックの押し込みが遊び範囲を超えていたら回転フラグをtureにする
			if (Length(rotation) > threshold) {
				isRotation = true;
			}
		}
		else {
			//スティックの押し込みが遊び範囲を超えていたら回転フラグをtureにする
			if (Length(rotation) > thresholdAttack) {
				isRotation = true;
			}
		}

		if (isRotation) {
			if (playerBehavior_ != Behavior::kAttack) {
				//回転速度
				const float kRotSpeedX = 0.02f;
				const float kRotSpeedY = 0.04f;
				//目標角度の算出
				destinationAngleX_ -= rotation.x * kRotSpeedX;
				destinationAngleY_ += rotation.y * kRotSpeedY;
			}
			else {
				//回転速度
				const float kRotSpeedX = 0.008f;
				const float kRotSpeedY = 0.01f;
				//目標角度の算出
				destinationAngleX_ -= rotation.x * kRotSpeedX;
				destinationAngleY_ += rotation.y * kRotSpeedY;
			}
		}
	}

	//角度制限
	if (playerBehavior_ != Behavior::kAttack) {
		minRotateX = -0.16f;
		maxRotateX = 0.2f;
	}
	else {
		minRotateX = -0.2f;
		maxRotateX = 0.2f;
	}

	if (destinationAngleX_ >= maxRotateX) {
		destinationAngleX_ = maxRotateX;
	}
	else if (destinationAngleX_ <= minRotateX) {
		destinationAngleX_ = minRotateX;
	}


	//最短角度補間
	if (playerBehavior_ != Behavior::kAttack) {
		viewProjection_.rotation_.x = LerpShortAngle(viewProjection_.rotation_.x, destinationAngleX_, 0.2f);
		viewProjection_.rotation_.y = LerpShortAngle(viewProjection_.rotation_.y, destinationAngleY_, 0.2f);
	}
	else {
		viewProjection_.rotation_.x = LerpShortAngle(viewProjection_.rotation_.x, destinationAngleX_, 0.4f);
		viewProjection_.rotation_.y = LerpShortAngle(viewProjection_.rotation_.y, destinationAngleY_, 0.4f);
	}

	//ビュー行列の計算
	viewProjection_.UpdateMatrix();

	ImGui::Begin("Rockon");
	ImGui::DragFloat("maxRotate", &maxRotateX, 0.01f);
	ImGui::DragFloat("minRotate", &minRotateX, 0.01f);
	ImGui::End();
}

Vector3 FollowCamera::Offset() {
	//追従対象からのオフセット
	if (playerBehavior_ != Behavior::kAttack) {
		offset_ = Lerp(offset_, normalOffset_, 0.1f);
	}
	else {
		offset_ = Lerp(offset_, aimOffset_, 0.1f);
	}
	Vector3 offset = offset_;
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