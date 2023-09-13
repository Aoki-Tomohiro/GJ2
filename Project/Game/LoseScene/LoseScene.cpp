#include "LoseScene.h"
#include "GameManager/GameManager.h"

#include "GameScene/GameScene.h"
#include "SelectScene/SelectScene.h"
#include "TitleScene/TitleScene.h"

#include <imgui.h>


/// <summary>
/// コンストラクタ
/// </summary>
LoseScene::LoseScene() {

}

/// <summary>
/// デストラクタ
/// </summary>
LoseScene::~LoseScene() {
	delete backSprite_;


	delete loseSprite_[0];
	delete loseSprite_[1];

}

/// <summary>
/// 初期化
/// </summary>
void LoseScene::Initialize(GameManager* gameManager) {
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
	position_ = { 0.0f,0.0f };
	
	loseSprite_[0] = new Sprite();
	loseSprite_[1] = new Sprite();

	loseTextureHandle_[0] = textureManager_->Load("Resources/Result/Lose/Lose1.png");
	loseTextureHandle_[1] = textureManager_->Load("Resources/Result/Lose/Lose2.png");
	
	
	loseSprite_[0]->Create(loseTextureHandle_[0], position_);
	loseSprite_[1]->Create(loseTextureHandle_[1], position_);

	loseTextureChangeTime_ = 0;
	loseTextureNumber_ = 0;




	backSprite_ = new Sprite();
	backTextureHandle_ = textureManager_->Load("Resources/Black/BlackTexture.png");

	backSprite_->Create(backTextureHandle_, position_);



	//BGM
	bgmAudio_ = Audio::GetInstance();;
	bgmHandle_ =  audio_->SoundLoadWave("Resources/Music/BGM/Lose/Lose.wav");

	//SE
	bgmAudio_->SoundPlayWave(bgmHandle_, true);

	//シーンが変わるときのSE
	backSEAudio_ = Audio::GetInstance();;
	backSEHandle_ = audio_->SoundLoadWave("Resources/Music/SE/Deside/Deside1.wav");




	//Trigger専用のタイマー
	triggerButtonBTime_ = 0;
	triggerButtonATime_ = 0;
	fadeOutTime_ = 0;

	//FadeOut用
	isFadeOutModeToSelect_ = false;
	isFadeOutModeToTitle_ = false;

	COLOR_BIND = 1.0f;
	transparency_ = { COLOR_BIND,COLOR_BIND,COLOR_BIND,0.0f };
	fadeInterval_ =  10.0 / 256.0f;;

	SECOND_ = 60;
	loadingTime = 0;




	//titleへ
	titleSceneChange_ = false;
	selectSceneChange_ = false;







}

/// <summary>
/// 更新
/// </summary>
void LoseScene::Update(GameManager* gameManager) {
	
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
	

	XINPUT_STATE joyState{};

	if (Input::GetInstance()->GetJoystickState(joyState)) {

		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			triggerButtonBTime_ += 1;
			
		}
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			triggerButtonATime_ += 1;
			
		}
		
	}



	loseTextureChangeTime_ += 1;
	if (loseTextureChangeTime_ > 0) {
		if (loseTextureChangeTime_ > 0 && 
			loseTextureChangeTime_ <=SECOND_/2) {
			loseTextureNumber_ = 0;
		}
		if (loseTextureChangeTime_ > SECOND_/2 && 
			loseTextureChangeTime_ <=SECOND_) {
			loseTextureNumber_ = 1;
		}
		if (loseTextureChangeTime_ > SECOND_) {
			loseTextureChangeTime_ = 0;
		}
	}

	if (isFadeOutModeToSelect_ == false && isFadeOutModeToTitle_ == false) {
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
void LoseScene::Draw(GameManager* gameManager) {
	if (loseTextureNumber_ == 0) {
		loseSprite_[0]->Draw();
	}
	if (loseTextureNumber_ == 1) {
		loseSprite_[1]->Draw();
	}
	
	backSprite_->Draw();
}





