#include "Player.h"
#include "PlayerBullet.h"
#include "GameScene/GameScene.h"
#include "../GameObject/Enemy/TransCube/TransCube.h"
#include "Input/Input.h"
#include "CollisionManager/CollisionConfig.h"
#include "ImGuiManager/ImGuiManager.h"

void Player::Initialize(const std::vector<Model*> models) {
	//モデルの初期化
	models_ = models;
	//ワールドトランスフォームの初期化
	worldTransformHead_.parent_ = &worldTransformBase_;
	worldTransformBody_.parent_ = &worldTransformBase_;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.translation_.x = -0.5f;
	worldTransformL_arm_.translation_.y = 1.8f;
	worldTransformR_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.translation_.x = 0.5f;
	worldTransformR_arm_.translation_.y = 1.8f;
	//スプライトの作成
	textureHandleHP1_ = TextureManager::GetInstance()->Load("Project/Resources/PlayerHP1.png");
	textureHandleHP2_ = TextureManager::GetInstance()->Load("Project/Resources/PlayerHP2.png");
	spriteHP1_ = std::make_unique<Sprite>();
	spriteHP1_->Create(textureHandleHP1_, { 50.0f,652.0f });
	spriteHP2_ = std::make_unique<Sprite>();
	spriteHP2_->Create(textureHandleHP2_, { 52.0f,654.0f });
	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	//衝突対象を自分の属性以外に設定
	SetCollisionMask(kCollisionMaskPlayer);
}

void Player::Update() {
	//強化状態の処理
	if (isEnhancedState_) {
		if (enhancedStateTimer_-- <= 0) {
			isEnhancedState_ = false;
		}
	}

	//ボックスのフラグをfalseにする
	isBoxPush_ = false;

	//Behaviorの遷移処理
	if (behaviorRequest_) {
		//振る舞いを変更する
		behavior_ = behaviorRequest_.value();
		//各振る舞いごとの初期化を実行
		switch (behavior_) {
		case Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;
		case Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		case Behavior::kDash:
			BehaviorDashInitialize();
			break;
		case Behavior::kBoxPush:
			BehaviorBoxPushInitialize();
			break;
		}
		//振る舞いリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	//Behaviorの実行
	switch (behavior_) {
	case Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;
	case Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	case Behavior::kDash:
		BehaviorDashUpdate();
		break;
	case Behavior::kBoxPush:
		BehaviorBoxPushUpdate();
		break;
	}

	//ワールドトランスフォームの更新
	worldTransformBase_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();

	//体力ゲージの更新
	UpdateLife();

	//無敵時間の処理
	UpdateInvincible();

	ImGui::Begin("Player");
	ImGui::Checkbox("isEnhanced", &isEnhancedState_);
	ImGui::Text("enhancedTimer : %d", enhancedStateTimer_);
	ImGui::DragFloat2("Sprite : uvScale", &spriteScale_.x, 0.001f);
	ImGui::End();
}

void Player::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	models_[0]->Draw(worldTransformHead_, viewProjection);
	models_[1]->Draw(worldTransformBody_, viewProjection);
	models_[2]->Draw(worldTransformL_arm_, viewProjection);
	models_[3]->Draw(worldTransformR_arm_, viewProjection);
}

void Player::DrawUI() {
	spriteHP1_->Draw();
	spriteHP2_->Draw();
}

void Player::OnCollision() {
	if (isInvincible_ == false) {
		isInvincible_ = true;
		invincibleTimer_ = kInvincibleTime;
		playerLife_--;
	}
}

Vector3 Player::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransformBase_.matWorld_.m[3][0];
	worldPos.y = worldTransformBase_.matWorld_.m[3][1];
	worldPos.z = worldTransformBase_.matWorld_.m[3][2];
	return worldPos;
}

void Player::OnCollisionBox() {
	isBoxPush_ = true;
	behaviorRequest_ = Behavior::kBoxPush;
}

void Player::BehaviorRootInitialize() {
	worldTransformL_arm_.rotation_ = { 0.0f,0.0f,0.0f };
	worldTransformR_arm_.rotation_ = { 0.0f,0.0f,0.0f };
}

