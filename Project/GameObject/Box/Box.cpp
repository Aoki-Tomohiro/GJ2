#include "Box.h"
#include "BoxConfig.h"
#include "TextureManager/TextureManager.h"
#include "ImGuiManager/ImGuiManager.h"

void Box::Initialize(uint32_t attribute) {
	//モデルの初期化
	model_ = std::make_unique<Model>();
	model_->CreateSphere();
	//テクスチャの読み込み
	textureHandle_ = TextureManager::GetInstance()->Load("Project/Resources/white.png");
	//ワールドトランスフォームの初期化
	int posNum = rand() % 4;
	if (posNum == kLeftTop) {
		worldTransform_.translation_.x = float(rand() % 21 - 10) - 40.0f;
		worldTransform_.translation_.y = 1.0f;
		worldTransform_.translation_.z = float(rand() % 21 - 10) + 40.0f;
	}
	else if (posNum == kRightTop) {
		worldTransform_.translation_.x = float(rand() % 21 - 10) + 40.0f;
		worldTransform_.translation_.y = 1.0f;
		worldTransform_.translation_.z = float(rand() % 21 - 10) + 40.0f;
	}
	else if (posNum == kLeftBottom) {
		worldTransform_.translation_.x = float(rand() % 21 - 10) - 40.0f;
		worldTransform_.translation_.y = 1.0f;
		worldTransform_.translation_.z = float(rand() % 21 - 10) - 40.0f;
	}
	else if (posNum == kRightBottom) {
		worldTransform_.translation_.x = float(rand() % 21 - 10) + 40.0f;
		worldTransform_.translation_.y = 1.0f;
		worldTransform_.translation_.z = float(rand() % 21 - 10) - 40.0f;
	}
	//属性の初期化
	attribute_ = attribute;
	if (attribute_ == kBoxAttributeRed) {
		model_->GetMaterial()->color_ = { 1.0f,0.0f,0.0f,1.0f };
		model_->GetMaterial()->Update();
	}
	else if (attribute_ == kBoxAttributeBlue) {
		model_->GetMaterial()->color_ = { 0.0f,0.0f,1.0f,1.0f };
		model_->GetMaterial()->Update();
	}
	else if (attribute_ == kBoxAttributeGreen) {
		model_->GetMaterial()->color_ = { 0.0f,1.0f,0.0f,1.0f };
		model_->GetMaterial()->Update();
	}
}

void Box::Update() {
	onPlayer_ = false;
	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
	ImGui::Begin("Boxs");
	ImGui::Text("boxAttribute : %d", attribute_);
	ImGui::End();
}

void Box::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
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

void Box::OnCollisionPlayer(const Vector3& velocity) {
	onPlayer_ = true;
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity);
}

void Box::OnCollisionBox(const Vector3& velocity) {
	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity);
}