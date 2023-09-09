#include "CollisionManager.h"

void CollisionManager::ClearColliderList() {
	//コライダーリストをクリア
	colliders_.clear();
	collidersAABB_.clear();
}

void CollisionManager::SetColliderList(Collider* collider) {
	//コライダーリストに登録
	colliders_.push_back(collider);
}

void CollisionManager::SetColliderListAABB(Collider* collider) {
	//コライダーリストに登録
	collidersAABB_.push_back(collider);
}

void CollisionManager::CheckAllCollisions() {
	//リスト内のペアを総当たり
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		//イテレータAからコライダーAを取得する
		Collider* colliderA = *itrA;
		//イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end(); ++itrB) {
			//イテレータBからコライダーBを取得する
			Collider* colliderB = *itrB;
			//ベアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}

	//リスト内のペアを総当たり
	std::list<Collider*>::iterator itrAAABB = collidersAABB_.begin();
	for (; itrAAABB != collidersAABB_.end(); ++itrAAABB) {
		//イテレータAからコライダーAを取得する
		Collider* colliderA = *itrAAABB;
		//イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::list<Collider*>::iterator itrBAABB = itrAAABB;
		itrBAABB++;
		for (; itrBAABB != collidersAABB_.end(); ++itrBAABB) {
			//イテレータBからコライダーBを取得する
			Collider* colliderB = *itrBAABB;
			//ベアの当たり判定
			CheckCollisionPairAABB(colliderA, colliderB);
		}
	}
}

void CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB) {
	//衝突フィルタリング
	if ((colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) == 0||
		(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()) == 0) {
		return;
	}
	//コライダーAのワールド座標を取得
	Vector3 posA = colliderA->GetWorldPosition();
	//コライダーBのワールド座標を取得
	Vector3 posB = colliderB->GetWorldPosition();
	//コライダーAとコライダーBの距離を計算
	float distance = Length(Subtract(posA, posB));
	//球と球の交差判定
	if (distance <= colliderA->GetRadius() + colliderB->GetRadius()) {
		//コライダーAの衝突時コールバックを呼び出す
		colliderA->OnCollision();
		//コライダーBの衝突時コールバックを呼び出す
		colliderB->OnCollision();
	}
}

void CollisionManager::CheckCollisionPairAABB(Collider* colliderA, Collider* colliderB) {
	//衝突フィルタリング
	if ((colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) == 0 ||
		(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()) == 0) {
		return;
	}
	//コライダーAのワールド座標を取得
	Vector3 posA = colliderA->GetWorldPosition();
	//コライダーBのワールド座標を取得
	Vector3 posB = colliderB->GetWorldPosition();
	//コライダーAのAABBを取得
	AABB aabbA = colliderA->GetAABB();
	//コライダーBのAABBを取得
	AABB aabbB = colliderB->GetAABB();
	if (posA.x + aabbA.min.x <= posB.x + aabbB.max.x && posA.x + aabbA.max.x >= posB.x + aabbB.min.x &&
		posA.y + aabbA.min.y <= posB.y + aabbB.max.y && posA.y + aabbA.max.y >= posB.y + aabbB.min.y &&
		posA.z + aabbA.min.z <= posB.z + aabbB.max.z && posA.z + aabbA.max.z >= posB.z + aabbB.min.z) {
		//コライダーAの衝突時コールバックを呼び出す
		colliderA->OnCollision();
		//コライダーBの衝突時コールバックを呼び出す
		colliderB->OnCollision();
	}
}