void Player::BehaviorRootUpdate() {
	XINPUT_STATE joyState{};
	if (Input::GetInstance()->GetJoystickState(joyState)) {
		//しきい値
		const float threshold = 0.7f;
		//移動フラグ
		bool isMoving = false;
		//移動量
		velocity_ = {
			(float)joyState.Gamepad.sThumbLX / SHRT_MAX,
			0.0f,
			(float)joyState.Gamepad.sThumbLY / SHRT_MAX,
		};

		//スティックの押し込みが遊び範囲を超えていたら移動フラグをtureにする
		if (Length(velocity_) > threshold) {
			isMoving = true;
		}

		if (isMoving) {
			//速さ
			float kSpeed = 0.3f;
			//強化状態の時
			if (isEnhancedState_) {
				kSpeed = 0.5f;
			}

			//移動量に速さを反映
			velocity_ = Multiply(Normalize(velocity_), kSpeed);

			//移動ベクトルをカメラの角度だけ回転する
			Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_->rotation_.y);
			velocity_ = TransformNormal(velocity_, rotateMatrix);

			//移動
			worldTransformBase_.translation_ = Add(worldTransformBase_.translation_, velocity_);

			//目標角度の算出
			destinationAngleY_ = std::atan2(velocity_.x, velocity_.z);
		}
	}

	//移動方向に見た目を合わせる
	worldTransformBase_.rotation_.y = LerpShortAngle(worldTransformBase_.rotation_.y, destinationAngleY_, 0.5f);

	//ジャンプ処理
	if (Input::GetInstance()->GetJoystickState(joyState)) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			if (workJump_.flag == false) {
				workJump_.power = 1.0f;
				workJump_.flag = true;
			}
		}
	}
	if (workJump_.flag) {
		worldTransformBase_.translation_.y += workJump_.power;
		workJump_.power -= 0.05f;
		if (worldTransformBase_.translation_.y <= 0.0f) {
			worldTransformBase_.translation_.y = 0.0f;
			workJump_.flag = false;
		}
	}

	//ダッシュ行動予約
	const uint32_t behaviorDashCoolTime = 300;
	if (workDash_.coolTime != behaviorDashCoolTime) {
		workDash_.coolTime++;
	}
	if (Input::GetInstance()->GetJoystickState(joyState)) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
			if (workDash_.coolTime == 300) {
				behaviorRequest_ = Behavior::kDash;
			}
		}
	}

	//攻撃行動予約
	if (Input::GetInstance()->GetJoystickState(joyState)) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			behaviorRequest_ = Behavior::kAttack;
		}
	}
}

void Player::BehaviorAttackInitialize() {

}

void Player::BehaviorAttackUpdate() {
	XINPUT_STATE joyState{};
	if (Input::GetInstance()->GetJoystickState(joyState)) {
		//しきい値
		const float threshold = 0.7f;
		//移動フラグ
		bool isMoving = false;
		//移動量
		velocity_ = {
			(float)joyState.Gamepad.sThumbLX / SHRT_MAX,
			0.0f,
			(float)joyState.Gamepad.sThumbLY / SHRT_MAX,
		};

		//スティックの押し込みが遊び範囲を超えていたら移動フラグをtureにする
		if (Length(velocity_) > threshold) {
			isMoving = true;
		}

		if (isMoving) {
			//速さ
			float kSpeed = 0.2f;
			if (isEnhancedState_) {
				kSpeed = 0.4f;
			}

			//移動量に速さを反映
			velocity_ = Multiply(Normalize(velocity_), kSpeed);

			//移動ベクトルをカメラの角度だけ回転する
			Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_->rotation_.y);
			velocity_ = TransformNormal(velocity_, rotateMatrix);

			//移動
			worldTransformBase_.translation_ = Add(worldTransformBase_.translation_, velocity_);

			//目標角度の算出
			Vector3 rotation = Normalize(Subtract(transCube_->GetWorldPosition(), Player::GetWorldPosition()));
			destinationAngleY_ = std::atan2(rotation.x, rotation.z);
		}
	}

	//移動方向に見た目を合わせる
	worldTransformBase_.rotation_.y = LerpShortAngle(worldTransformBase_.rotation_.y, destinationAngleY_, 0.5f);

	//ジャンプ処理
	if (Input::GetInstance()->GetJoystickState(joyState)) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			if (workJump_.flag == false) {
				workJump_.power = 1.0f;
				workJump_.flag = true;
			}
		}
	}
	if (workJump_.flag) {
		worldTransformBase_.translation_.y += workJump_.power;
		workJump_.power -= 0.05f;
		if (worldTransformBase_.translation_.y <= 0.0f) {
			worldTransformBase_.translation_.y = 0.0f;
			workJump_.flag = false;
		}
	}

	//ダッシュ行動予約
	const uint32_t behaviorDashCoolTime = 300;
	if (workDash_.coolTime != behaviorDashCoolTime) {
		workDash_.coolTime++;
	}
	if (Input::GetInstance()->GetJoystickState(joyState)) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) {
			if (workDash_.coolTime == 300) {
				behaviorRequest_ = Behavior::kDash;
			}
		}
	}

	//RBを押し続けているときに弾を発射する
	if (Input::GetInstance()->GetJoystickState(joyState)) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			Player::Fire();
			return;
		}
	}

	//RBを離したら通常行動に戻る
	behaviorRequest_ = Behavior::kRoot;
}

void Player::BehaviorDashInitialize() {
	workDash_.dashParameter_ = 0;
	workDash_.coolTime = 0;
	worldTransformBase_.rotation_.y = destinationAngleY_;
}

