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
	collisionManager_ = new CollisionManager();
	//デバッグカメラの生成
	debugCamera_ = new DebugCamera();

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
	//モデルを配列にまとめる
	playerModels_ = { modelPlayerHead_.get(),modelPlayerBody_.get(),modelPlayerL_arm_.get(),modelPlayerR_arm_.get() };
	//自キャラの初期化
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels_);

	//追従カメラの初期化
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->SetTarget(&player_->GetWorldTransform());
	player_->SetViewProjection(&followCamera_->GetViewProjection());

	//Skydome
	modelSkydome_ = std::make_unique<Model>();
	modelSkydome_->CreateFromOBJ("Project/Resources/Skydome", "Skydome.obj");
	modelSkydome_->GetMaterial()->enableLighting_ = false;
	modelSkydome_->GetMaterial()->Update();
};

void GameScene::Update(GameManager* gameManager) {
	//自キャラの更新
	player_->Update();
	//追従カメラの更新
	followCamera_->Update();
	//Skydome
	worldTransform_.UpdateMatrix();

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
	//Skydome
	modelSkydome_->Draw(worldTransform_, viewProjection_);

#pragma region ポストプロセス
	//ポストプロセスの描画前処理
	postProcess_->PreDraw();

	//ポストプロセスの描画後処理
	postProcess_->PostDraw();
#pragma endregion

};