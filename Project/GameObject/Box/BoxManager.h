#pragma once
#include "Box.h"
#include <memory>

class Player;

class BoxManager{
public:
	//箱のスポーン間隔
	static const int boxSpornTime = 60 * 10;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 箱を追加
	/// </summary>
	void AddBox();

	/// <summary>
	/// 自キャラをセット
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; };

private:
	//並べるエリアのモデル
	std::unique_ptr<Model> model_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform worldTransform_{};
	//箱のリスト
	std::list<std::unique_ptr<Box>> boxs_{};
	//箱のスポーンタイマー
	int32_t timer_ = boxSpornTime;
	//自キャラ
	Player* player_ = nullptr;
};

