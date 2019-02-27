
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

// ͨ��������ɫ�����ݵ�ÿ�����ص���ɫ���ݡ�
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
