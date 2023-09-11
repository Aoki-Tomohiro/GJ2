#pragma once
#include "Model/Model.h"
#include "TransformationMatrix/WorldTransform.h"

class Box{
public:
	enum pos {
		kLeftTop,
		kRightTop,
		kLeftBottom,
		kRightBottom,
	};

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(uint32_t attribute);

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
	/// ワールドポジションを取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition();

	/// <summary>
	/// 自キャラとの当たり判定
	/// </summary>
	/// <param name="move"></param>
	void OnCollisionPlayer(const Vector3& velocity);

	/// <summary>
	/// 箱との当たり判定
	/// </summary>
	/// <param name="onPlayer"></param>
	/// <param name="velocity"></param>
	void OnCollisionBox(const Vector3& velocity);

	/// <summary>
	/// 自機と触れているかのフラグを取得
	/// </summary>
	bool GetOnPlayer() { return onPlayer_; };

	/// <summary>
	/// エリアのフラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetInArea(bool flag) { inArea_ = flag; };

	/// <summary>
	/// エリアのフラグを取得
	/// </summary>
	/// <returns></returns>
	bool GetInArea() { return inArea_; };

	/// <summary>
	/// 死亡フラグを設定
	/// </summary>
	/// <param name="flag"></param>
	void SetIsDead(bool flag) { isDead_ = flag; };

	/// <summary>
	/// 死亡フラグを設定
	/// </summary>
	/// <returns></returns>
	bool GetIsDead() { return isDead_; };

	/// <summary>
	/// 属性を取得
	/// </summary>
	/// <returns></returns>
	uint32_t GetAttribute() { return attribute_; };

private:
	//箱のモデル
	std::unique_ptr<Model> model_ = nullptr;
	//テクスチャ
	uint32_t textureHandle_ = 0;
	//ワールドトランスフォーム
	WorldTransform worldTransform_{};
	//属性
	uint32_t attribute_;
	//自キャラと触れているか
	bool onPlayer_ = false;
	//エリアに入っているか
	bool inArea_ = false;
	//箱の死亡フラグ
	bool isDead_ = false;
};

