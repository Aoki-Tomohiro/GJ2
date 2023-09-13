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
	delete backSprite_;


	delete winSprite_[0];
	delete winSprite_[1];

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
	backTextureHandle_ = textureManager_->Load("Resources/Black/BlackTexture.png");
	backSprite_->Create(backTextureHandle_, position_);
	

	//スプライトの初期化
	//インスタンスを作り忘れないように
	winSprite_[0] = new Sprite();
	winSprite_[1] = new Sprite();
	

	winTextureHandle_[0] = textureManager_->Load("Resources/Result/Win/Win1.png");
	winTextureHandle_[1] = textureManager_->Load("Resources/Result/Win/Win2.png");


	winSprite_[0]->Create(winTextureHandle_[0] , position_);
	winSprite_[1]->Create(winTextureHandle_[1], position_);
	
	winTextureChangeTime_ = 0;
	winTextureNumber_ = 0;

	//透明度
	COLOR_BIND = 1.0f;
	transparency_ = { COLOR_BIND,COLOR_BIND,COLOR_BIND,0.0f };
	fadeInterval_ = 10.0 / 256.0f;


	SECOND_ = 60;
	loadingTime = 0;

	triggerButtonBTime_ = 0;
	triggerButtonATime_ = 0;
	fadeOutTime_ = 0;



	titleSceneChange_ = false;
	selectSceneChange_ = false;


	//BGM
	bgmAudio_ =  Audio::GetInstance();
	bgmHandle_ = audio_->SoundLoadWave("Resources/Music/BGM/Win/Win.wav");

	bgmAudio_->SoundPlayWave(bgmHandle_, true);

	//シーンが変わるときのSE
	backSEAudio_ = Audio::GetInstance();;
	backSEHandle_ = audio_->SoundLoadWave("Resources/Music/SE/Deside/Deside1.wav");




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
	
	//画像の切り替わり
	winTextureChangeTime_ += 1;
	if (winTextureChangeTime_ > 0) {
		if (winTextureChangeTime_>0 && 
			winTextureChangeTime_<=SECOND_/2 ) {
			winTextureNumber_ = 0;
		}
		if (winTextureChangeTime_>SECOND_/2 && 
			winTextureChangeTime_<=SECOND_ ) {
			winTextureNumber_ = 1;
		}
		if (winTextureChangeTime_>SECOND_) {
			winTextureChangeTime_ = 0;
		}

	}
	

	XINPUT_STATE joyState{};

	if (Input::GetInstance()->GetJoystickState(joyState)) {

		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			triggerButtonBTime_ += 1;
			
		}
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			triggerButtonATime_ += 1;
			
		}
		
	}


	if (isFadeOutModeToSelect_ == false && isFadeOutModeToTitle_==false) {
		if (input_->IsPushKeyEnter(DIK_B) || input_->IsPushKeyEnter(DIK_RETURN)) {
			//StartSE再生
			//ループ無し
			backSEAudio_->SoundPlayWave(backSEHandle_, false);

			//BGMを止める
			bgmAudio_->StopAudio(bgmHandle_);
				
			isFadeOutModeToSelect_ = true;
		}
		


		//トリガー代わり
		//Bを押したばあい
		if (triggerButtonBTime_ == 1) {
			
			//StartSE再生
			//ループ無し
			backSEAudio_->SoundPlayWave(backSEHandle_, false);

			//BGMを止める
			bgmAudio_->StopAudio(bgmHandle_);
				
			isFadeOutModeToSelect_ = true;

		}


		if (input_->IsPushKeyEnter(DIK_A) || input_->IsPushKeyEnter(DIK_SPACE)) {
			//StartSE再生
			//ループ無し
			backSEAudio_->SoundPlayWave(backSEHandle_, false);

			//BGMを止める
			bgmAudio_->StopAudio(bgmHandle_);
				
			isFadeOutModeToTitle_ = true;
		}

		if (triggerButtonATime_ == 1) {
			
			//StartSE再生
			//ループ無し
			backSEAudio_->SoundPlayWave(backSEHandle_, false);

			//BGMを止める
			bgmAudio_->StopAudio(bgmHandle_);
				
			isFadeOutModeToTitle_ = true;

		}
	}

	

	
	
	if (isFadeOutModeToSelect_ == true) {
		transparency_.w += fadeInterval_;
		
		///ローディング
		if (transparency_.w >= 1.0f) {
			loadingTime += 1;

			if (loadingTime > SECOND_ * 3) {
				selectSceneChange_ = true;
				
			}
		}

	}
	
	if (isFadeOutModeToTitle_ == true) {
		transparency_.w += fadeInterval_;
		
		///ローディング
		if (transparency_.w >= 1.0f) {
			loadingTime += 1;

			if (loadingTime > SECOND_ * 3) {
				titleSceneChange_ = true;
				
			}
		}

	}

	//スプライトの透明度をここで設定
	backSprite_->SetColor({COLOR_BIND,COLOR_BIND,COLOR_BIND,transparency_.w});
	

	//SceneChange
	if (selectSceneChange_ == true) {
		gameManager->ChangeScene(new SelectScene());	
	}
	if (titleSceneChange_ == true) {
		gameManager->ChangeScene(new TitleScene());
	}


}

/// <summary>
/// 描画
/// </summary>
void WinScene::Draw(GameManager* gameManager) {
	//Skydome
	//modelSkydome_->Draw(worldTransform_, viewProjection_);

	if (winTextureNumber_ == 0) {
		winSprite_[0]->Draw();
	}
	if (winTextureNumber_ == 1) {
		winSprite_[1]->Draw();
	}
	

	backSprite_->Draw();
}





