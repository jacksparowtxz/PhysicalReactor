
Texture2DMS<float> input : register(t0);
RWTexture2D<float> output : register(u0);




[numthreads(16, 16, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    uint2 dim;
    uint sampleCount;
    input.GetDimensions(dim.x, dim.y, sampleCount);
    ///////////////////////////////OUT OF BOUND IN COMPUTE SHADER RETURN 0
    if (DTid.x>dim.x||DTid.y>dim.y)
    {
        return;
    }
    ///////////////////////////////////////////////////////////NEED NOT TO CHECK OUT OF BOUND
    float result = 0;
    for (uint i = 0; i < sampleCount;++i)
    {
        result = max(result, input.Load(DTid.xy, i).r);
    }

    output[DTid.xy] = result;
}