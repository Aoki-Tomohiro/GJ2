#include "GameScene.h"
#include <cassert>

GameScene::GameScene() {};

GameScene::~GameScene() {};

void GameScene::Initialize(GameManager* gameManager) {
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
	modelPlayerHead_->CreateFromOBJ("Project/Resources/Player_Head", "Player_Head.obj");
	//体
	modelPlayerBody_ = std::make_unique<Model>();
	modelPlayerBody_->CreateFromOBJ("Project/Resources/Player_Body", "Player_Body.obj");
	//左腕
	modelPlayerL_arm_ = std::make_unique<Model>();
	modelPlayerL_arm_->CreateFromOBJ("Project/Resources/Player_L_arm", "Player_L_arm.obj");
	//右腕
	modelPlayerR_arm_ = std::make_unique<Model>();
	modelPlayerR_arm_->CreateFromOBJ("Project/Resources/Player_R_arm", "Player_R_arm.obj");
	//自弾
	modelPlayerBullet_ = std::make_unique<Model>();
	modelPlayerBullet_->CreateSphere();
	//モデルを配列にまとめる
	playerModels_ = { modelPlayerHead_.get(),modelPlayerBody_.get(),modelPlayerL_arm_.get(),modelPlayerR_arm_.get(),modelPlayerBullet_.get() };
	//自キャラの初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels_);
	player_->SetGameScene(this);

	//敵キャラの初期化
	modelTransCube_ = std::make_unique<Model>();
	modelTransCube_->CreateFromOBJ("Project/Resources/EnemyObj/TransCube", "TransCube.obj");

	transCube_ = std::make_unique<TransCube>();
	transCube_->SetPlayer(player_.get());

	transCube_->Initialize();

	//追従カメラの初期化
	followCamera_ = std::make_unique<FollowCamera>();
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
};

void GameScene::Update(GameManager* gameManager) {
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

	transCube_.get()->SetPlayer(player_.get());
	//敵キャラの更新
	transCube_->Update();

	//箱の更新
	boxManager_->Update();


	//衝突マネージャーのリストをクリア
	collisionManager_->ClearColliderList();
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
};

void GameScene::Draw(GameManager* gameManager) {
	//自キャラの描画
	player_->Draw(viewProjection_);
	//自弾の描画
	for (std::unique_ptr<PlayerBullet>& bullet : playerBullets_) {
		bullet->Draw(viewProjection_);
	}
	//敵キャラの描画
	transCube_->Draw(viewProjection_);
	//箱の描画
	boxManager_->Draw(viewProjection_);
	//地面の描画
	ground_->Draw(viewProjection_);


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

void GameScene::AddPlayerBullet(PlayerBullet* playerBullet) {
	//自機の弾を追加する
	playerBullets_.push_back(std::unique_ptr<PlayerBullet>(playerBullet));
}

void GameScene::SetCollisions()
{
	//自キャラ
	collisionManager_->SetColliderList(player_.get());
	collisionManager_->SetColliderListAABB(player_.get());
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets_) {
		collisionManager_->SetColliderList(bullet.get());
	}

	//敵
	collisionManager_->SetColliderList(transCube_.get());
	for (TransCubeBullet* bullet : transCube_.get()->Getbullets()) {
		collisionManager_->SetColliderList(bullet);
	}
	for (TransCubeGroundAttack* bullet : transCube_.get()->GetGroundBullets()) {
		collisionManager_->SetColliderListAABB(bullet);

	}
}
