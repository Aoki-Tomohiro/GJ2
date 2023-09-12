#include "ExplanationScene.h"
#include "GameManager/GameManager.h"
#include "SelectScene/SelectScene.h"


/// <summary>
/// コンストラクタ
/// </summary>
ExplanationScene::ExplanationScene() {

}

/// <summary>
/// デストラクタ
/// </summary>
ExplanationScene::~ExplanationScene() {
	for (int i = 0; i < EXPLANATION_NUMBER_; i++) {
		delete explanationSprite_[0];
		delete explanationSprite_[1];
	}
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




	//スプライトの初期化
	//Fade用
	backSprite_ = new Sprite();
	backTextureHandle_ = textureManager_->Load("Project/Resources/Black/BlackTexture.png");
	backSprite_->Create(backTextureHandle_, { 0.0f,0.0f });

	//透明度
	COLOR_BIND = 1.0f;
	//1.0fで真っ暗
	transparency_ = { COLOR_BIND,COLOR_BIND,COLOR_BIND,1.0f };
	fadeInterval_ = 10.0 / 256.0f;


	const int SECOND_ = 60;
	int32_t loadingTime = 0;

	//説明
	for (int i = 0; i < EXPLANATION_NUMBER_; i++) {
		explanationSprite_[0] = new Sprite();
		explanationSprite_[1] = new Sprite();


		explanationHandle_[0] = textureManager_->Load("Project/Resources/Explanation/Explanation1.png");
		explanationHandle_[1] = textureManager_->Load("Project/Resources/Explanation/Explanation2.png");


		explanationSprite_[0]->Create(explanationHandle_[0] , {0.0f,0.0f});
		explanationSprite_[1]->Create(explanationHandle_[1] , {0.0f,0.0f});

		explanationCurrentNumber_ = 0;
	}
}

/// <summary>
/// 更新
/// </summary>
void ExplanationScene::Update(GameManager* gameManager) {
	//FadeIn
	//黒背景が透明になっていっていく
	//疑似FadeIn
	if (isFadeInMode_ == true) {
		transparency_.w -= fadeInterval_;
	}
	if (transparency_.w <= 0.0f) {
		isFadeInMode_ = false;
	}





	//FadeOut
	if (isFadeOutMode_ == true) {
		bgmAudio_->StopAudio(bgmHandle_);
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

}

/// <summary>
/// 描画
/// </summary>
void ExplanationScene::Draw(GameManager* gameManager) {
	explanationSprite_[0]->Draw();






}






