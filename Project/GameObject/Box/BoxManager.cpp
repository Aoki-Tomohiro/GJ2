#include "BoxManager.h"
#include "BoxConfig.h"
#include "../GameObject/Player/Player.h"
#include "TextureManager/TextureManager.h"
#include "ImGuiManager/ImGuiManager.h"
#include <ctime>

void BoxManager::Initialize() {
	//ランド関数の初期化
	std::srand(unsigned int(time(nullptr)));
	//テクスチャの読み込み
	textureHandle_ = TextureManager::GetInstance()->Load("Resources/white.png");
	//エリアの初期化
	for (int i = 0; i < areas_.size(); i++) {
		//モデルの作成
		areas_[i].model = std::make_unique<Model>();
		areas_[i].model->CreateFromOBJ("Resources", "plane.obj");
		//ワールドトランスフォームの初期化
		areas_[i].worldTransform.translation_ = { i * 4.0f - 4.0f,0.1f,0.0f };
		areas_[i].worldTransform.rotation_.x = 3.14159265359f / 2.0f;
		areas_[i].worldTransform.scale_ = { 2.0f,2.0f,2.0f };
		//属性の初期化
		uint32_t attribute = rand() % 3;
		/*uint32_t attribute = i;*/
		if (attribute == kRed) {
			areas_[i].kAreaAttribute = kAreaAttributeRed;
			areas_[i].model->GetMaterial()->color_ = { 1.0f,0.0f,0.0f,1.0f };
			areas_[i].model->GetMaterial()->Update();
		}
		else if (attribute == kBlue) {
			areas_[i].kAreaAttribute = kAreaAttributeBlue;
			areas_[i].model->GetMaterial()->color_ = { 0.0f,0.0f,1.0f,1.0f };
			areas_[i].model->GetMaterial()->Update();
		}
		else if (attribute == kGreen) {
			areas_[i].kAreaAttribute = kAreaAttributeGreen;
			areas_[i].model->GetMaterial()->color_ = { 0.0f,1.0f,0.0f,1.0f };
			areas_[i].model->GetMaterial()->Update();
		}
	}

	powerUpSE_ = Audio::GetInstance();
	powerUpSEHandle_ = powerUpSE_->SoundLoadWave("Resources/Music/SE/PowerUp/PowerUp1.wav");


}

void BoxManager::Update() {
	//死亡フラグが立っている箱を削除
	boxs_.remove_if([](std::unique_ptr<Box>& box) {
		if (box->GetIsDead()) {
			box.reset();
			return true;
		}
		return false;
		});

	//エリア内の箱の数を初期化する
	inBoxCount_ = 0;

	//エリアのワールドトランスフォームの更新
	for (int i = 0; i < areas_.size(); i++) {
		areas_[i].worldTransform.UpdateMatrix();
	}

	//箱を出現させる
	AddBox();

	//箱の更新
	for (const std::unique_ptr<Box>& box : boxs_) {
		box->Update();
	}

	//当たり判定
	CheckCollision();

	//エリアにある箱を確認
	CheckArea();
}

void BoxManager::Draw(const ViewProjection& viewProjection) {
	//エリアの描画
	for (int i = 0; i < areas_.size(); i++) {
		areas_[i].model->Draw(areas_[i].worldTransform, viewProjection, textureHandle_);
	}
	//箱の描画
	for (const std::unique_ptr<Box>& box : boxs_) {
		box->Draw(viewProjection);
	}
}

