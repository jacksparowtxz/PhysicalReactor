#pragma once
#include"CommonInclude.h"



struct Vertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT3 Normal;
	DirectX::XMFLOAT3 Tangent;
	DirectX::XMFLOAT3 Bitgent;
	DirectX::XMFLOAT2 TexCoord;

	Vertex();

	Vertex(DirectX::XMFLOAT3 pos,
		DirectX::XMFLOAT3 normal,
		DirectX::XMFLOAT3 tangent,
		DirectX::XMFLOAT3 bitgent,
		DirectX::XMFLOAT2 texCoord);

};
