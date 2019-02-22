// 存储用于构成几何图形的三个基本列优先矩阵的常量缓冲区。
cbuffer ViewProjectionConstantBuffer : register(b0)
{
    matrix View;
    matrix Projection;
    matrix TexTransform;
    matrix WorldInvTranspose;
};


cbuffer ModelConstantBuffer : register(b1)
{
	matrix ModelWorld;
};



// 用作顶点着色器输入的每个顶点的数据。
struct VertexShaderInput
{
	float3 PosL     : POSITION;
	float3 NormalL  : NORMAL;
	float3 TangentL : TANGENT;
	float2 Tex      : TEXCOORD;
};

// 通过像素着色器传递的每个像素的颜色数据。
struct PixelShaderInput
{
        float4 PosH : SV_POSITION;
        float3 PosW : POSITION;
        float3 NormalW : NORMAL;
        float3 TangentW : TANGENT;
        float2 Tex : TEXCOORD;
};

// 用于在 GPU 上执行顶点处理的简单着色器。
PixelShaderInput main(VertexShaderInput input)
{
	PixelShaderInput output;
	float4 pos = float4(input.PosL, 1.0f);

	// 将顶点位置转换为投影空间。
	pos = mul(pos, ModelWorld);
    output.PosW = pos.xyz;
	pos = mul(pos, View);
	pos = mul(pos, Projection);
	output.PosH = pos;
    output.NormalW = mul(input.NormalL, (float3x3) WorldInvTranspose);
    output.Tex = mul(float4(input.Tex, 1.0f, 1.0f),TexTransform).xy;
    output.TangentW = mul(input.TangentL, (float3x3) ModelWorld);

	return output;
}
