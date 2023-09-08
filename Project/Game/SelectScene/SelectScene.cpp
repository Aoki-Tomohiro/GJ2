#include "SelectScene.h"
#include "GameManager/GameManager.h"
#include "GameScene/GameScene.h"
#include <imgui.h>


/// <summary>
/// コンストラクタ
/// </summary>
SelectScene::SelectScene() {

}

/// <summary>
/// デストラクタ
/// </summary>
SelectScene::~SelectScene() {

}

/// <summary>
/// 初期化
/// </summary>
void SelectScene::Initialize(GameManager* gameManager) {
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


	//初期では1を選択している
	stageNumber_ = 1;


	//スプライトの初期化
	position_ = { 0.0f,0.0f };
	//textureHandle_=textureManager_->Load("Project/Resources/Title/TitleLogo/TitleLogo.png");

	//sprite_->Create(textureHandle_, position_, kNormal);
	
}

/// <summary>
/// 更新
/// </summary>
void SelectScene::Update(GameManager* gameManager) {
	
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



	ImGui::Begin("Select");
	ImGui::Text("Select");
	ImGui::End();

	XINPUT_STATE joyState{};

	//Aボタンでステージ選択へ
	//今はそのままゲームシーンへ
	if (Input::GetInstance()->GetJoystickState(joyState)) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			gameManager->ChangeScene(new GameScene());
		}
	}



}

/// <summary>
/// 描画
/// </summary>
void SelectScene::Draw(GameManager* gameManager) {
	//Skydome
	//modelSkydome_->Draw(worldTransform_, viewProjection_);


	//sprite_->Draw();

}





