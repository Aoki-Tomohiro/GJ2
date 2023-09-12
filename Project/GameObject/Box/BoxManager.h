#pragma once
#include "Box.h"
#include <array>
#include <memory>
#include "Audio/Audio.h"

class Player;

class BoxManager{
public:
	//箱のスポーン間隔
	static const int boxSpornTime = 60 * 10;
	//エリアの数
	static const int kAreaMax = 3;

	//エリアの構造体
	struct Area {
		std::unique_ptr<Model> model = nullptr;
		WorldTransform worldTransform{};
		uint32_t kAreaAttribute;
	};

	enum AreaAttribute {
		kRed,
		kBlue,
		kGreen,
	};

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
	/// 自キャラをセット
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; };

private:
	/// <summary>
	/// 箱を追加
	/// </summary>
	void AddBox();

	/// <summary>
	/// 当たり判定を確認
	/// </summary>
	void CheckCollision();

	/// <summary>
	/// エリアにある箱を確認
	/// </summary>
	void CheckArea();

private:
	//エリア
	std::array<Area, kAreaMax> areas_{};
	//エリアのサイズ
	Vector3 areaSize_{ 1.0f,1.0f,1.0f };
	//箱のリスト
	std::list<std::unique_ptr<Box>> boxs_{};
	//箱のスポーンタイマー
	int32_t timer_ = boxSpornTime;
	//自キャラ
	Player* player_ = nullptr;
	//フィールドにある箱の数
	int32_t boxCount_ = 0;
	int32_t redBoxCount_ = 0;
	int32_t blueBoxCount_ = 0;
	int32_t greenBoxCount_ = 0;
	//エリアにある箱の数
	int32_t inBoxCount_ = 0;
	//テクスチャ
	uint32_t textureHandle_ = 0;

	Audio* powerUpSE_ = nullptr;
	uint32_t powerUpSEHandle_ = 0u;

};

