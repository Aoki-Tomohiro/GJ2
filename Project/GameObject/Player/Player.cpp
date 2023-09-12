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
	//レティクル
	textureHandleReticle_ = TextureManager::GetInstance()->Load("Project/Resources/Reticle.png");
	sprite2DReticle_ = std::make_unique<Sprite>();
	sprite2DReticle_->Create(textureHandleReticle_, { 0.0f,0.0f });

	//ダメージエフェクト
	textureHandleDamage_ = TextureManager::GetInstance()->Load("Project/Resources/PlayerDamage.png");
	spriteDamage_ = std::make_unique<Sprite>();
	spriteDamage_->Create(textureHandleDamage_, { 0.0f,0.0f });

	//Audioのインスタンスを取得
	audio_ = Audio::GetInstance();

	damegedSE_ =Audio::GetInstance();
	damagedSEHandle_ =  audio_->SoundLoadWave("Project/Resources/Music/SE/Damage/PlayerDamaged.wav");

	attackSE_ = Audio::GetInstance();
	attackSEHandle_ = audio_->SoundLoadWave("Project/Resources/Music/SE/Attack/PlayerAttack.wav");


	dashSE_ =  Audio::GetInstance();
	dashSEHandle_ =  audio_->SoundLoadWave("Project/Resources/Music/SE/Dash/Dash.wav");;

	jumpSE_ =  Audio::GetInstance();
	jumpSEHandle_ =  audio_->SoundLoadWave("Project/Resources/Music/SE/Jump/Jump.wav");


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

	//3Dレティクルの座標を計算
	Set3DReticlePosition();

	ImGui::Begin("Player");
	ImGui::Checkbox("isEnhanced", &isEnhancedState_);
	ImGui::Text("enhancedTimer : %d", enhancedStateTimer_);
	ImGui::DragFloat2("Sprite : uvScale", &spriteScale_.x, 0.001f);
	ImGui::Text("LT : Aim");
	ImGui::Text("RT : Shot");
	ImGui::Text("LB : Dash");
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
	if (behavior_ == Behavior::kAttack) {
		sprite2DReticle_->Draw();
	}
	if (isInvincible_) {
		spriteDamage_->Draw();
	}
}

void Player::OnCollision() {
	if (isInvincible_ == false) {
		isInvincible_ = true;
		invincibleTimer_ = kInvincibleTime;
		playerLife_--;
		spriteDamage_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		damegedSE_->SoundPlayWave(damagedSEHandle_, -0);
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
		//if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
		//	behaviorRequest_ = Behavior::kAttack;
		//}
		if (joyState.Gamepad.bLeftTrigger) {
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
			Vector3 rotation = Normalize(Subtract(Player::Get3DReticleWorldPosition(), Player::GetWorldPosition()));
			destinationAngleY_ = std::atan2(rotation.x, rotation.z);
		}
	}

	//移動方向に見た目を合わせる
	worldTransformBase_.rotation_.y = LerpShortAngle(worldTransformBase_.rotation_.y, destinationAngleY_, 0.5f);

	//射撃処理
	Player::Fire();

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
		if (joyState.Gamepad.bLeftTrigger != 0) {
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

		dashSE_->SoundPlayWave(dashSEHandle_, 0);



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
		jumpSE_->SoundPlayWave(jumpSEHandle_, 0);
	}

	//通常状態に戻す
	behaviorRequest_ = Behavior::kRoot;
}

void Player::Fire() {
	XINPUT_STATE joyState{};
	if (Input::GetInstance()->GetJoystickState(joyState)) {
		//RTを押しているとき発射する
		if (joyState.Gamepad.bRightTrigger) {
			// 発射タイマーが０以下の時に弾を発射する
			if (--bulletTimer_ <= 0) {
				// 発射タイマーを戻す
				bulletTimer_ = 20;
				if (isEnhancedState_) {
					bulletTimer_ = 10;
				}

				// 弾の速度
				const float kBulletSpeed = 3.0f;
				Vector3 velocity(0, 0, kBulletSpeed);

				// 速度ベクトルを自機の向きに合わせて回転させる
				/*velocity = TransformNormal(velocity, worldTransformBase_.matWorld_);*/
				/*velocity = Subtract(transCube_->GetWorldPosition(), Player::GetWorldPosition());*/
				velocity = Subtract(Player::Get3DReticleWorldPosition(), Player::GetWorldPosition());
				velocity = Normalize(velocity);
				velocity = Multiply(velocity, kBulletSpeed);

				// 弾を生成し、初期化
				PlayerBullet* newBullet = new PlayerBullet();
				Vector3 worldPos{};
				worldPos = Player::GetWorldPosition();
				newBullet->Initialize(models_[4], worldPos, velocity);

				// 弾を登録する
				gameScene_->AddPlayerBullet(newBullet);

				attackSE_->SoundPlayWave(attackSEHandle_, 0);
				




			}
		}
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
		spriteDamage_->SetColor({ 1.0f,1.0f,1.0f,invincibleTimer_ / 60.0f });
		if (--invincibleTimer_ <= 0) {
			isInvincible_ = false;
		}
	}
}

void Player::Set3DReticlePosition() {
	// 自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 100.0f;
	//オフセット
	Vector3 offset{ 0.0f,0.0f,1.0f };
	//カメラの角度から回転行列を作成
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(viewProjection_->rotation_.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(viewProjection_->rotation_.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(viewProjection_->rotation_.z);
	Matrix4x4 rotateMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));
	//オフセットを回転行列分回転させる
	offset = TransformNormal(offset, rotateMatrix);
	//ベクトルの長さをそろえる
	offset = Normalize(offset);
	offset = Multiply(offset, kDistancePlayerTo3DReticle);
	//3Dレティクルの座標を計算
	worldTransform3DReticle_.translation_ = Add(viewProjection_->translation_, offset);
	//3Dレティクルのワールドトランスフォームを更新
	worldTransform3DReticle_.UpdateMatrix();

	// 3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	Vector3 positionReticle = Get3DReticleWorldPosition();
	// ビューポート行列
	Matrix4x4 matViewport =
		MakeViewportMatrix(0, 0, float(WinApp::GetInstance()->kClientWidth), float(WinApp::GetInstance()->kClientHeight), 0, 1);
	// ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4x4 matViewProjectionViewport =
		Multiply(viewProjection_->matView_, Multiply(viewProjection_->matProjection_, matViewport));
	// ワールド→スクリーン座標変換(ここで3Dから2Dになる)
	positionReticle = Transform(positionReticle, matViewProjectionViewport);
	// スプライトのレティクルに座標設定
	sprite2DReticle_->SetTranslation(Vector2(positionReticle.x - 48, positionReticle.y - 48));
}

Vector3 Player::Get3DReticleWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得(ワールド座標)
	worldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];
	return worldPos;
}