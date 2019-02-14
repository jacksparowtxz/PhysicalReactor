#include "stdafx.h"
#include "StaticMesh.h"


StaticMesh::StaticMesh():directory(""),aabb(nullptr)
{

}


StaticMesh::~StaticMesh()
{
	SAFE_DELETE(aabb);
	
}
