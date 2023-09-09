#include "BoxManager.h"
#include "../GameObject/Player/Player.h"
#include <ctime>

void BoxManager::Initialize() {
	//モデルの初期化
	model_ = std::make_unique<Model>();
	model_->CreateFromOBJ("Project/Resources", "plane.obj");
	//ワールドトランスフォームの初期化
	worldTransform_.translation_.y = 0.1f;
	worldTransform_.rotation_.x = 3.14159265359f / 2.0f;
	worldTransform_.scale_ = { 2.0f,2.0f,2.0f };
	//ランド関数の初期化
	std::srand(unsigned int(time(nullptr)));
}

void BoxManager::Update() {
	//箱を出現させる
	if (--timer_ <= 0) {
		timer_ = boxSpornTime;
		AddBox();
	}

	worldTransform_.UpdateMatrix();

	//箱の更新
	for (const std::unique_ptr<Box>& box : boxs_) {
		box->Update();
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
		//箱Aの座標を取得
		Vector3 boxAPos = boxA->GetWorldPosition();
		std::list<std::unique_ptr<Box>>::iterator itrB = itrA;
		itrB++;
		for (; itrB != boxs_.end(); ++itrB) {
			Box* boxB = itrB->get();
			//箱Bの座標を取得
			Vector3 boxBPos = boxB->GetWorldPosition();
			//箱の当たり判定
			if (boxAPos.x - 1.0f <= boxBPos.x + 1.0f && boxAPos.x + 1.0f >= boxBPos.x - 1.0f &&
				boxAPos.y - 1.0f <= boxBPos.y + 1.0f && boxAPos.y + 1.0f >= boxBPos.y - 1.0f &&
				boxAPos.z - 1.0f <= boxBPos.z + 1.0f && boxAPos.z + 1.0f >= boxBPos.z - 1.0f) {
				//箱の移動方向を計算
				Vector3 boxAVelocity = Normalize(Subtract(boxAPos, boxBPos));
				//箱の大きさから箱Aと箱Bの長さを引く
				float boxALength = 2.0f - Length(Subtract(boxBPos, boxAPos));
				//箱の移動量を計算
				boxAVelocity = Multiply(boxAVelocity, boxALength);
				//箱Aが自キャラと当たっていたら
				if (boxA->GetOnPlayer()) {
					//箱Aを当たった分だけ押し戻す
					boxA->OnCollisionBox(boxAVelocity);
					//自キャラを箱Aの移動量だけ戻す
					player_->Move(boxAVelocity);
				}
				//箱の移動方向を計算
				Vector3 boxBVelocity = Normalize(Subtract(boxBPos, boxAPos));
				//箱の大きさから箱Aと箱Bの長さを引く
				float boxBLength = 2.0f - Length(Subtract(boxAPos, boxBPos));
				//箱の移動量を計算
				boxBVelocity = Multiply(boxBVelocity, boxBLength);
				//箱Bが自キャラと当たっていたら
				if (boxB->GetOnPlayer()) {
					//箱Bを当たった分だけ押し戻す
					boxB->OnCollisionBox(boxBVelocity);
					//自キャラを箱Bの移動量だけ戻す
					player_->Move(boxBVelocity);
				}
			}
		}
	}

	//箱を特定の場所に運んだら自キャラをパワーアップ

}

void BoxManager::Draw(const ViewProjection& viewProjection) {
	//エリアの描画
	model_->Draw(worldTransform_, viewProjection);
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