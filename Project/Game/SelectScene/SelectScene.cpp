#include "SelectScene.h"
#include "GameManager/GameManager.h"
#include "GameScene/GameScene.h"
#include "GameSceneRobot/GameSceneRobot.h"
#include "TitleScene/TitleScene.h"
#include "ExplanationScene/ExplanationScene.h"
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
	//Spriteのデストラクタも忘れずに
	delete silhouetteSprite_;

	
	delete backSprite_;
	
	for (int i = 0; i < STAGE_INDEX_; i++) {
		delete stageIconSprite_[i];
	}
	
		

	//タイトルに戻るためのアイコン
	delete backToTitleSprite_;

	
	//カーソル
	//一番後ろに描画しないと見えない
	delete cursorSprite_;



	

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


	//初期では0を選択している
	stageNumber_ = 0;

	

	//スプライトの初期化
	//Fade用
	backSprite_ = new Sprite();
	backTextureHandle_ = textureManager_->Load("Resources/Black/BlackTexture.png");
	Vector2 backTexturePosition = { 0.0f,0.0f };
	backSprite_->Create(backTextureHandle_, backTexturePosition);

	

	//Timer系の変数
	triggerButtonBTime_ = 0;
	triggerButtonLeftTime_ = 0;
	triggerButtonRightTime_ = 0;

	isTriggerB_ = false;
	isTriggerLeft_ = false;
	isTriggerRight_ = false;

	fadeInTime_ = 0.0f;
	fadeOutTime_ = 0.0f;
	isFadeInMode_ = true;
	isFadeOutMode_ = false;

	

	//透明度
	COLOR_BIND = 1.0f;
	//1.0fで真っ暗
	transparency_ = { COLOR_BIND,COLOR_BIND,COLOR_BIND,1.0f };
	fadeInterval_ = 10.0 / 256.0f;


	const int SECOND_ = 60;
	int32_t loadingTime = 0;




	
	//敵のシルエットが描かれているのが良いかも
	for (int i = 0; i < 4; i++) {
		silhouetteSprite_[i] = new Sprite();
		
	}
	
	silhouetteTextureHandle_[0]= textureManager_->Load("Resources/Select/Select1.png");
	silhouetteTextureHandle_[1]= textureManager_->Load("Resources/Select/Select2.png");
	
	Vector2 position_ = {0.0f,0.0f};
	silhouetteSprite_[0]->Create(silhouetteTextureHandle_[0], position_);
	silhouetteSprite_[1]->Create(silhouetteTextureHandle_[1], position_);
	

	
	//ステージアイコン
	//1,2,
	//←,?
	//タイトルに戻る
	backToTitleSprite_ = new Sprite();
	backToTitleTexture_ = textureManager_->Load("Resources/Select/ToTitle.png");
	backToTitlePosition_ = { 375.0f,500.0f };
	backToTitleSprite_->Create(backToTitleTexture_, backToTitlePosition_);
	
	
	
	for (int i = 0; i < STAGE_INDEX_; i++) {
		stageIconSprite_[i] = new Sprite();
	}
	stageIconTexture_[0]= textureManager_->Load("Resources/Select/Stage1.png");
	stageIconTexture_[1]= textureManager_->Load("Resources/Select/Stage2.png");
	stageIconTexture_[2]= textureManager_->Load("Resources/Select/Explanation.png");
	

	//アイコンの間隔
	WIDTH_INTERVAL_ = 150.0f;
	HEIGHT_INTERVAL_ = 100.0f;
	stageIconPosition[0] = { backToTitlePosition_.x + WIDTH_INTERVAL_ * 1.0f,backToTitlePosition_.y + WIDTH_INTERVAL_ * 0.0f };
	stageIconPosition[1] = { backToTitlePosition_.x + WIDTH_INTERVAL_ * 2.0f,backToTitlePosition_.y + WIDTH_INTERVAL_ * 0.0f };
	stageIconPosition[2] = { backToTitlePosition_.x + WIDTH_INTERVAL_ * 3.0f,backToTitlePosition_.y + WIDTH_INTERVAL_ * 0.0f };

	stageIconSprite_[0]->Create(stageIconTexture_[0], stageIconPosition[0]);
	stageIconSprite_[1]->Create(stageIconTexture_[1], stageIconPosition[1]);
	stageIconSprite_[2]->Create(stageIconTexture_[2], stageIconPosition[2]);
	
	//カーソル
	cursorSprite_ = new Sprite();
	cursorTexture_ = textureManager_->Load("Resources/Select/Cursor.png");
	cursorPosition = {backToTitlePosition_ };
	cursorSprite_->Create(cursorTexture_, cursorPosition);



	
	
	
	explanationChange_ = false;



	//Music
	bgmAudio_ = Audio::GetInstance();
	bgmHandle_ = audio_->SoundLoadWave("Resources/Music/BGM/Select/SelectBGM.wav");

	bgmAudio_->SoundPlayWave(bgmHandle_, true);


	//SE
	startSEAudio_ =  Audio::GetInstance();
	startSEHandle_=audio_->SoundLoadWave("Resources/Music/SE/Deside/Start.wav");
	returnSEHandle_=audio_->SoundLoadWave("Resources/Music/SE/Deside/Deside2.wav");
	explanationSEHandle_=audio_->SoundLoadWave("Resources/Music/SE/Deside/Deside1.wav");


	selectSEAudio_ = Audio::GetInstance();;
	selectSEHandle_ = audio_->SoundLoadWave("Resources/Music/SE/Select/Select.wav");


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
	ImGui::Text("B To GameScene");
	ImGui::Text("Select Key Left Or Right");
	ImGui::InputInt("triggerLeftTime", &triggerButtonLeftTime_);

	ImGui::InputFloat2("cursorPosition", &cursorPosition.x);

	ImGui::InputInt("stageNumber_", &stageNumber_);
	
	
	//FadeIn
	//黒背景が透明になっていっていく
	//疑似FadeIn
	if (isFadeInMode_ == true) {
		transparency_.w -= fadeInterval_;
	}
	if (transparency_.w <= 0.0f) {
		isFadeInMode_ = false;
	}

