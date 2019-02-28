#include "stdafx.h"
#include "Level.h"

Level::Level(Allocator *allocator) :StaticMeshList(*allocator), sky(nullptr)
{

}

Level::~Level()
{
	allocatorFC::deallocateDelete(*allocator, sky);
}


void Level::AddStaticMesh(StaticMesh* sm)
{
	StaticMeshList.Push_Back(sm);
}

void Level::AddSky()
{
	allocatorFC::allocateNew<Sky>(*allocator, sky);
}
