#include "ViewProjection.h"

void ViewProjection::UpdateMatrix() {
	//Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotation_, translation_);
	//matView_ = Inverse(worldMatrix);
	//matProjection_ = MakePerspectiveFovMatrix(fov_, aspectRatio_, nearClip_, farClip_);

	//平行移動行列の計算
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translation_);
	//回転行列の計算
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotation_.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotation_.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotation_.z);
	Matrix4x4 rotateMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));
	//ビュー行列の計算
	matView_ = Multiply(Inverse(translateMatrix), Inverse(rotateMatrix));
	//プロジェクション行列の計算
	matProjection_ = MakePerspectiveFovMatrix(fov_, aspectRatio_, nearClip_, farClip_);
}