#pragma region 選択
	
	if (cursorSprite_->GetTranslation().x == backToTitlePosition_.x) {
		stageNumber_ = 0;
	}
	if (cursorSprite_->GetTranslation().x == stageIconPosition[0].x) {
		stageNumber_ = 1;
	}
	if (cursorSprite_->GetTranslation().x == stageIconPosition[1].x) {
		stageNumber_ = 2;
	}
	if (cursorSprite_->GetTranslation().x == stageIconPosition[2].x) {
		stageNumber_ = 3;
	}

	//2つ用意で
	if (isFadeInMode_ == false) {

		XINPUT_STATE joyState{};
		if (Input::GetInstance()->GetJoystickState(joyState)) {

			//決定
			if (isTriggerB_ == false) {
				if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
					triggerButtonBTime_ += 1;
				}
			}
			

			
			//十字ボタンで選択
			//左
			if (isTriggerLeft_ == false ) {
				if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT && triggerButtonLeftTime_==0) {
					
					triggerButtonLeftTime_ += 1;
				
				}
			}
			
			if (isTriggerRight_ == false) {
				if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT&& triggerButtonRightTime_==0) {
				
					triggerButtonRightTime_ += 1;
				}
			}
			

			//1の時移動
			if ((triggerButtonLeftTime_==1 )) {
				selectSEAudio_->SoundPlayWave(selectSEHandle_, 0);

				isTriggerLeft_ = true;
				triggerButtonLeftTime_ = 0;

				if (stageNumber_ == 0) {
					cursorSprite_->SetTranslation({ cursorSprite_->GetTranslation().x,cursorPosition.y });
				}
				else {
					cursorSprite_->SetTranslation({ cursorSprite_->GetTranslation().x - WIDTH_INTERVAL_,cursorPosition.y });
					
					
				}

			}

			if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)==0) {
				isTriggerLeft_ = false;
				triggerButtonLeftTime_ = 0;
			}







			//右
			if ((triggerButtonRightTime_==1 )) {
				isTriggerRight_ = true;
				triggerButtonRightTime_ = 0;
				selectSEAudio_->SoundPlayWave(selectSEHandle_, 0);
				if (stageNumber_ == 3) {
					cursorSprite_->SetTranslation({ cursorSprite_->GetTranslation().x,cursorPosition.y });
				}
				else {
					cursorSprite_->SetTranslation({ cursorSprite_->GetTranslation().x + WIDTH_INTERVAL_,cursorPosition.y });
					
					
				}

			}

			if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)==0) {
				isTriggerRight_ = false;
				triggerButtonRightTime_ = 0;
			}








			//操作説明





		}
		
		//タイトルへ
		if (triggerButtonBTime_ == 1 && stageNumber_==0) {
			isFadeOutMode_ = true;
			//StartSE再生
			//ループ無し
			startSEAudio_->SoundPlayWave(returnSEHandle_, false);




		}
		if (triggerButtonBTime_ == 1 && stageNumber_==1) {
			isFadeOutMode_ = true;
			//StartSE再生
			//ループ無し
			startSEAudio_->SoundPlayWave(startSEHandle_, false);
		}
		
		if (triggerButtonBTime_ == 1 && stageNumber_==2) {
			isFadeOutMode_ = true;
			startSEAudio_->SoundPlayWave(startSEHandle_, false);
		}

		//操作説明へ
		if (triggerButtonBTime_ == 1 && stageNumber_==3) {
			isFadeOutMode_ = true;
			startSEAudio_->SoundPlayWave(explanationSEHandle_, false);
		}
	}
	


#pragma endregion


	//FadeOut
	if (isFadeOutMode_ == true) {
		bgmAudio_->StopAudio(bgmHandle_);
		transparency_.w += fadeInterval_;
		ImGui::Text("decide");

		///完全に暗くなるとローディングへ
		if (transparency_.w >= 1.0f) {
			loadingTime += 1;
		}
	}

	//スプライトの透明度をここで設定
	backSprite_->SetColor({COLOR_BIND,COLOR_BIND,COLOR_BIND,transparency_.w});
	

	//LoadingからTitleSceneへ
	if (loadingTime > SECOND_ * 3 && stageNumber_ == 0) {
		gameManager->ChangeScene(new TitleScene());
	}
	//LoadingからGameSceneへ
	if (loadingTime > SECOND_ * 3 && stageNumber_ == 1) {
		gameManager->ChangeScene(new GameScene());	
	}
	if (loadingTime > SECOND_ * 3 && stageNumber_ == 2) {
		gameManager->ChangeScene(new GameSceneRobot());
			
	}	
	if (loadingTime > SECOND_ * 3 && stageNumber_ == 3) {
		gameManager->ChangeScene(new ExplanationScene());
			
	}	

	
	
	ImGui::End();


}

/// <summary>
/// 描画
/// </summary>
void SelectScene::Draw(GameManager* gameManager) {
	
	//背景
	silhouetteSprite_->Draw();

	//StageIcon
	stageIconSprite_[0]->Draw();
	stageIconSprite_[1]->Draw();
	stageIconSprite_[2]->Draw();

	//タイトルに戻るためのアイコン
	backToTitleSprite_->Draw();
	
	//カーソル
	//後ろに描画しないと見えない
	cursorSprite_->Draw();


	//Fade用
	backSprite_->Draw();

}





