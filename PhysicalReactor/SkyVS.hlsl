
cbuffer ViewProjectionConstantBuffer : register(b0)
{
    matrix View;
    matrix Projection;
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

    float4 Temp = mul(float4(Input.PosL, 1.0f), View);
  
    PSInput.PosH = mul(Temp, Projection).xyz;
    PSInput.PosL = Input.PosL;
    return PSInput;
}
