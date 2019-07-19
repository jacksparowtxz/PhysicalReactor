#include "stdafx.h"
#include "Vertex.h"


Vertex::Vertex()
{
	Pos = XMFLOAT3(0.f, 0.f, 0.f);
	Normal = XMFLOAT3(0.f, 0.f, 0.f);
	TexCoord = XMFLOAT2(0.f, 0.f);
	Bitgent= XMFLOAT3(0.f, 0.f, 0.f);
	Tangent = XMFLOAT3(0.f, 0.f, 0.f);
}

Vertex::Vertex(DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 normal,  DirectX::XMFLOAT3 tangent, DirectX::XMFLOAT3 bitgent, DirectX::XMFLOAT2 texCoord)
{
	Pos = pos;
	Normal = normal;
	Tangent = tangent;
	Bitgent = bitgent;
	TexCoord = texCoord;

}