void BoxManager::AddBox() {
	//箱を出現させる
	if (boxCount_ < 9 && player_->GetIsEnhancedState() == false) {
		if (--timer_ <= 0) {
			//箱の作成
			Box* box = new Box();
			uint32_t boxAttribute_ = rand() % 3;
			//箱の初期化
			if (boxAttribute_ == kRed) {
				//赤い箱が３つないとき
				if (redBoxCount_ < 3) {
					box->Initialize(kBoxAttributeRed);
					redBoxCount_++;
				}
				//赤い箱が３つあるとき
				else {
					//もう一度ランダムに属性を決める
					boxAttribute_ = rand() % 2 + 1;
					if (boxAttribute_ == kBlue) {
						//青い箱が３つないとき
						if (blueBoxCount_ < 3) {
							//青い箱を出現させる
							box->Initialize(kBoxAttributeBlue);
							blueBoxCount_++;
						}
						//緑の箱が３つないとき
						else if (greenBoxCount_ < 3) {
							box->Initialize(kBoxAttributeGreen);
							greenBoxCount_++;
						}
					}
					else if (boxAttribute_ == kGreen) {
						//緑の箱が３つないとき
						if (greenBoxCount_ < 3) {
							box->Initialize(kBoxAttributeGreen);
							greenBoxCount_++;
						}
						//青い箱が３つないとき
						else if (blueBoxCount_ < 3) {
							//青い箱を出現させる
							box->Initialize(kBoxAttributeBlue);
							blueBoxCount_++;
						}
					}
				}
			}
			else if (boxAttribute_ == kBlue) {
				//青い箱が３つないとき
				if (blueBoxCount_ < 3) {
					box->Initialize(kBoxAttributeBlue);
					blueBoxCount_++;
				}
				//青い箱が３つあるとき
				else {
					//もう一度ランダムに属性を決める
					boxAttribute_ = rand() % 2;
					if (boxAttribute_ == 0) {
						//赤い箱が３つないとき
						if (redBoxCount_ < 3) {
							//赤い箱を出現させる
							box->Initialize(kBoxAttributeRed);
							redBoxCount_++;
						}
						//緑の箱が３つないとき
						else if (greenBoxCount_ < 3) {
							box->Initialize(kBoxAttributeGreen);
							greenBoxCount_++;
						}
					}
					else if (boxAttribute_ == 1) {
						//緑の箱が３つないとき
						if (greenBoxCount_ < 3) {
							box->Initialize(kBoxAttributeGreen);
							greenBoxCount_++;
						}
						//赤い箱が３つないとき
						else if (redBoxCount_ < 3) {
							//赤い箱を出現させる
							box->Initialize(kBoxAttributeRed);
							redBoxCount_++;
						}
					}
				}
			}
			else if (boxAttribute_ == kGreen) {
				//緑の箱が３つないとき
				if (greenBoxCount_ < 3) {
					box->Initialize(kBoxAttributeGreen);
					greenBoxCount_++;
				}
				//緑の箱が３つあるとき
				else {
					//もう一度ランダムに属性を決める
					boxAttribute_ = rand() % 2;
					if (boxAttribute_ == kRed) {
						//赤い箱が３つないとき
						if (redBoxCount_ < 3) {
							//赤い箱を出現させる
							box->Initialize(kBoxAttributeRed);
							redBoxCount_++;
						}
						//青い箱が３つないとき
						else if (blueBoxCount_ < 3) {
							box->Initialize(kBoxAttributeBlue);
							blueBoxCount_++;
						}
					}
					else if (boxAttribute_ == kBlue) {
						//青い箱が３つないとき
						if (blueBoxCount_ < 3) {
							box->Initialize(kBoxAttributeBlue);
							blueBoxCount_++;
						}
						//赤い箱が３つないとき
						else if (redBoxCount_ < 3) {
							//赤い箱を出現させる
							box->Initialize(kBoxAttributeRed);
							redBoxCount_++;
						}
					}
				}
			}
			//箱をリスト追加
			boxs_.push_back(std::unique_ptr<Box>(box));
			//タイマーを戻す
			timer_ = boxSpornTime;
			//箱の数を増やす
			boxCount_++;
		}
	}
}

void BoxManager::CheckCollision() {
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

	//箱とエリアの当たり判定
	for (int i = 0; i < areas_.size(); i++) {
		for (const std::unique_ptr<Box>& box : boxs_) {
			//同じ属性同士に当たり判定をつける
			if ((areas_[i].kAreaAttribute & box->GetAttribute()) != 0) {
				Vector3 pos = box->GetWorldPosition();
				if (areas_[i].worldTransform.translation_.x - areaSize_.x <= pos.x + 1.0f && areas_[i].worldTransform.translation_.x + areaSize_.x >= pos.x - 1.0f &&
					areas_[i].worldTransform.translation_.y - areaSize_.y <= pos.y + 1.0f && areas_[i].worldTransform.translation_.y + areaSize_.y >= pos.y - 1.0f &&
					areas_[i].worldTransform.translation_.z - areaSize_.z <= pos.z + 1.0f && areas_[i].worldTransform.translation_.z + areaSize_.z >= pos.z - 1.0f) {
					box->SetInArea(true);
					inBoxCount_++;
				}
			}
		}
	}
}

void BoxManager::CheckArea() {
	//エリア内に箱が3つある時自キャラを強くする
	if (inBoxCount_ >= 3) {
		//自キャラを強化状態にする
		player_->SetEnhanced();

		//効果音もなるよ
		powerUpSE_->SoundPlayWave(powerUpSEHandle_, 0);


		//箱を消す
		for (const std::unique_ptr<Box>& box : boxs_) {
			if (box->GetInArea()) {
				box->SetIsDead(true);
				boxCount_--;
				uint32_t attribute = box->GetAttribute();
				if (attribute == kBoxAttributeRed) {
					redBoxCount_--;
				}
				else if (attribute == kBoxAttributeBlue) {
					blueBoxCount_--;
				}
				else if (attribute == kBoxAttributeGreen) {
					greenBoxCount_--;
				}
			}
		}
		//エリアの属性をランダムに変更
		for (int i = 0; i < areas_.size(); i++) {
			uint32_t attribute = rand() % 3;
			if (attribute == kRed) {
				areas_[i].kAreaAttribute = kAreaAttributeRed;
				areas_[i].model->GetMaterial()->color_ = { 1.0f,0.0f,0.0f,1.0f };
				areas_[i].model->GetMaterial()->Update();
			}
			else if (attribute == kBlue) {
				areas_[i].kAreaAttribute = kAreaAttributeBlue;
				areas_[i].model->GetMaterial()->color_ = { 0.0f,0.0f,1.0f,1.0f };
				areas_[i].model->GetMaterial()->Update();
			}
			else if (attribute == kGreen) {
				areas_[i].kAreaAttribute = kAreaAttributeGreen;
				areas_[i].model->GetMaterial()->color_ = { 0.0f,1.0f,0.0f,1.0f };
				areas_[i].model->GetMaterial()->Update();
			}
		}
	}
}