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
	//Spriteのデストラクタも忘れずに
	delete sprite_;

	
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
	backTextureHandle_ = textureManager_->Load("Project/Resources/Black/BlackTexture.png");
	Vector2 backTexturePosition = { 0.0f,0.0f };
	backSprite_->Create(backTextureHandle_, backTexturePosition);

	

	//Timer系の変数
	triggerButtonBTime_ = 0;
	triggerButtonLeftTime_ = 0;
	triggerButtonRightTime_ = 0;

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




	//背景的なスプライト
	//敵のシルエットが描かれているのが良いかも
	sprite_ = new Sprite();
	textureHandle_=textureManager_->Load("Project/Resources/Select/Select.png");
	Vector2 position_ = {0.0f,0.0f};
	sprite_->Create(textureHandle_, position_);
	

	
	//ステージアイコン
	//1,2,
	//←,?
	//タイトルに戻る
	backToTitleSprite_ = new Sprite();
	backToTitleTexture_ = textureManager_->Load("Project/Resources/Select/ToTitle.png");
	backToTitlePosition_ = { 400.0f,450.0f };
	backToTitleSprite_->Create(backToTitleTexture_, backToTitlePosition_);
	
	
	
	for (int i = 0; i < STAGE_INDEX_; i++) {
		stageIconSprite_[i] = new Sprite();
	}
	stageIconTexture_[0]= textureManager_->Load("Project/Resources/Select/Stage1.png");
	stageIconTexture_[1]= textureManager_->Load("Project/Resources/Select/Stage2.png");
	stageIconTexture_[2]= textureManager_->Load("Project/Resources/Select/Explanation.png");
	

	//アイコンの間隔
	WIDTH_INTERVAL_ = 200.0f;
	stageIconPosition[0] = { backToTitlePosition_.x + WIDTH_INTERVAL_ * 1.0f,backToTitlePosition_.y };
	stageIconPosition[1] = { backToTitlePosition_.x + WIDTH_INTERVAL_ * 2.0f,backToTitlePosition_.y };
	stageIconPosition[2] = { backToTitlePosition_.x + WIDTH_INTERVAL_ * 2.0f,backToTitlePosition_.y };

	stageIconSprite_[0]->Create(stageIconTexture_[0], stageIconPosition[0]);
	stageIconSprite_[1]->Create(stageIconTexture_[1], stageIconPosition[1]);
	stageIconSprite_[2]->Create(stageIconTexture_[2], stageIconPosition[2]);
	
	//カーソル
	cursorSprite_ = new Sprite();
	cursorTexture_ = textureManager_->Load("Project/Resources/Select/Cursor.png");
	cursorPosition = {backToTitlePosition_ };
	cursorSprite_->Create(cursorTexture_, cursorPosition);


	//Music
	bgmAudio_ = Audio::GetInstance();
	bgmHandle_ = audio_->SoundLoadWave("Project/Resources/Music/BGM/Select/SelectBGM.wav");

	bgmAudio_->SoundPlayWave(bgmHandle_, true);


	//SE
	startSEAudio_ =  Audio::GetInstance();
	startSEHandle_=audio_->SoundLoadWave("Project/Resources/Music/SE/Deside/Start.wav");
	

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
	ImGui::InputInt("triggerRightTime", &triggerButtonRightTime_);

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
	
	//if (cursorSprite_->GetTranslation().x == backToTitlePosition_.x) {
	//	triggerButtonLeftTime_ = 0;
	//	triggerButtonRightTime_ = 0;
	//	stageNumber_ = 0;
	//}
	//if (cursorSprite_->GetTranslation().x == stageIconPosition[0].x) {
	//	triggerButtonLeftTime_ = 0;
	//	triggerButtonRightTime_ = 0;
	//	stageNumber_ = 1;
	//}
	//if (cursorSprite_->GetTranslation().x == stageIconPosition[1].x) {
	//	triggerButtonLeftTime_ = 0;
	//	triggerButtonRightTime_ = 0;
	//	stageNumber_ = 2;
	//}


	//2つ用意で
	if (isFadeInMode_ == false) {

		XINPUT_STATE joyState{};

		if (Input::GetInstance()->GetJoystickState(joyState)) {

			//決定
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
				triggerButtonBTime_ += 1;
				
			}

			
			//十字ボタンで選択
			//左
			if (isTriggerLeft_ == false && triggerButtonLeftTime_==0) {
				if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) {
					isTriggerLeft_ = true;
					triggerButtonLeftTime_ += 1;
				
				}
			}
			
			if (isTriggerRight_ == false) {
				if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
				
					triggerButtonRightTime_ += 1;
				}
			}
			
			


			//1の時移動
			if ((triggerButtonLeftTime_==1 )) {
				

				if (isTriggerLeft_ == true) {
					cursorSprite_->SetTranslation({ cursorSprite_->GetTranslation().x - WIDTH_INTERVAL_,cursorPosition.y });
				}
				triggerButtonLeftTime_ = 0;

				if ((joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)==0) {
				
					isTriggerLeft_ = false;
					triggerButtonLeftTime_ = 0;
				}
			}
			if ((triggerButtonRightTime_==1 )) {
				isTriggerRight_ = false;
			}



		}
		
		/*if (isTriggerLeft_ == true) {
			cursorSprite_->SetTranslation({ cursorSprite_->GetTranslation().x - WIDTH_INTERVAL_,cursorPosition.y });
			triggerButtonLeftTime_ = 0;
		}*/
		if (isTriggerRight_ == true) {
			cursorSprite_->SetTranslation({ cursorSprite_->GetTranslation().x + WIDTH_INTERVAL_,cursorPosition.y });
			triggerButtonRightTime_ = 0;
		}




			//トリガー代わり
		if (triggerButtonBTime_ == 1) {
			isFadeOutMode_ = true;
			//StartSE再生
			//ループ無し
			startSEAudio_->SoundPlayWave(startSEHandle_, false);

			
		}
		//if (isTriggerLeft_ == true) {
		//	triggerButtonLeftTime_ += 1;
		//	
		//	
		//}
		//if (isTriggerRight_ == true) {
		//	triggerButtonRightTime_ += 1;
		//	
		//}


		//if (triggerButtonLeftTime_ == 1) {
		//	ImGui::Text("Left");
		//	
		//	//中にGetterを入れたら大丈夫だった
		//	if (cursorSprite_->GetTranslation().x > backToTitlePosition_.x) {
		//		cursorSprite_->SetTranslation({ cursorSprite_->GetTranslation().x - WIDTH_INTERVAL_,cursorPosition.y });
		//		
		//		isTriggerLeft_ = false;
		//	}
		//	else{
		//		cursorSprite_->SetTranslation(cursorSprite_->GetTranslation());
		//		triggerButtonLeftTime_ = 0;
		//		isTriggerLeft_ = false;
		//	}
		//	
		//	
		//}

		//if (triggerButtonRightTime_ == 1) {
		//	ImGui::Text("Right");
		//	cursorSprite_->SetTranslation({ cursorSprite_->GetTranslation().x+ WIDTH_INTERVAL_,cursorPosition.y });
		//		
		//	isTriggerRight_ = false;
		//	triggerButtonRightTime_ = 0;
		//	//中にGetterを入れたら大丈夫だった
		//	//if (cursorSprite_->GetTranslation().x < stageIconPosition[1].x) {
		//	//	
		//	//}
		//	/*else{
		//		cursorSprite_->SetTranslation(cursorSprite_->GetTranslation());
		//		
		//		isTriggerRight_ = false;
		//	}*/

		//}

		

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
	
	
	//LoadingからGameSceneへ
	if (loadingTime > SECOND_ * 3) {
		gameManager->ChangeScene(new GameScene());	
	}
		
	


	
	
	ImGui::End();


}

/// <summary>
/// 描画
/// </summary>
void SelectScene::Draw(GameManager* gameManager) {
	
	//背景
	sprite_->Draw();

	//StageIcon
	stageIconSprite_[0]->Draw();
	stageIconSprite_[1]->Draw();
	

	//タイトルに戻るためのアイコン
	backToTitleSprite_->Draw();
	
	//カーソル
	//後ろに描画しないと見えない
	cursorSprite_->Draw();


	//Fade用
	backSprite_->Draw();

}




