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
	backSprite_ = new Sprite();
	position_ = { 0.0f,0.0f };
	backTextureHandle_ = textureManager_->Load("Project/Resources/Black/BlackTexture.png");
	backSprite_->Create(backTextureHandle_, position_);
	

	//スプライトの初期化
	//インスタンスを作り忘れないように
	sprite_ = new Sprite();
	
	textureHandle_=textureManager_->Load("Project/Resources/Result/Win/Win.png");

	sprite_->Create(textureHandle_, position_);
	


	//透明度
	COLOR_BIND = 1.0f;
	transparency_ = { COLOR_BIND,COLOR_BIND,COLOR_BIND,0.0f };
	fadeInterval_ = 10.0 / 256.0f;


	const int SECOND_ = 60;
	int32_t loadingTime = 0;

	triggerButtonBTime_ = 0;
	triggerButtonATime_ = 0;
	fadeOutTime_ = 0;


	//BGM
	bgmAudio_ =  Audio::GetInstance();
	bgmHandle_ = audio_->SoundLoadWave("Project/Resources/Music/BGM/Win/Win.wav");

	bgmAudio_->SoundPlayWave(bgmHandle_, true);

	backSEAudio_ = Audio::GetInstance();;
	backSEHandle_ = audio_->SoundLoadWave("Project/Resources/Music/BGM/Win/Win.wav");;




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
	
	
	ImGui::Begin("WinScene");
	ImGui::Text("1:To GameScene");
	ImGui::Text("2:To SelectScene");
	ImGui::Text("3:To TitleScene");


	ImGui::End();

	XINPUT_STATE joyState{};

	if (Input::GetInstance()->GetJoystickState(joyState)) {

		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			triggerButtonBTime_ += 1;
			
		}
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			triggerButtonATime_ += 1;
			
		}
		
	}

	//トリガー代わり
	if (triggerButtonBTime_ == 1) {
		isFadeOutMode_ = true;
		//StartSE再生
		//ループ無し
		backSEAudio_->SoundPlayWave(backSEHandle_, false);

		//BGMを止める
		bgmAudio_->StopAudio(bgmHandle_);
			
		if (loadingTime > SECOND_ * 3) {
			gameManager->ChangeScene(new SelectScene());	
		}
	

		//BでSelectSceneへ
		if (input_->IsPushKeyEnter(DIK_2)) {
			bgmAudio_->StopAudio(bgmHandle_);
			gameManager->ChangeScene(new SelectScene());
		}
	}

	if (triggerButtonATime_ == 1) {

	}

	if (isFadeOutMode_ == true) {
		transparency_.w += fadeInterval_;
		

		
		///ローディング
		if (transparency_.w >= 1.0f) {
			loadingTime += 1;
		}
	}
	//スプライトの透明度をここで設定
	backSprite_->SetColor({COLOR_BIND,COLOR_BIND,COLOR_BIND,transparency_.w});
		
	
	//AでSelectSceneへ
	if (input_->IsPushKeyEnter(DIK_3)) {
		bgmAudio_->StopAudio(bgmHandle_);
		gameManager->ChangeScene(new TitleScene());
	}



}

/// <summary>
/// 描画
/// </summary>
void WinScene::Draw(GameManager* gameManager) {
	//Skydome
	//modelSkydome_->Draw(worldTransform_, viewProjection_);


	sprite_->Draw();

}





