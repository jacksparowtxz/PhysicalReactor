#include "ShaderManager.h"
#include "Render/Renderer.h"
#include <memory>
#include <iostream>
#include "MISC/tinyxml2.h"
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <windows.h>




using namespace PRE;
using namespace tinyxml2;
using namespace std;



ShaderManager::ShaderManager()
{

}

ShaderManager::~ShaderManager()
{
	
}


void ShaderManager::LoadShader(const std::string& pathfilename,SHADERSTAGE type)
{
	//BYTE* data =new BYTE;
	//size_t datasize = 0;
	std::string path;
	std::string filename;
	//PRE::readByteData(pathfilename, &data, datasize);
	GraphicBlob* shaderblob = new GraphicBlob;
	SplitPath(pathfilename, path, filename);
	/////////////////////////
	const char* filenamechar = filename.c_str();
	int iSize;
	wchar_t* wfilename = nullptr;;
	iSize = MultiByteToWideChar(CP_ACP, 0, filenamechar, -1, NULL, 0);
	wfilename = (wchar_t *)malloc(iSize * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, 0, filenamechar, -1, wfilename, iSize);

	switch (type)
	{
	case PRE::VS_STAGE:
	{
		VertexShader* vs =new VertexShader;
		Renderer::GetDevice()->CreateVertexShader(wfilename, shaderblob, vs);
		VSMap[filename]=vs;
		CreateVertexLayout(filename,shaderblob);
	}
	break;
	case PRE::HS_STAGE:
	{
		HullShader* hs = new HullShader;
		Renderer::GetDevice()->CreateHullShader(wfilename, shaderblob, hs);
		HSMap[filename]=hs;
	}
	break;
	case PRE::DS_STAGE:
	{
		DomainShader* ds = new DomainShader;
		Renderer::GetDevice()->CreateDomainShader(wfilename, shaderblob, ds);
		DSMap[filename]= ds;
	}
	break;
	case PRE::GS_STAGE:
	{
		GeometryShader* gs = new GeometryShader;
		Renderer::GetDevice()->CreateGemotryShader(wfilename, shaderblob, gs);
		GSMap[filename]=gs;
	}
	break;
	case PRE::PS_STAGE:
	{
	    PixelShader* ps = new PixelShader;
		Renderer::GetDevice()->CreatePixelShader(wfilename, shaderblob, ps);
		PSMap[filename]= ps;
	}
	break;
	case PRE::CS_STAGE:
	{
	    ComputerShader* cs =  new ComputerShader;
		Renderer::GetDevice()->CreateComputerShader(wfilename, shaderblob, cs);
		CSMap[filename]= cs;
	}
	break;
	default:
		break;
	}
	delete shaderblob;
}


/*
struct DeletePoint
{
	template <typename T>
	void operator()(const T& pX) const
	{
		delete pX.second;
	}
};
*/


