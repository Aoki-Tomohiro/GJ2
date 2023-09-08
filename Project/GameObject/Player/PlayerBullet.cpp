#include "PlayerBullet.h"
#include "MathFunction.h"
#include "CollisionManager/CollisionConfig.h"

PlayerBullet::~PlayerBullet() {
	delete model_;
}

void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity) {
	//モデルの初期化
	assert(model);
	model_ = new Model();
	model_->CreateSphere();
	//ワールドトランスフォームの初期化
	worldTransform_.translation_ = position;
	worldTransform_.translation_.y += 1.0f;
	//速度の初期化
	velocity_ = velocity;
	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayerBullet);
	//衝突対象を自分の属性以外に設定
	SetCollisionMask(~kCollisionAttributePlayerBullet);
}

void PlayerBullet::Update() {
	//ワールドトランスフォーム
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	//弾を進行方向に向ける
	worldTransform_.rotation_.y = std::atan2(velocity_.x, velocity_.z);
	//ワールドトランスフォーム
	worldTransform_.UpdateMatrix();

	//デスフラグが０以下になった時に弾を削除する
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}
}

void PlayerBullet::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection);
}

Vector3 PlayerBullet::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];
	return worldPos;
}

void PlayerBullet::OnCollision() {
	isDead_ = true;
}