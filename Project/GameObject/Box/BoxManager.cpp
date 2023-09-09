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
			if (player_->GetIsBoxPush() == false) {
				player_->OnCollisionBox();
				box->OnCollisionPlayer(player_->GetVelocity());
			}
			//else {
			//	Vector3 velocity = Normalize(Subtract(playerPos, boxPos));
			//	float length = 2.0f - Length(Subtract(boxPos, playerPos));
			//	velocity = Multiply(velocity, length);
			//	player_->Move(velocity);
			//}
		}
	}

	//すべての箱の当たり判定
	std::list<std::unique_ptr<Box>>::iterator itrA = boxs_.begin();
	for (; itrA != boxs_.end(); ++itrA) {
		Box* boxA = itrA->get();
		Vector3 boxAPos = boxA->GetWorldPosition();
		std::list<std::unique_ptr<Box>>::iterator itrB = itrA;
		itrB++;
		for (; itrB != boxs_.end(); ++itrB) {
			Box* boxB = itrB->get();
			Vector3 boxBPos = boxB->GetWorldPosition();

			if (boxAPos.x - 1.0f <= boxBPos.x + 1.0f && boxAPos.x + 1.0f >= boxBPos.x - 1.0f &&
				boxAPos.y - 1.0f <= boxBPos.y + 1.0f && boxAPos.y + 1.0f >= boxBPos.y - 1.0f &&
				boxAPos.z - 1.0f <= boxBPos.z + 1.0f && boxAPos.z + 1.0f >= boxBPos.z - 1.0f) {
				Vector3 boxAVelocity = Normalize(Subtract(boxAPos, boxBPos));
				float boxALength = 2.0f - Length(Subtract(boxBPos, boxAPos));
				boxAVelocity = Multiply(boxAVelocity, boxALength);
				if (boxA->GetOnPlayer()) {
					boxA->OnCollisionBox(boxAVelocity);
					player_->Move(boxAVelocity);
				}
				Vector3 boxBVelocity = Normalize(Subtract(boxBPos, boxAPos));
				float boxBLength = 2.0f - Length(Subtract(boxAPos, boxBPos));
				boxBVelocity = Multiply(boxBVelocity, boxBLength);
				if (boxB->GetOnPlayer()) {
					boxB->OnCollisionBox(boxBVelocity);
					player_->Move(boxBVelocity);
				}
			}
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