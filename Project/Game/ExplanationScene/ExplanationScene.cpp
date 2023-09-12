#include "ExplanationScene.h"
#include "GameManager/GameManager.h"
#include "SelectScene/SelectScene.h"
#include "imgui.h"

/// <summary>
/// コンストラクタ
/// </summary>
ExplanationScene::ExplanationScene() {

}

/// <summary>
/// デストラクタ
/// </summary>
ExplanationScene::~ExplanationScene() {
	for (int i = 0; i < 3; i++) {
		delete explanationSprite_[i];
	}

	delete backSprite_;
}

/// <summary>
/// 初期化
/// </summary>
void ExplanationScene::Initialize(GameManager* gameManager) {
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


	isFadeInMode_ = true;

	//スプライトの初期化
	//Fade用
	backSprite_ = new Sprite();
	backTextureHandle_ = textureManager_->Load("Resources/Black/BlackTexture.png");
	backSprite_->Create(backTextureHandle_, { 0.0f,0.0f });

	//透明度
	COLOR_BIND = 1.0f;
	//1.0fで真っ暗
	transparency_ = { COLOR_BIND,COLOR_BIND,COLOR_BIND,1.0f };
	fadeInterval_ = 10.0f / 256.0f;


	const int SECOND_ = 60;
	int32_t loadingTime = 0;

	//説明
	explanationSprite_[0] = new Sprite();
	explanationSprite_[1] = new Sprite();
	explanationSprite_[2] = new Sprite();



	explanationHandle_[0] = textureManager_->Load("Resources/Explanation/Explanation1.png");
	explanationHandle_[1] = textureManager_->Load("Resources/Explanation/Explanation2.png");
	explanationHandle_[2] = textureManager_->Load("Resources/Explanation/Explanation3.png");


	explanationSprite_[0]->Create(explanationHandle_[0] , {0.0f,0.0f});
	explanationSprite_[1]->Create(explanationHandle_[1] , {0.0f,0.0f});
	explanationSprite_[2]->Create(explanationHandle_[2] , {0.0f,0.0f});


	explanationCurrentNumber_ = 0;



	explanationSEHandle_=audio_->SoundLoadWave("Resources/Music/SE/Deside/Deside1.wav");

	//BGM
	bgmAudio_ =  Audio::GetInstance();
	bgmHandle_ = audio_->SoundLoadWave("Resources/Music/BGM/Explanation/Explanation.wav");

	bgmAudio_->SoundPlayWave(bgmHandle_, 1);

	//SE
	selectSEAudio_ = Audio::GetInstance();
	explanationSEHandle_ = audio_->SoundLoadWave("Resources/Music/SE/Select/Select.wav");
	returnSEHandle_= audio_->SoundLoadWave("Resources/Music/SE/Deside/Deside2.wav");




}

/// <summary>
/// 更新
/// </summary>
void ExplanationScene::Update(GameManager* gameManager) {
	
	ImGui::Begin("Explanation");
	ImGui::InputFloat4("transparency", &transparency_.x);
	ImGui::InputInt("loadingTime", &loadingTime);
	
	
	
	//FadeIn
	//黒背景が透明になっていっていく
	//疑似FadeIn
	if (isFadeInMode_ == true) {
		transparency_.w -= fadeInterval_;
	}
	if (transparency_.w <= 0.0f) {
		isFadeInMode_ = false;
	}


	if (isFadeInMode_ == false) {
		isTriggerB_;

		XINPUT_STATE joyState{};
		if (Input::GetInstance()->GetJoystickState(joyState)) {

			//決定
			if (isTriggerB_ == false) {
				if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B && triggerButtonBTime_==0) {
					triggerButtonBTime_ += 1;
				}
			}
			//1の時移動
			if ((triggerButtonBTime_==1 )) {
				

				isTriggerB_ = true;
				triggerButtonBTime_ = 0;

				explanationCurrentNumber_ += 1;

				if (explanationCurrentNumber_ == 2) {
					isFadeOutMode_ = true;
					selectSEAudio_->SoundPlayWave(returnSEHandle_, 0);
					bgmAudio_->StopAudio(bgmHandle_);
				}
				else {
					selectSEAudio_->SoundPlayWave(explanationSEHandle_, 0);
				}


			}

			if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B)==0) {
				isTriggerB_ = false;
				triggerButtonBTime_ = 0;
			}

			


		}
	}


	//FadeOut
	if (isFadeOutMode_ == true) {
		transparency_.w += fadeInterval_;

		///完全に暗くなるとローディングへ
		if (transparency_.w >= 1.0f) {
			loadingTime += 1;
		}
	}

	//スプライトの透明度をここで設定
	backSprite_->SetColor({COLOR_BIND,COLOR_BIND,COLOR_BIND,transparency_.w});
	


	if (loadingTime > SECOND_ * 3 ) {
		gameManager->ChangeScene(new SelectScene());
			
	}	



	ImGui::End();
}

/// <summary>
/// 描画
/// </summary>
void ExplanationScene::Draw(GameManager* gameManager) {
	if (explanationCurrentNumber_ == 0) {
		explanationSprite_[0]->Draw();
	}
	
	if (explanationCurrentNumber_ == 1) {
		explanationSprite_[1]->Draw();
	}

	if (explanationCurrentNumber_ == 2 ||
		explanationCurrentNumber_ == 3) {
		explanationSprite_[2]->Draw();
	}

	backSprite_->Draw();



}






