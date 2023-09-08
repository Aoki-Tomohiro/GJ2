#include "WinScene.h"
#include "GameManager/GameManager.h"

#include "GameScene/GameScene.h"
#include "SelectScene/SelectScene.h"
#include "TitleScene/TitleScene.h"

#include <imgui.h>


/// <summary>
/// コンストラクタ
/// </summary>
WinScene::WinScene() {

}

/// <summary>
/// デストラクタ
/// </summary>
WinScene::~WinScene() {
	
}

/// <summary>
/// 初期化
/// </summary>
void WinScene::Initialize(GameManager* gameManager) {
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


	//スプライトの初期化
	//インスタンスを作り忘れないように
	sprite_ = new Sprite();
	
	position_ = { 0.0f,0.0f };
	textureHandle_=textureManager_->Load("Project/Resources/Result/Win/Win.png");

	sprite_->Create(textureHandle_, position_);
	
}

/// <summary>
/// 更新
/// </summary>
void WinScene::Update(GameManager* gameManager) {
	
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
	}

	//Aボタンでステージ選択へ
	//今はそのままゲームシーンへ
	
	//1でGameSceneへ
	if (input_->IsPushKeyEnter(DIK_1)) {
		gameManager->ChangeScene(new GameScene());
	}
	//2でSelectSceneへ
	if (input_->IsPushKeyEnter(DIK_2)) {
		gameManager->ChangeScene(new SelectScene());
	}
	//3でSelectSceneへ
	if (input_->IsPushKeyEnter(DIK_3)) {
		gameManager->ChangeScene(new TitleScene());
	}

	ImGui::Begin("WinScene");
	ImGui::Text("1:To GameScene");
	ImGui::Text("2:To SelectScene");
	ImGui::Text("3:To TitleScene");


	ImGui::End();

	XINPUT_STATE joyState{};

	


}

/// <summary>
/// 描画
/// </summary>
void WinScene::Draw(GameManager* gameManager) {
	//Skydome
	//modelSkydome_->Draw(worldTransform_, viewProjection_);


	sprite_->Draw();

}





