#include "Box.h"

void Box::Initialize() {
	//モデルの初期化
	model_ = std::make_unique<Model>();
	model_->CreateSphere();
	//ワールドトランスフォームの初期化
	worldTransform_.translation_.x = float(rand() % 101 - 50);
	worldTransform_.translation_.y = 1.0f;
	worldTransform_.translation_.z = float(rand() % 101 - 50);
}

void Box::Update() {
	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void Box::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection);
}

Vector3 Box::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void Box::OnCollision(const Vector3& move) {
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
}