void ShaderManager::CreateShader()
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile("Shader.xml");
	XMLElement* Root = doc.FirstChildElement();
	std::vector<std::string> shadernames;
	for (XMLElement* elem = Root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement("TYPE")) 
	{
		const char* shadertype=elem->Attribute("category");
		string type(std::move(shadertype));
		if (type=="VS/")
		{
			for (XMLElement* temp = elem->FirstChildElement("Name"); temp != NULL; elem->LastChildElement("Name"), temp = temp->NextSiblingElement("Name"))
			{
				const char* r = temp->GetText();
				shadernames.push_back(type+std::move(r));
			}
			for (auto i:shadernames)
			{
				LoadShader(i, VS_STAGE);
				
			}
			shadernames.clear();
		}
		else if(type=="PS/")
		{
			for (XMLElement* temp = elem->FirstChildElement("Name"); temp != NULL; elem->LastChildElement("Name"), temp = temp->NextSiblingElement("Name"))
			{
				const char* r = temp->GetText();
				shadernames.push_back(type+std::move(r));
			}
			for (auto i : shadernames)
			{
				LoadShader(i, PS_STAGE);
				
			}
			shadernames.clear();
		}
		else if (type == "HS/")
		{
			for (XMLElement* temp = elem->FirstChildElement("Name"); temp != NULL; elem->LastChildElement("Name"), temp = temp->NextSiblingElement("Name"))
			{
				const char* r = temp->GetText();
				shadernames.push_back(type+std::move(r));
			}
			for (auto i : shadernames)
			{
				LoadShader(i, HS_STAGE);
				
			}
			shadernames.clear();
		}
		else if (type == "GS/")
		{
			for (XMLElement* temp = elem->FirstChildElement("Name"); temp != NULL; elem->LastChildElement("Name"), temp = temp->NextSiblingElement("Name"))
			{
				const char* r = temp->GetText();
				shadernames.push_back(type+std::move(r));
			}
			for (auto i : shadernames)
			{
				LoadShader(i, GS_STAGE);
				
			}
			shadernames.clear();
		}
		else if (type == "DS/")
		{
			for (XMLElement* temp = elem->FirstChildElement("Name"); temp != NULL; elem->LastChildElement("Name"), temp = temp->NextSiblingElement("Name"))
			{
				const char* r = temp->GetText();
				shadernames.push_back(type+std::move(r));
			}
			for (auto i : shadernames)
			{
				LoadShader(i, DS_STAGE);
				
			}
			shadernames.clear();
		}
		else if (type == "CS/")
		{
			for (XMLElement* temp = elem->FirstChildElement("Name"); temp != NULL; elem->LastChildElement("Name"), temp = temp->NextSiblingElement("Name"))
			{
				const char* r = temp->GetText();
				shadernames.push_back(type+std::move(r));
			}
			for (auto i : shadernames)
			{
				LoadShader(i, CS_STAGE);
				
			}
			shadernames.clear();
		}
	}

}

void ShaderManager::CreateVertexLayout(const std::string& filename,GraphicBlob* Graphicblob)
{
	 VertexLayout* vertexlayout = new VertexLayout;
	const VertexLayoutDesc vertexDesc[] =
	{
		{ "POSITION", 0, FORMAT_R32G32B32_FLOAT, 0, 0, INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, INPUT_PER_VERTEX_DATA, 0 },
	};
	Renderer::GetDevice()->CreateInputLayout(vertexDesc, ARRAYSIZE(vertexDesc), Graphicblob, vertexlayout);
	VLMap[filename] = vertexlayout;
}

PRE::VertexShader* ShaderManager::GetVertexShader(const std::string& filename)
{
	return VSMap[filename];
}

PRE::PixelShader* ShaderManager::GetPixelShader(const std::string& filename)
{
	return PSMap[filename];
}

PRE::HullShader* ShaderManager::GetHullShader(const std::string& filename)
{
	return HSMap[filename];
}

PRE::DomainShader* ShaderManager::GetDomainShader(const std::string& filename)
{
	return DSMap[filename];
}

PRE::GeometryShader* ShaderManager::GetGeoShader(const std::string& filename)
{
	return GSMap[filename];
}

PRE::ComputerShader* ShaderManager::GetComputerShader(const std::string& filename)
{
	return CSMap[filename];
}

PRE::VertexLayout* ShaderManager::GetVertexLayout(const std::string& filename)
{
	return VLMap[filename];
}

void ShaderManager::GetPSO(Material* material, GraphicPSO* pso)
{
	pso->desc.vs = VSMap["SkyVS.hlsl"];
	pso->desc.ps = PSMap["SkyPS.hlsl"];
	pso->desc.VL = VLMap["SkyVS.hlsl"];
}

void ShaderManager::GetPSO(OBJECTTYPE objecttype, GraphicPSO* pso)
{
	switch (objecttype)
	{
	case TYPE_VOID:
		return;
		break;
	case TYPE_SKY:
		pso->desc.vs = VSMap["SkyVS.hlsl"];
		pso->desc.ps = PSMap["SkyPS.hlsl"];
		pso->desc.VL = VLMap["SkyVS.hlsl"];
		break;
	case TYPE_STATICMESH:
		pso->desc.vs = VSMap["vs.hlsl"];
		pso->desc.ps = PSMap["ps.hlsl"];
		pso->desc.VL = VLMap["vs.hlsl"];
		break;
	default:
		break;
	}
}
