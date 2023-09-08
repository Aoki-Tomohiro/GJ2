#include "Sprite.hlsli"

struct TransformationMatrix {
	float32_t4x4 WVP;
};

ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);

struct VertexShaderInput {
	float32_t4 position : POSITION0;
	float32_t4 color : COLOR;
	float32_t2 texcoord : TEXCOORD0;
};

VertexShaderOutput main(VertexShaderInput input) {
	VertexShaderOutput output;
	output.position = mul(input.position, gTransformationMatrix.WVP);
	output.color = input.color;
	output.texcoord = input.texcoord;
	return output;
}