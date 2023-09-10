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


	//初期では1を選択している
	stageNumber_ = 1;

	

	//スプライトの初期化
	//Fade用
	backSprite_ = new Sprite();
	backTextureHandle_ = textureManager_->Load("Project/Resources/Black/BlackTexture.png");
	Vector2 backTexturePosition = { 0.0f,0.0f };
	backSprite_->Create(backTextureHandle_, backTexturePosition);

	

	//Timer系の変数
	triggerButtonBTime_ = 0;
	fadeOutTime_ = 0;
	isFadeOutMode_ = false;

	//透明度
	COLOR_BIND = 1.0f;
	transparency_ = { COLOR_BIND,COLOR_BIND,COLOR_BIND,0.0f };
	fadeOutInterval_ = 10.0 / 256.0f;


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
	Vector2 backToTitlePosition = { 400.0f,500.0f };
	backToTitleSprite_->Create(backToTitleTexture_, backToTitlePosition);
	
	
	
	for (int i = 0; i < 2; i++) {
		stageIconSprite_[i] = new Sprite();
	}
	stageIconTexture_[0]= textureManager_->Load("Project/Resources/Select/Stage1.png");
	stageIconTexture_[1]= textureManager_->Load("Project/Resources/Select/Stage2.png");
	
	//アイコンの間隔
	WIDTH_INTERVAL_ = 200.0f;
	stageIconPosition[0] = { backToTitlePosition.x + WIDTH_INTERVAL_ * 1.0f,backToTitlePosition.y };
	stageIconPosition[1] = { backToTitlePosition.x + WIDTH_INTERVAL_ * 2.0f,backToTitlePosition.y };
		
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
	ImGui::Text("Space To GameScene");
	ImGui::Text("SelectKey Left Or Right");

	
	//2つ用意で
	if (input_->IsPushKeyEnter(DIK_SPACE)) {
		gameManager->ChangeScene(new GameScene());
	}

	XINPUT_STATE joyState{};

	if (Input::GetInstance()->GetJoystickState(joyState)) {

		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_B) {
			triggerButtonBTime_ += 1;
			
		}
		
	}

	//トリガー代わり
	if (triggerButtonBTime_ == 1) {
		isFadeOutMode_ = true;
			
	}

	if (isFadeOutMode_ == true) {
		transparency_.w += fadeOutInterval_;
		

		///完全に暗くなるとローディングへ
		if (transparency_.w >= 1.0f) {
			loadingTime += 1;
		}
	}
	//スプライトの透明度をここで設定
	backSprite_->SetColor({COLOR_BIND,COLOR_BIND,COLOR_BIND,transparency_.w});
	
	//Loading
	if (loadingTime > SECOND_ * 3) {
		gameManager->ChangeScene(new GameScene());	
	}
		
	


	//左右キーで移動
	if (input_->IsPushKeyEnter(DIK_RIGHT)) {
		ImGui::Text("Right");
		//中にGetterを入れたら大丈夫だった
		if (cursorSprite_->GetTranslation().x < stageIconPosition[1].x) {
			cursorSprite_->SetTranslation({ cursorSprite_->GetTranslation().x+ WIDTH_INTERVAL_,cursorPosition.y });
		
		}
		else{
			cursorSprite_->SetTranslation(cursorSprite_->GetTranslation());
		}
		
	}
	if (input_->IsPushKeyEnter(DIK_LEFT)) {
		ImGui::Text("Left");

		if (cursorSprite_->GetTranslation().x > 400.0f) {
			cursorSprite_->SetTranslation({ cursorSprite_->GetTranslation().x - WIDTH_INTERVAL_,cursorPosition.y });
		}
		else{
			cursorSprite_->SetTranslation(cursorSprite_->GetTranslation());
		}
		

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





