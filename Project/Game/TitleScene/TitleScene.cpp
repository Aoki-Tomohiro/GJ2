#include "TitleScene.h"
#include "GameManager/GameManager.h"
#include "SelectScene/SelectScene.h"
#include "GameScene/GameScene.h"
#include <imgui.h>


/// <summary>
/// コンストラクタ
/// </summary>
TitleScene::TitleScene() {

}

/// <summary>
/// デストラクタ
/// </summary>
TitleScene::~TitleScene() {
	delete sprite_;
	delete backSprite_;
}

/// <summary>
/// 初期化
/// </summary>
void TitleScene::Initialize(GameManager* gameManager) {
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


	//Timer系の変数
	triggerButtonBTime_ = 0;
	fadeOutTime_ = 0;
	isFadeInMode_ = true;
	isFadeOutMode_ = false;

	//透明度
	COLOR_BIND = 1.0f;
	transparency_ = { COLOR_BIND,COLOR_BIND,COLOR_BIND,1.0f };
	fadeInterval_ = 10.0 / 256.0f;


	const int SECOND_ = 60;
	int32_t loadingTime = 0;


	//スプライトの初期化
	backSprite_ = new Sprite();
	position_ = { 0.0f,0.0f };
	backTextureHandle_ = textureManager_->Load("Resources/Black/BlackTexture.png");
	backSprite_->Create(backTextureHandle_, position_);
	


	sprite_ = new Sprite();
	textureHandle_=textureManager_->Load("Resources/Title/TitleLogo/TitleLogo.png");
	sprite_->Create(textureHandle_, position_);
	

	//Start
	startSprite_ = new Sprite();
	startTextureHandle_ = textureManager_->Load("Resources/Title/TitleLogo/TitleStart.png");
	startSprite_->Create(startTextureHandle_, position_);
	transparencyChangeTime_ = 0;
	isStartTexture_ = true;

	//Music
	bgmAudio_ =  Audio::GetInstance();
	bgmHandle_=audio_->SoundLoadWave("Resources/Music/BGM/Title/TitleBGM.wav");



	//SE
	startSEAudio_ =  Audio::GetInstance();
	startSEHandle_=audio_->SoundLoadWave("Resources/Music/SE/Deside/Start.wav");
	bgmAudio_->SoundPlayWave(bgmHandle_, true);

}

/// <summary>
/// 更新
/// </summary>
void TitleScene::Update(GameManager* gameManager) {
	
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



	ImGui::Begin("Title");
	ImGui::Text("Title");
	ImGui::Text("Space To SlectScene");
	ImGui::InputFloat4("Transparency", &transparency_.x);
	ImGui::End();

	//BGM再生
	//ループあり
	//FadeIn
	//黒背景が透明になっていっていく
	//疑似FadeIn
	if (isFadeInMode_ == true) {
		transparency_.w -= fadeInterval_;

		if (transparency_.w <= 0.0f) {
			isFadeInMode_ = false;
		}
	}
	


	if (isFadeInMode_ == false) {
		XINPUT_STATE joyState{};

		if (Input::GetInstance()->GetJoystickState(joyState)) {

			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
				triggerButtonBTime_ += 1;
			
			}
		
		}

		//トリガー代わり
		if (triggerButtonBTime_ == 1) {
			isFadeOutMode_ = true;
			//StartSE再生
			//ループ無し
			startSEAudio_->SoundPlayWave(startSEHandle_, false);
		
			//BGMを止める
			bgmAudio_->StopAudio(bgmHandle_);
			
		}

		
		transparencyChangeTime_ += 1;
		
		if (transparencyChangeTime_ > 0) {
			if (transparencyChangeTime_ > 0 &&
				transparencyChangeTime_ <= SECOND_/2) {
				isStartTexture_ = true;
			}
			if (transparencyChangeTime_ > SECOND_/2 &&
				transparencyChangeTime_ <= SECOND_) {
				isStartTexture_ = false;
			}
			if (transparencyChangeTime_ > SECOND_) {
				transparencyChangeTime_ = 0;
			}
			

		}


		







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
		
	if (loadingTime > SECOND_ * 3) {
		gameManager->ChangeScene(new SelectScene());	
	}
		
	
	


}

/// <summary>
/// 描画
/// </summary>
void TitleScene::Draw(GameManager* gameManager) {
	sprite_->Draw();

	backSprite_->Draw();

	if (isStartTexture_ == true) {
		startSprite_->Draw();

	}
	
}





