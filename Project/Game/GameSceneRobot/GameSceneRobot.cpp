#include "GameSceneRobot.h"
#include <cassert>

#include "GameManager/GameManager.h"
#include "WinScene/WinScene.h"
#include "LoseScene/LoseScene.h"

GameSceneRobot::GameSceneRobot() {};

GameSceneRobot::~GameSceneRobot() {



};

void GameSceneRobot::Initialize(GameManager* gameManager) {
	//TextureManagerのインスタンスを取得
	textureManager_ = TextureManager::GetInstance();
	//Audioのインスタンスを取得
	audio_ = Audio::GetInstance();
	//Inputのインスタンスを取得
	input_ = Input::GetInstance();
	//PostProcessのインスタンスを取得
	postProcess_ = PostProcess::GetInstance();
	//衝突マネージャーの作成
	collisionManager_ = std::make_unique<CollisionManager>();
	//デバッグカメラの生成
	debugCamera_ = std::make_unique<DebugCamera>();

	//自キャラのモデル作成
	//頭
	modelPlayerHead_ = std::make_unique<Model>();
	modelPlayerHead_->CreateFromOBJ("Resources/Player_Head", "Player_Head.obj");
	//体
	modelPlayerBody_ = std::make_unique<Model>();
	modelPlayerBody_->CreateFromOBJ("Resources/Player_Body", "Player_Body.obj");
	//左腕
	modelPlayerL_arm_ = std::make_unique<Model>();
	modelPlayerL_arm_->CreateFromOBJ("Resources/Player_L_arm", "Player_L_arm.obj");
	//右腕
	modelPlayerR_arm_ = std::make_unique<Model>();
	modelPlayerR_arm_->CreateFromOBJ("Resources/Player_R_arm", "Player_R_arm.obj");
	//自弾
	modelPlayerBullet_ = std::make_unique<Model>();
	modelPlayerBullet_->CreateSphere();
	//モデルを配列にまとめる
	playerModels_ = { modelPlayerHead_.get(),modelPlayerBody_.get(),modelPlayerL_arm_.get(),modelPlayerR_arm_.get(),modelPlayerBullet_.get() };
	//自キャラの初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels_);
	player_->SetGameSceneRobot(this);

	//敵キャラの初期化
	EnemyRobot_ = std::make_unique<EnemyRobot>();
	EnemyRobot_->Initialize();

	//追従カメラの初期化
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize({ 0.0f,15.0f,-80.0f });
	followCamera_->SetTarget(&player_->GetWorldTransform());
	player_->SetViewProjection(&followCamera_->GetViewProjection());

	//地面の初期化
	ground_ = std::make_unique<TestGround>();
	ground_->Initialize();

	//箱の初期化
	boxManager_ = std::make_unique<BoxManager>();
	boxManager_->Initialize();
	boxManager_->SetPlayer(player_.get());

	//衝突マネージャーの生成
	collisionManager_ = std::make_unique<CollisionManager>();

	//Music
	bgmAudio_ = Audio::GetInstance();
	bgmHandle_ = audio_->SoundLoadWave("Resources/Music/BGM/Game/GameBGM2.wav");

	bgmAudio_->SoundPlayWave(bgmHandle_, true);
};