void Player::BehaviorDashUpdate() {
	XINPUT_STATE joyState{};
	if (Input::GetInstance()->GetJoystickState(joyState)) {
		//速さ
		float kSpeed = 1.0f;
		//移動量
		velocity_ = {
			(float)joyState.Gamepad.sThumbLX / SHRT_MAX,
			0.0f,
			(float)joyState.Gamepad.sThumbLY / SHRT_MAX,
		};

		//移動量に速さを反映
		velocity_ = Multiply(Normalize(velocity_), kSpeed);

		//移動ベクトルをカメラの角度だけ回転する
		Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_->rotation_.y);
		velocity_ = TransformNormal(velocity_, rotateMatrix);

		//移動
		worldTransformBase_.translation_ = Add(worldTransformBase_.translation_, velocity_);
	}

	//ダッシュの時間
	const uint32_t behaviorDashTime = 10;
	//規定の時間経過で通常行動に戻る
	if (++workDash_.dashParameter_ >= behaviorDashTime) {
		behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::BehaviorBoxPushInitialize() {

}

void Player::BehaviorBoxPushUpdate() {
	XINPUT_STATE joyState{};
	if (Input::GetInstance()->GetJoystickState(joyState)) {
		//しきい値
		const float threshold = 0.7f;
		//移動フラグ
		bool isMoving = false;
		//移動量
		velocity_ = {
			(float)joyState.Gamepad.sThumbLX / SHRT_MAX,
			0.0f,
			(float)joyState.Gamepad.sThumbLY / SHRT_MAX,
		};

		//スティックの押し込みが遊び範囲を超えていたら移動フラグをtureにする
		if (Length(velocity_) > threshold) {
			isMoving = true;
		}

		if (isMoving) {
			//速さ
			const float kSpeed = 0.2f;

			//移動量に速さを反映
			velocity_ = Multiply(Normalize(velocity_), kSpeed);

			//移動ベクトルをカメラの角度だけ回転する
			Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_->rotation_.y);
			velocity_ = TransformNormal(velocity_, rotateMatrix);

			//移動
			worldTransformBase_.translation_ = Add(worldTransformBase_.translation_, velocity_);

			//目標角度の算出
			destinationAngleY_ = std::atan2(velocity_.x, velocity_.z);
		}
	}

	//移動方向に見た目を合わせる
	worldTransformBase_.rotation_.y = LerpShortAngle(worldTransformBase_.rotation_.y, destinationAngleY_, 0.5f);

	//ジャンプ処理
	if (Input::GetInstance()->GetJoystickState(joyState)) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			if (workJump_.flag == false) {
				workJump_.power = 1.0f;
				workJump_.flag = true;
			}
		}
	}
	if (workJump_.flag) {
		worldTransformBase_.translation_.y += workJump_.power;
		workJump_.power -= 0.05f;
		if (worldTransformBase_.translation_.y <= 0.0f) {
			worldTransformBase_.translation_.y = 0.0f;
			workJump_.flag = false;
		}
	}

	//通常状態に戻す
	behaviorRequest_ = Behavior::kRoot;
}

void Player::Fire() {
	// 発射タイマーが０以下の時に弾を発射する
	if (--bulletTimer_ <= 0) {
		// 発射タイマーを戻す
		bulletTimer_ = 20;
		if (isEnhancedState_) {
			bulletTimer_ = 10;
		}

		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		// 速度ベクトルを自機の向きに合わせて回転させる
		/*velocity = TransformNormal(velocity, worldTransformBase_.matWorld_);*/
		velocity = Subtract(transCube_->GetWorldPosition(), Player::GetWorldPosition());
		velocity = Normalize(velocity);
		velocity = Multiply(velocity, kBulletSpeed);

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		Vector3 worldPos{};
		worldPos = Player::GetWorldPosition();
		newBullet->Initialize(models_[4], worldPos, velocity);

		// 弾を登録する
		gameScene_->AddPlayerBullet(newBullet);
	}
}

void Player::UpdateLife() {
	switch (playerLife_) {
	case 0:
		spriteScale_.x = Lerp(spriteScale_.x, 0.0f, 0.1f);
		spriteHP2_->SetScale(spriteScale_);
		break;
	case 1:
		spriteScale_.x = Lerp(spriteScale_.x, 0.2f, 0.1f);
		spriteHP2_->SetScale(spriteScale_);
		break;
	case 2:
		spriteScale_.x = Lerp(spriteScale_.x, 0.4f, 0.1f);
		spriteHP2_->SetScale(spriteScale_);
		break;
	case 3:
		spriteScale_.x = Lerp(spriteScale_.x, 0.6f, 0.1f);
		spriteHP2_->SetScale(spriteScale_);
		break;
	case 4:
		spriteScale_.x = Lerp(spriteScale_.x, 0.8f, 0.1f);
		spriteHP2_->SetScale(spriteScale_);
		break;
	}
}

void Player::UpdateInvincible() {
	if (isInvincible_) {
		if (--invincibleTimer_ <= 0) {
			isInvincible_ = false;
		}
	}
}