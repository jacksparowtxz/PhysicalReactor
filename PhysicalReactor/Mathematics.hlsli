#include"Common.hlsli"

////////////spherical-harmonics Function
/////////////////////////////////////"Simplifying-Spherical-Harmonics-for-Lighting"Dr Graham Hazel

/*float3 Coefficients[9] =
{
 2.0952,    2.0944,    1.91702,
0.653221,	0.693606,	0.762548,
-0.0489448,	-0.0396268	,-0.0285173,
0.0441169	,0.0430626,	0.0677261,
0.03926,	0.0186167,	0.00929239,
0.183267,	0.158966	,0.1199,
-0.168802,	-0.157308	,-0.159969,
-0.0361812,	-0.0386558	,-0.0466892,
-0.199456,	-0.169909,	-0.153581,
};*/

float3 spherical_harmonics_Irrandice(float3 Coefficients[9],float3 n)
{

    float SHbasis[16];
    float x = n.x;
    float y = n.y;
    float z = n.z;
    float x2 = x * x;
    float y2 = y * y;
    float z2 = z * z;

    SHbasis[0] = 1.0f / sqrt(1.0f / 4.0f*PI);
    SHbasis[1] = sqrt(3.0f / (4.0f * PI)) * y;
    SHbasis[2] = sqrt(3.0f / (4.0f * PI)) * z;
    SHbasis[3] = sqrt(3.0f / (4.0f * PI)) * x;
    SHbasis[4] = 1.f / 2.f * sqrt(15.f / PI) * x * y;
    SHbasis[5] = 1.f / 2.f * sqrt(15.f / PI) * y * z;
    SHbasis[6] = 1.f / 4.f * sqrt(5.f / PI) * (-x * x - y * y + 2 * z * z);
    SHbasis[7] = 1.f / 2.f * sqrt(15.f / PI) * z * x;
    SHbasis[8] = 1.f / 4.f * sqrt(15.f / PI) * (x * x - y * y);

    float3 color = { 0.f, 0.f, 0.f };
    for (int i = 0; i < 9;i++)
    {
        color += Coefficients[i] * SHbasis[i];
    }

    return color;
}