#pragma once
#include "Model/Model.h"
#include "TransformationMatrix/WorldTransform.h"

class Box{
public:
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

private:
	//箱のモデル
	std::unique_ptr<Model> model_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform worldTransform_{};
	//
	bool onPlayer_ = false;
};

