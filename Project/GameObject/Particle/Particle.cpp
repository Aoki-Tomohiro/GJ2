#include "Particle.h"
#include "TextureManager/TextureManager.h"

void Particle::Initialize(const Vector3& position) {
	model_ = std::make_unique<Model>();
	model_->CreateFromOBJ("Project/Resources/EnemyObj/TransCube", "TransCube.obj");
	//ワールドトランスフォームの初期化
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.x = float(rand() % 315) / 100;
	worldTransform_.rotation_.y = float(rand() % 315) / 100;
	worldTransform_.rotation_.z = float(rand() % 315) / 100;
	worldTransform_.scale_.x = 0.4f;
	worldTransform_.scale_.y = 0.4f;
	worldTransform_.scale_.z = 0.4f;
	//速度をランダムに決める
	velocity_.x = float(rand() % 17 - 8) / 10;
	velocity_.y = float(rand() % 21 + 10) / 10;
	/*velocity_.y = float(rand() % 11 + 5) / 10;*/
	velocity_.z = float(rand() % 17 - 8) / 10;
}

void Particle::Update() {
	//デスタイムが０以下になったらデスフラグを立てる
	if (--dethTimer_ < 0) {
		isDead_ = true;
	}
	velocity_.y -= 0.1f;
	//ワールドトランスフォームに速度を足す
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void Particle::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection);
}