void GameSceneRobot::Update(GameManager* gameManager) {
	//追従カメラの更新
	followCamera_->Update(player_->GetBehavior());

	//自キャラの更新
	player_->Update();

	//デスフラグの立った自弾を削除
	playerBullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		if (bullet->isDead()) {
			bullet.reset();
			return true;
		}
		return false;
		});
	// 自弾の更新
	for (std::unique_ptr<PlayerBullet>& bullet : playerBullets_) {
		bullet->Update();
	}
	EnemyRobot_->SetPlayer(player_.get());
	//敵キャラの更新
	EnemyRobot_->Update();

	//箱の更新
	boxManager_->Update();

	//デスフラグの立ったパーティクルを削除
	particleEmitters_.remove_if([](std::unique_ptr<ParticleEmitter>& particleEmitter) {
		if (particleEmitter->isDead()) {
			particleEmitter.reset();
			return true;
		}
		return false;
		});
	//パーティクルの更新
	for (std::unique_ptr<ParticleEmitter>& particleEmitter : particleEmitters_) {
		particleEmitter->Update();
	}

	//衝突マネージャーのリストをクリア
	collisionManager_->ClearColliderList();
	//コライダーをセット
	SetCollisions();
	//衝突判定
	collisionManager_->CheckAllCollisions();

	//デバッグカメラの更新
	debugCamera_->Update();
	//デバッグカメラの切り替え
	if (input_->IsPushKeyEnter(DIK_1)) {
		if (isDebugCameraActive_ == false) {
			isDebugCameraActive_ = true;
		}
		else {
			isDebugCameraActive_ = false;
		}
	}
	//ビュープロジェクションの更新
	if (isDebugCameraActive_ == true) {
		viewProjection_.matView_ = debugCamera_->GetViewProjection().matView_;
		viewProjection_.matProjection_ = debugCamera_->GetViewProjection().matProjection_;
	}
	else {
		viewProjection_.matView_ = followCamera_->GetViewProjection().matView_;
		viewProjection_.matProjection_ = followCamera_->GetViewProjection().matProjection_;
	}


	//2でSelectSceneへ
	if (input_->IsPushKeyEnter(DIK_W)) {
		bgmAudio_->StopAudio(bgmHandle_);
		gameManager->ChangeScene(new WinScene());
		return;
	}
	//3でSelectSceneへ
	if (input_->IsPushKeyEnter(DIK_L)) {
		bgmAudio_->StopAudio(bgmHandle_);
		gameManager->ChangeScene(new LoseScene());
		return;
	}

	ImGui::Begin(" ");
	//ポストプロセス
	ImGui::Checkbox("PostProcess", &postProcess_->isActive);
	ImGui::Checkbox("Bloom", &postProcess_->isBloomActive);
	ImGui::Checkbox("Fog", &postProcess_->isFogActive);
	ImGui::Checkbox("DoF", &postProcess_->isDofActive);
	ImGui::Checkbox("LensDistortion", &postProcess_->isLensDistortionActive);
	ImGui::Checkbox("Vignette", &postProcess_->isVignetteActive);
	//デバッグカメラ
	ImGui::Checkbox("DebugCamera", &isDebugCameraActive_);
	ImGui::Text("1 : DebugCamera");
	ImGui::Text("WASD : Move up/down/left/right");
	ImGui::Text("MouseWheel : Move forward/backward");
	ImGui::Text("Left Right : RotateX");
	ImGui::Text("UP DOWN : RotateY");
	ImGui::End();

	ImGui::Begin("GameSceneRobot");
	ImGui::Text("W:WinScene");
	ImGui::Text("L:LoseScene");
	ImGui::End();
};

void GameSceneRobot::Draw(GameManager* gameManager) {
	//自キャラの描画
	player_->Draw(viewProjection_);

	//自弾の描画
	for (std::unique_ptr<PlayerBullet>& bullet : playerBullets_) {
		bullet->Draw(viewProjection_);
	}

	//敵キャラの描画
	EnemyRobot_->Draw(viewProjection_);

	//箱の描画
	boxManager_->Draw(viewProjection_);

	//地面の描画
	ground_->Draw(viewProjection_);

	//パーティクルの描画
	for (std::unique_ptr<ParticleEmitter>& particleEmitter : particleEmitters_) {
		particleEmitter->Draw(viewProjection_);
	}

#pragma region ポストプロセス
	//ポストプロセスの描画前処理
	postProcess_->PreDraw();

	//ポストプロセスの描画後処理
	postProcess_->PostDraw();
#pragma endregion

#pragma region スプライトの描画
	//自機のHPの描画
	player_->DrawUI();
#pragma endregion
};

void GameSceneRobot::AddPlayerBullet(PlayerBullet* playerBullet) {
	//自機の弾を追加する
	playerBullets_.push_back(std::unique_ptr<PlayerBullet>(playerBullet));
}


void GameSceneRobot::AddParticleEmitter(ParticleEmitter* particleEmitter) {
	//パーティクルを追加する
	particleEmitters_.push_back(std::unique_ptr<ParticleEmitter>(particleEmitter));
}

void GameSceneRobot::SetCollisions(){
	//自キャラ
	collisionManager_->SetColliderList(player_.get());
	collisionManager_->SetColliderListAABB(player_.get());
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets_) {
		collisionManager_->SetColliderList(bullet.get());
	}


	collisionManager_->SetColliderList(EnemyRobot_.get());
	for (RobotBullet* bullet : EnemyRobot_.get()->GetBullets())
	{
		collisionManager_->SetColliderList(bullet);
	}
	for (RobotPunch* bullet : EnemyRobot_.get()->GetPunch())
	{
		collisionManager_->SetColliderListAABB(bullet);

	}
}
