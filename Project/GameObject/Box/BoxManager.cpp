#include "BoxManager.h"
#include "../GameObject/Player/Player.h"
#include <ctime>

void BoxManager::Initialize() {
	std::srand(unsigned int(time(nullptr)));
}

void BoxManager::Update() {
	if (--timer_ <= 0) {
		timer_ = boxSpornTime;
		AddBox();
	}

	//自キャラと箱の当たり判定
	Vector3 playerPos = player_->GetWorldPosition();
	for (const std::unique_ptr<Box>& box : boxs_) {
		Vector3 boxPos = box->GetWorldPosition();
		if (playerPos.x - 1.0f <= boxPos.x + 1.0f && playerPos.x + 1.0f >= boxPos.x - 1.0f &&
			playerPos.y - 1.0f <= boxPos.y + 1.0f && playerPos.y + 1.0f >= boxPos.y - 1.0f &&
			playerPos.z - 1.0f <= boxPos.z + 1.0f && playerPos.z + 1.0f >= boxPos.z - 1.0f) {
			player_->OnCollisionBox();
			box->OnCollision(player_->GetVelocity());
		}
	}

	//箱の更新
	for (const std::unique_ptr<Box>& box : boxs_) {
		box->Update();
	}
}

void BoxManager::Draw(const ViewProjection& viewProjection) {
	//箱の描画
	for (const std::unique_ptr<Box>& box : boxs_) {
		box->Draw(viewProjection);
	}
}

void BoxManager::AddBox() {
	//箱の作成
	Box* box = new Box();
	//箱の初期化
	box->Initialize();
	//箱をリスト追加
	boxs_.push_back(std::unique_ptr<Box>(box));
}