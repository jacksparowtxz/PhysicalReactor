
cbuffer ViewProjectionConstantBuffer : register(b0)
{
    matrix WorldViewProj;
    //matrix model;
}




struct VertexShaderInput
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float3 TangentL : TANGENT;
    float2 Tex : TEXCOORD;
};

// 通过像素着色器传递的每个像素的颜色数据。
struct PixelShaderInput
{
    float4 PosH : SV_POSITION;
    float3 PosL : POSITION;

};

PixelShaderInput main(VertexShaderInput Input)
{
    PixelShaderInput PSInput;
    PSInput.PosH = mul(float4(Input.PosL, 1.0f), WorldViewProj).xyww;
    PSInput.PosL = Input.PosL;
    return PSInput;
}
