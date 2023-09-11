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
	
	delete stageIconSprite_[0];
	delete stageIconSprite_[1];
		

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
	//←,1,2,?
	//タイトルに戻る
	backToTitleSprite_ = new Sprite();
	backToTitleTexture_ = textureManager_->Load("Project/Resources/Select/ToTitle.png");
	backToTitlePosition_ = { 400.0f,500.0f };
	backToTitleSprite_->Create(backToTitleTexture_, backToTitlePosition_);
	
	
	
	for (int i = 0; i < 2; i++) {
		stageIconSprite_[i] = new Sprite();
	}
	stageIconTexture_[0]= textureManager_->Load("Project/Resources/Select/Stage1.png");
	stageIconTexture_[1]= textureManager_->Load("Project/Resources/Select/Stage2.png");
	
	//アイコンの間隔
	WIDTH_INTERVAL_ = 200.0f;
	stageIconPosition[0] = { backToTitlePosition_.x + WIDTH_INTERVAL_ * 1.0f,backToTitlePosition_.y };
	stageIconPosition[1] = { backToTitlePosition_.x + WIDTH_INTERVAL_ * 2.0f,backToTitlePosition_.y };
		
	stageIconSprite_[0]->Create(stageIconTexture_[0], stageIconPosition[0]);
	stageIconSprite_[1]->Create(stageIconTexture_[1], stageIconPosition[1]);
	
	//カーソル
	cursorSprite_ = new Sprite();
	cursorTexture_ = textureManager_->Load("Project/Resources/Select/Cursor.png");
	cursorPosition = { 400.0f,500.0f };
	cursorSprite_->Create(cursorTexture_, cursorPosition);



	

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
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) {
				triggerButtonLeftTime_ += 1;
				
			}
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) {
				triggerButtonRightTime_ += 1;
				
			}
			
		}
		


			//トリガー代わり
		if (triggerButtonBTime_ == 1) {
			isFadeOutMode_ = true;
		}
		if (triggerButtonLeftTime_ == 1) {
			triggerButtonLeftTime_ = 0;
			isTriggerLeft_ = true;
			
		}
		if (triggerButtonRightTime_ == 1) {
			triggerButtonRightTime_ = 0;
			isTriggerRight_ = true;
		}


		if (isTriggerLeft_ == true) {
			ImGui::Text("Left");
			
			//中にGetterを入れたら大丈夫だった
			if (cursorSprite_->GetTranslation().x > backToTitlePosition_.x) {
				cursorSprite_->SetTranslation({ cursorSprite_->GetTranslation().x - WIDTH_INTERVAL_,cursorPosition.y });
				
				isTriggerLeft_ = false;
			}
			else{
				cursorSprite_->SetTranslation(cursorSprite_->GetTranslation());
				
				isTriggerLeft_ = false;
			}
			
			
		}

		if (isTriggerRight_ == true) {
			ImGui::Text("Right");
			
			//中にGetterを入れたら大丈夫だった
			if (cursorSprite_->GetTranslation().x < stageIconPosition[1].x) {
				cursorSprite_->SetTranslation({ cursorSprite_->GetTranslation().x+ WIDTH_INTERVAL_,cursorPosition.y });
				
				isTriggerRight_ = false;
			}
			else{
				cursorSprite_->SetTranslation(cursorSprite_->GetTranslation());
				
				isTriggerRight_ = false;
			}

		}

		if (cursorSprite_->GetTranslation().x == backToTitlePosition_.x) {
			stageNumber_ = 0;
		}
		if (cursorSprite_->GetTranslation().x == stageIconPosition[0].x) {
			stageNumber_ = 1;
		}
		if (cursorSprite_->GetTranslation().x == stageIconPosition[1].x) {
			stageNumber_ = 2;
		}


	}
	

	//ステージナンバーを設定する


#pragma endregion


	//FadeOut
	if (isFadeOutMode_ == true) {
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





