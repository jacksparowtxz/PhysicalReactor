#pragma once
#include "CommonInclude.h"
#include "Render/GraphicEnum.h"
#include "Render/RenderDevice.h"
#include "Render/ShaderManager.h"




class Renderer {

public:
	static PRE::RenderDevice* renderdevice;
	static PRE::RenderDevice*  GetDevice()
	{
		if (renderdevice != nullptr)
		{
			return renderdevice;
		}
		return nullptr;
	}
	static void CloseDevice();
	static ShaderManager* shadermanager;
};













/*
namespace Halo 
{
	struct Transform;
	struct Vertex;
	struct SkinnedVertex;
	struct Material;
	struct Object;
	struct Mesh;
	struct Object;
	struct Mesh;
	struct Armature;
	struct Bone;
	struct KeyFrame;
	struct SHCAM;
	struct Light;
	struct Decal;
	struct WorldInfo;
	struct Camera;
	struct Model;
	struct Scene;
	struct Cullable;
	struct EnivromentProbe;
	struct ForceField;
}

class Lines;
class Cube;
class Translator;
class Particle;
class EmittedParticle;
class HairParticle;
class Sprite;
class SPTree;
class TaskThread;
class PHYSICS;
class RenderTargte;
class ocean;
class oceanParameter;

struct Ray;



class Renderer
{
public:
	static Halo::RenderDevice* graphiscDevice;
	static Halo::RenderDevice* GetDevice()
	{
		assert(graphiscDevice != nullptr);
		return graphiscDevice;
	}

	static void Present(std::function<void()> drawToScreenl = nullptr,
		std::function<void()> drawToScreen2 = nullptr, std::function<void()> drawToScreen3 = nullptr);

	static Halo::Sampler *samplers[SSLOT_COUNT];
	static Halo::VertexShader *vertexshaders[VSTYPE_LAST];
	static Halo::PixelShader *pixelShaders[PSTYPE_LAST];
	static Halo::GeometryShader *GeometryShaders[GSTYPE_LAST];
	static Halo::HullShader *HullShaders[HSTYPE_LAST];
	static Halo::DomainShader *domainshaders[DSTYPE_LAST];
	static Halo::ComputerShader *computeshaders[CSTYPE_LAST];
	static Halo::VertexLayout *vertexLayouts[VSTYPE_LAST];
	static Halo::RasterizerState *rasterizers[RSTYPE_LAST];
	static Halo::DepthStencilState *depthStencils[DSTYPE_LAST];
	static Halo::BlendState *blendstates[BSTYPE_LAST];
	static Halo::GPUBuffer *constantbuffers[CBTYPE_LAST];
	static Halo::GPUBuffer *resourcebuffers[RSTYPE_LAST];
	static Halo::Texture *textures[TEXTYPE_LAST];
	static Halo::Sampler *customsamplers[SSTYPE_LAST];

	static Halo::GPURingBuffer *dynamicVertexBufferPool;

	static const Halo::FORMAT RTFormat_ldr = Halo::FORMAT_R8G8B8A8_UNORM;
	static const Halo::FORMAT RTFormat_hdr = Halo::FORMAT_R16G16B16A16_FLOAT;
	static const Halo::FORMAT RTFormat_gbuffer_0 = Halo::FORMAT_R8G8B8A8_UNORM;
	static const Halo::FORMAT RTFormat_gbuffer_1 = Halo::FORMAT_R16G16B16A16_FLOAT;
	static const Halo::FORMAT RTFormat_gbuffer_2 = Halo::FORMAT_R8G8B8A8_UNORM;
	static const Halo::FORMAT RTFormat_gbuffer_3 = Halo::FORMAT_R8G8B8A8_UNORM;

	static const Halo::FORMAT RTFormat_deferred_lightbuffer = Halo::FORMAT_R16G16B16A16_FLOAT;
	static const Halo::FORMAT RTFormat_lineardepth = Halo::FORMAT_R16_UNORM;
	static const Halo::FORMAT RTFormat_ssao = Halo::FORMAT_R8_UNORM;
	static const Halo::FORMAT RTFormat_waterripple = Halo::FORMAT_R8G8B8A8_SNORM;
	static const Halo::FORMAT RTFormat_normalmaps = Halo::FORMAT_R8G8B8A8_SNORM;
	static const Halo::FORMAT RTFormat_depthresolve = Halo::FORMAT_R32_FLOAT;
	static const Halo::FORMAT RTFormat_voxelradiance = Halo::FORMAT_R16G16B16A16_FLOAT;
	static const Halo::FORMAT RTFormat_envprobe = Halo::FORMAT_R16G16B16A16_FLOAT;
	static const Halo::FORMAT RTFormat_impostor_albdedo = Halo::FORMAT_R8G8B8A8_UNORM;
	static const Halo::FORMAT RTFormat_impostor_normal = Halo::FORMAT_R8G8B8A8_UNORM;
	static const Halo::FORMAT RTFormat_impostor_surface = Halo::FORMAT_R8G8B8A8_UNORM;

	static const Halo::FORMAT DSFormat_full = Halo::FORMAT_D32_FLOAT_S8X24_UINT;
	static const Halo::FORMAT DSFormat_full_alias = Halo::FORMAT_R32G8X24_TYPELESS;
	static const Halo::FORMAT DSFormat_small = Halo::FORMAT_D16_UNORM;
	static const Halo::FORMAT DSFormat_small_alias = Halo::FORMAT_R16_TYPELESS;

	static float GAMMA;
	static int SHADOWRES_2D;
	static int SHADOWRES_CUBE;
	static int SHADOWCOUNT_2D;
	static int SHADOWCOUNT_CUBE;
	static int SOFTSHADOWQUALITY_2D;
	static bool HAIRPARTICLEENABLE;
	static bool EMITTERSENABLED;
	static float SPECULARAA;
	static float renderTime;
	static float renderTime_Prev;
	static float deltatime;
	static XMFLOAT2 temporalAAJitter;
	static XMFLOAT2 temporalAAjitterPrev;
	static float RESOLUTIONSCALE;
	static bool TRANSPARENTSHADOWSENABLE;
	static bool ALPHACOMPSITIONENABLE;

	static void SetShadowProps2D(int resolution, int count, int softShadowQulity);
	static void SetShadowPropsCube(int resolution, int count);

	CBUFFER(WorldCB, CBSLOT_RENDERER_WORLD)
	{
		XMFLOAT2 mScreenWidthHeight;
		XMFLOAT2 mScreenWidthHeight_Inverse;
		XMFLOAT2 mInternalResolution;
		XMFLOAT2 mInternalResolution_Inverse;

		float mGamma;
		XMFLOAT3 mHorizon;
		XMFLOAT3 mZenith;
		float mCloudScale;
		XMFLOAT3 mFog;
		float mSpecularAA;
		float mVoxelRadianceDataSize;
		float mVoxelRadianceDataSize_Inverse;
		UINT mVoxelRadianceDataRes;
		float mVoxelRadianceDataRes_Inverse;
		UINT mVoxelRadianceDataMIPs;
		UINT mVoxelRadianceDataNumCones;
		float mVoxelRadiacneDataNumCones_Inverse;
		float mVoxelRadiacneDataRayStepSize;
		BOOL mVoxelRadianceReflectionsEnabled;
		XMFLOAT3 mVoxelRadianceDataCenter;
		BOOL mAdvancedRefractions;
		XMUINT3 mEntityCullingTileCount;
		BOOL mTransparentShadowsEnabled;
		int mGlobaEnvProbeIndex;
		UINT mEnvProbeMipCount;
		float mEnvProbeMipCount_Inverse;
	};
	CBUFFER(FrameCB, CBSLOT_RENDERER_FRAME)
	{
		float mTime;
		float mTimePrev;
		float mDeltaTime;
		UINT mFrameCount;
		UINT mLightArrayOffset;
		UINT mLightArrayCount;
		UINT mDecalArrayOffset;
		UINT mDecalArrayCount;
		UINT mForceFieldArrayOffset;
		UINT mForceFiedlArrayCount;
		UINT mEnvProbeArrayOffet;
		UINT mEnvProbeArrayCount;
		XMFLOAT3 mWindDirection;
		float mWindWaveSize;
		float mWindRandomness;
		int mSunEntityArrayIndex;
		BOOL mVoxelRadianceRetargetted;
		UINT mTemporalAASampleRotation;
		XMFLOAT2 mTemporalAAJitter;
		XMFLOAT2 mTemporalAAJitter;
		XMMATRIX mVP;
		XMMATRIX mView;
		XMMATRIX mProj;
		XMFLOAT3 mCamPos;
		float mCamDistanceFromOrigin;
		XMMATRIX mPrevV;
		XMMATRIX mPrevP;
		XMMATRIX mPrevVP;
		XMMATRIX mPrevInvP;
		XMMATRIX mReflVP;
		XMMATRIX mInvV;
		XMMATRIX mInvP;
		XMMATRIX mInvVP;
		XMFLOAT3 mAt;
		float mZNearP;
		XMFLOAT3 mUp;
		float mZFarP;
		float mZNearP_Recip;
		float mZFarP_Recip;
		float mZRang;
		float mZRange_Recip;
		XMFLOAT4 mFrustumPlanesWS[6];
		XMFLOAT3 mWorldBoundsMin;
		float Poad0;
		XMFLOAT3 mWorldBoundsMax;
		float Pad1;
		XMFLOAT3 mWorldBoundsExtents;
		float pad2;
		XMFLOAT3 mWorldBounsExtents_Inverse;
		float pad3;
	};

	CBUFFER(CameraCB, CBSLOT_RENDERER_CAMERA)
	{
		XMMATRIX mVP;
		XMMATRIX mView;
		XMMATRIX mProj;
		XMMATRIX mCamPos;
		float pad0;
	};

	CBUFFER(MiscCB, CBSLOT_RENDERER_MISC)
	{
		XMMATRIX mTransform;
		XMFLOAT4 mColor;
	};

	CBUFFER(VolumeLightCB, CBSLOT_RENDERER_VOLUMELIGHT)
	{
		XMMATRIX world;
		XMFLOAT4 col;
		XMFLOAT4 enerdis;
	};
	CBUFFER(DecalCB, CBSLOT_RENDERER_DECAL)
	{
		XMMATRIX mDecalVP;
		int hasTexNor;
		XMFLOAT3 eye;
		float opacity;
		XMFLOAT3 front;
	};

	CBUFFER(CubeMapRenderCB, CBSLOT_RENDERER_CUBEMAPRENDER)
	{
		XMMATRIX mViewProjection[6];
	};

	CBUFFER(APICB, CBSLOT_API)
	{
		XMFLOAT4 cilpPlane;
		float alphaRef;
		float pad[3];

		APICB(const XMFLOAT4& IncilpPlane = XMFLOAT4(0, 0, 0, 0), float InalphaRef = 0.75) :cilpPlane(IncilpPlane), alphaRef(InalphaRef) {}

	};


	CBUFFER(TessellationCB, CBSLOT_RENDERER_TESSELLATION)
	{
		XMFLOAT4 tessellationFactors;
	};

	CBUFFER(DispatchParamsCB, CBSLOT_RENDERER_DISPATCHPARAMS)
	{
		UINT numThreadGroups[3];
		UINT value0;
		UINT numThreads[3];
		UINT value1;
	
	};

	protected:
		static bool wireRender;
		static bool debugSphere;
		static bool debugBoneLine;
		static bool debugPartitionTree;
		static bool debugEnvProbes;
		static bool debugEmitters;
		static bool debugForceFields;
		static bool debugCameras;
		static bool gridHelper;
		static bool voxelHelper;
		static bool advanceLightCulling;
		static bool advanceRefractions;
		static bool ldsSkinningEnable;
		static bool requesReflectionRendering;


		static Halo::Texture2D* envirMap;
		static void LoadBuffers();
		static void LoadShaders();
		static void SetupStates();

		static float GameSpeed;

		static Halo::Scene* scene;

		static XMFLOAT4 waterplane;
		static bool debugLightCulling;
		static bool occlusionCulling;
		static bool temporalAA;
		static bool temporalAADEBUG;
		static bool freezeCullingCamera;


		struct VoxelizeSceneData
		{
			bool enabled;
			int res;
			float voxelsize;
			XMFLOAT3 center;
			XMFLOAT3 extents;
			int numCones;
			float rayStepSize;
			bool secondaryBounceEnable;
			bool reflectionEnabel;
			bool centerChangedThisFrame;
			UINT mips;

			VoxelizeSceneData() :enabled(false), res(128), voxelsize(1.0f), center(XMFLOAT3(0, 0, 0)), extents(XMFLOAT3(0, 0, 0)),
				numCones(8), rayStepSize(0.5f), secondaryBounceEnable(true), reflectionEnabel(false), centerChangedThisFrame(true), mips(7)
			{}

		} static voxelizeSceneData;

		static Halo::GPUQuery occlusionQueries[256];

		static UINT entityArrayOffset_Lights;
		static UINT entityArrayCount_Lights;
		static UINT entityArrayOffset_Decals;
		static UINT entityArrayCounr_Decal;
		static UINT entityArrayOffset_ForceField;
		static UINT entityArrayCount_ForceField;
		static UINT entityArrayOffset_EnvProbes;
		static UINT entityArrayCount_EnvProbes;

		public:
			static std::string SHADERPATH;

			static void SetupStaticComponent();
			static void CleanUpStatic();

			static void FixedUpdate();
			static void UpdatePerFrameData(float dt);
			static void UpdateRenderData(GRAPHICSTHREAD threadID);
			static void OcclusionCulling_Render(GRAPHICSTHREAD threadID);
			static void OcclusionCulling_Read();
	     	static void PutDecal(Halo::Decal* decal);
			static void PutWaterRipples(const std::string& image, const XMFLOAT3& pos);
			static void ManageWaterRipples();
			static void DrawWaterRipples(GRAPHICSTHREAD threadID);
			static void SetGameSpeed(float value)
			{
				GameSpeed = value;
				if (GameSpeed<0)
				{
					GameSpeed = 0;
				}
			}
			static float GetGameSpeed();
			static void SetResolutionScale(float value)
			{
				RESOLUTIONSCALE = value;
			}
			static float GetResolutionScale()
			{
				return RESOLUTIONSCALE;
			}
			static void SetTransparentShadowsEnabled(float value)
			{
				TRANSPARENTSHADOWSENABLE = value;
			}
			static float GetTransparentShadowEnabled()
			{
				return TRANSPARENTSHADOWSENABLE;
			}
			static XMUINT2 GetInternalResolution()
			{
				return XMUINT2((UINT)ceilf(GetDevice()->GetScreenWidth()*GetResolutionScale()), (UINT)ceilf(GetDevice()->GetScreenHeight()*GetResolutionScale()));
			   
			}
			static bool ResolutionChanged();
			static void SetGamma(float value)
			{
				GAMMA = value;
			}

			static float GetGamma()
			{
				return GAMMA;
			}

			static void SetWireRender(bool value)
			{
				wireRender = value;
			}

			static bool IsWireRender()
			{
				return wireRender;
			}

			static void ToggleDebugSphere()
			{
				debugSphere = !debugSphere;
			}
			static void SetToDrawDebugSpheres(bool param)
			{
				debugSphere = param;
			}
			static bool GetToDrawDebugSpheres()
			{
				return debugSphere;
			}

			static void SetToDrawDebugBoneline(bool param)
			{
				debugBoneLine = param;
			}

			static bool GetToDrawDebugBoneline()
			{
				return debugBoneLine;
			}
			static void SetToDrawDebugPartitionTree(bool param)
			{
				debugPartitionTree = param;
			}
			static bool GetToDrawDebugPartitionTree()
			{
				return debugPartitionTree;
			}

			static void SetToDrawDebugEnvProbes(bool param)
			{
				debugEnvProbes = param;
			}

			static bool GetToDrawDebugEnvProbes()
			{
				 return debugEnvProbes;
			}

			static void SetToDrawDebugEmitters(bool param)
			{
				debugEmitters = param;
			}

			static bool GetToDrawDebugEmitters()
			{
				return debugEmitters;
			}


			static void SetToDrawDebugForceField(bool param)
			{
				debugForceFields = param;
			}

			static bool GetToDrawDebugForceField()
			{
				return debugForceFields;
			}

			static void SetToDrawGridHelper(bool value)
			{
				gridHelper = value;
			}

			static bool GetToDrawGridHelper()
			{
				return gridHelper;
			}


			static void SetToDrawVoxelHelper(bool value)
			{
				voxelHelper = value;
			}

			static bool GetToDrawVoxelHelper()
			{
				return voxelHelper;
			}

			static void SetDebugLightCulling(bool enabled)
			{
				debugLightCulling = enabled;
			}

			static bool GetDebugLightCulling()
			{
			    return	debugLightCulling;
			}

			static void SetAdvancedLightCulling(bool enabled)
			{
				advanceLightCulling = enabled;
			}

			static bool GetAdvanceLightCulling()
			{
				return advanceLightCulling;
			}

			static void SetAlphaComposittionEnabled(bool enabled)
			{
				ALPHACOMPSITIONENABLE = enabled;
			}

			static bool GetAlphaComposittionEnabled()
			{
				return ALPHACOMPSITIONENABLE;
			}

			static void SetOcclusionCullingEnabled(bool enabled);


			static bool GetOcclusionCullingEnabled()
			{
				return occlusionCulling;
			}

			static void SetLDSSkinnigEnabled(bool enabled)
			{
				ldsSkinningEnable = enabled;
			}

			static bool GetLDSSkinningEnabled()
			{
				return ldsSkinningEnable;
			}

			static void SetTemporalAAEnable(bool enabled)
			{
				temporalAA = enabled;
			}

			static bool GetTemporalAAEnable()
			{
				return temporalAA;
			}

			static void SetTemporalAADebugEnabled(bool enabled)
			{
				temporalAADEBUG = enabled;
			}

			static bool GetTemporalAADebugEnabled()
			{
				return temporalAADEBUG;
			}

			static void SetFreezeCullingCamearaEnabled(bool enabled)
			{
				freezeCullingCamera = enabled;
			}

			static bool GetFreezeCullingCameraEnable()
			{
				return freezeCullingCamera;
			}

			static void SetVoxelRadianceEnable(bool enabled)
			{
				voxelizeSceneData.enabled = enabled;
			}

			static bool GetVoxelRadianceEnable()
			{
				return voxelizeSceneData.enabled;
			}

			static void SetVoxelRadianceSecondatyBounceEnable(bool enabled)
			{
				voxelizeSceneData.secondaryBounceEnable = enabled;
			}

			static bool GetVoxelRadianceSecondatyBounceEnable()
			{
				return voxelizeSceneData.secondaryBounceEnable;
			}

			static void SetVoxelRadianceReflectionsEnabled(bool enabled)
			{
				voxelizeSceneData.reflectionEnabel = enabled;
			}

			static bool GetVoxelRadianceReflectionsEnabled()
			{
				return voxelizeSceneData.reflectionEnabel;
			}

			static void SetVoxelRadianceVoxelSize(float value)
			{
				voxelizeSceneData.voxelsize = value;
			}

			static float GetVoxelRadianceVoxelSize()
			{
				 return voxelizeSceneData.voxelsize;
			}

			static void SetVoxelRadianceResolution(int value)
			{
				voxelizeSceneData.res = value;
			}

			static int GetVoxelRadianceResolution()
			{
			    return voxelizeSceneData.res;
			}

			static void SetVoxelRadianceNumCones(int value)
			{
				voxelizeSceneData.numCones = value;
			}

			static int GetVoxelRadianceNumCones()
			{
				return voxelizeSceneData.numCones;
			}

			static void SetVoxelRadianceRayStepSize(float value)
			{
				voxelizeSceneData.rayStepSize = value;
			}

			static float GetVoxelRadianceRayStepSize()
			{
				return voxelizeSceneData.rayStepSize;
			}

			static void SetAdvancedRefractionsEnabled(bool value)
			{
				advanceRefractions = value;
			}
				
			static bool GetAdvancedRefractionsEnabled()
			{
			//	return advanceRefractions;
				//NEED ADDITIONAL DRIVER SUPPORT
			}

			static bool IsRequestedReflectionEnabled()
			{
				return requesReflectionRendering;
			}

			static void SetEniromentMap(Halo::Texture2D* tex)
			{
				envirMap = tex;
			}

			static Halo::Texture2D* GetEnivromentMap()
			{
				return envirMap;
			}

			static Halo::Texture2D* GetLuminace(Halo::Texture2D* sourceIamge, GRAPHICSTHREAD threadID);
			static const XMFLOAT4& GetWaterPlane();

			static Halo::Transform* GetTransformByName(const std::string& name);
			static Halo::Transform* GetTransformByID(uint64_t id);
			static Halo::Armature* getArmatureByName(const std::string& get);
			static int GetActionByName(Halo::Armature* armature, const std::string& get);
			static int getBoneByName(Halo::Armature* armature, const std::string& get);
			static Halo::Material* GetMaterialByName(const std::string& get);
			static Halo::Object* getObjectByName(const std::string& name);
			static Halo::Camera* getCameraByName(const std::string& name);
			static Halo::Light* getLightByName(const std::string& name);

			static void ReloadShaders(const std::string& path = "");
			static void BindPersistenState(GRAPHICSTHREAD theadID);

			struct FrameCulling
			{
				Frustum frustum;
				CulledCollection culledRenderer;
				CulledCollection culledRenderer_opaque;
				CulledCollection culledRenderer_transparent;
				std::vector<HairParticle*> culledHairParticleSystem;
				Culledlist culledLighs;
				std::list<Halo::Decal*> culledDecals;
				std::list<Halo::EnvironmentProbe*> culledEnvProbes;


				void Clear()
			   {
					culledRenderer.clear();
					culledRenderer_opaque.clear();
					culledRenderer_transparent.clear();
					culledHairParticleSystem.clear();
					culledLighs.clear();
					culledDecals.clear();
					culledEnvProbes.clear();
				
				
				}
			};


			static std::unordered_map<Halo::Camera*, FrameCulling> frameCulling;

			inline static XMUINT3 GetEntityCullingTileCount()
			{
				return XMUINT3(
					(UINT)ceilf((float)GetInternalResolution().x / (float)TILED_CULLING_BLOCKSIZE),
					(UINT)ceilf((float)GetInternalResolution().y / (float)TILED_CULLING_BLOCKSIZE),
					1);
			}
public:
	static void UpdateWorldCB(GRAPHICSTHREAD threadID);
	static void UpdateFrameCB(GRAPHICSTHREAD threadID);
	static void UpdateCameraCB(Halo::Camera* camera, GRAPHICSTHREAD threadID);
	static void SetCilpPlane(const XMFLOAT4& cilpPlane, GRAPHICSTHREAD threadID);
	static void SetAlphaRef(float alphaRef, GRAPHICSTHREAD threadID);
	static void BindGBufferTextures(Halo::Texture2D* slot0, Halo::Texture2D* slot1, Halo::Texture2D* slot2,
		Halo::Texture2D* slot3, Halo::Texture2D* slot4, GRAPHICSTHREAD threadID);
	static void BindDepthTexture(Halo::Texture2D* depth, Halo::Texture2D* linearDepth, GRAPHICSTHREAD threadID);

	static void RenderMeshes(const XMFLOAT3& eye, const CulledCollection& culledRenderer, SHADERTYPE shadetype,
		UINT renderTypeFlags, GRAPHICSTHREAD threadID,bool tessllation=false,bool occlusionCulling=false,uint32_t layerMask=0xFFFFFFFF);

	static void DrawSky(GRAPHICSTHREAD threadID);
	static void DrawSun(GRAPHICSTHREAD threadID);
	static void DrawWorld(Halo::Camera* camera, bool tessellation, GRAPHICSTHREAD threadID, SHADERTYPE shadertype, bool grass, bool,
		bool occlusionCulling, uint32_t layerMask = 0xFFFFFFF);
	static void DrawForShadowMap(GRAPHICSTHREAD threadID, uint32_t layermask = 0xFFFFFFFF);
	static void DrawWorldTransparent(Halo::Camera* camera, SHADERTYPE shaderType, GRAPHICSTHREAD threadID, bool grass,
		bool occlusionCulling, uint32_t layerMask = 0xFFFFFFFF);

	static void DrawDebugWorld(Halo::Camera* camera, GRAPHICSTHREAD threadID);
	static void DrawSoftParticles(Halo::Camera*camera, bool distortion, GRAPHICSTHREAD threadID);
	static void DrawTrails(GRAPHICSTHREAD threadID, Halo::Texture2D* refacRes);
	static void DrawLights(Halo::Camera* camera, GRAPHICSTHREAD threadID);
	static void DrawLightVisualizers(Halo::Camera* camera, GRAPHICSTHREAD threadID);
	static void DrawLensFlares(GRAPHICSTHREAD threadID);
	static void RefresEnvProbes(GRAPHICSTHREAD threadID);
	static void VoxelRadiance(GRAPHICSTHREAD threadID);

	static void ComputeTiledLightCulling(bool deferred, GRAPHICSTHREAD threadID);
	static void ResolveMSAADepthBuffer(Halo::Texture2D* dst, Halo::Texture2D* src, GRAPHICSTHREAD threadID);

	static void BuildSceneBVH(GRAPHICSTHREAD threadID);;
	static void DrawTracedScene(Halo::Camera* camera, Halo::Texture2D* result, GRAPHICSTHREAD threadID);

	enum MIPGENFILTER
	{
         MIPGENFILTER_POINT,
		 MIPGENFILTER_LINEAR,
		 MIPGENFILTER_LINEAR_MAXIMUN,
		 MIPGENFILTER_GAUSSIAN,
	};

	static void GenerateMipChain(Halo::Texture1D* texture, MIPGENFILTER filter, GRAPHICSTHREAD threadID, int arrayIndex = -1);
	static void GenerateMipChain(Halo::Texture2D* texture, MIPGENFILTER filter, GRAPHICSTHREAD threadID, int arrayIndex = -1);
	static void GenerateMipChain(Halo::Texture3D* texture, MIPGENFILTER filter, GRAPHICSTHREAD threadID, int arrayIndex = -1);


	enum BORDERXPANDSTYLE
	{
		BORDERXPAND_DISABLE,
		BORDERXPAND_WRAP,
		BORAERXPAND_CLMAP,
	};


	static void CopyTexture2D(Halo::Texture2D* dst, UINT DstMIP, UINT DstX, UINT DstY, Halo::Texture2D* src,
		UINT SrcMip, GRAPHICSTHREAD threadID,
		BORDERXPANDSTYLE borderexpan = BORDERXPAND_DISABLE);

	static void GenrateClouds(Halo::Texture2D* dst, UINT refinementCount, float randomness, GRAPHICSTHREAD threadID);

	static void ManageDecalAltas(GRAPHICSTHREAD threadID);

	static XMVECTOR GetSunPosition();
	static XMFLOAT4 GetSunColor();
	static int GetSunAarryIndex();


	static SPTree* sptree;
	static SPTree* sptree_light;



	static Halo::Scene& GetScene();

	static std::unordered_set<Halo::Object*> objectsWithTrails;
	static std::unordered_set<EmittedParticle*> emitterSystem;

	static std::deque<Sprite*> waterRipples;
	static void ClearWorld();

	static Halo::Camera*cam;
	static Halo::Camera* refCam;
	static Halo::Camera* prevFrameCam;

	static Halo::Camera* getCamera()
	{
		return cam;
	}

	static Halo::Camera* GetRefCamera()
	{
		return refCam;
	}

	static RAY getPickRay(long cursorX, long cursorY);

	struct RayIntersectWorldResult
	{
		Halo::Object* object = nullptr;
		XMFLOAT3 position = XMFLOAT3(0, 0, 0);
		XMFLOAT3 normal = XMFLOAT3(0, 0, 0);
		float distance = FLT_MAX;
		int subsetIndex = -1;
	};

	static RayIntersectWorldResult RayIntersectWorld(const RAY&ray,UINT renderTypeMask=RENDERTYPE_OPAQUE,uint32_t layerMask=0xFFFFFFF,
		bool dynamicObjects=true,bool onlyVisible=false);

	static PHYSICS* physicsEngine;
	static void SynchronizeWithPhysicsEngine(float dt = 1.0f / 60.f);
	static Ocean* ocean;

	static void SetOceanEnabled(bool enabled, const OceanParameter& params);

	static Ocean* GetOcean()
	{
		return ocean;
	}

	static Halo::Model* LoadModel(const std::string& filename, const XMMATRIX& transform = XMMatrixIdentity());
	static void LoadWorldInfo(const std::string& filename);
	static void LoadDefaultLighting();
	static void PutEnvProbe(const XMFLOAT3& position);

	static void CreateImpostor(Halo::Mesh* mesh, GRAPHICSTHREAD threadID);

	static std::vector<std::pair<XMFLOAT4X4, XMFLOAT4>> renderableBoxes;

	static void AddRenderableBox(const XMFLOAT4& boxMatrix, const XMFLOAT4& color = XMFLOAT4(1, 1, 1, 1));

	struct RenderableLine
	{
		XMFLOAT3 start = XMFLOAT3(0, 0, 0);
		XMFLOAT3 end = XMFLOAT3(0, 0, 0);
		XMFLOAT4 color = XMFLOAT4(1, 1, 1, 1);
	};


	static std::vector<RenderableLine> renderableLines;
	static void AddRenderableLine(const RenderableLine & line);
	static void AddDeferredMIPGen(Halo::Texture2D* tex);


	static void AddModel(Halo::Model* value);
	static void Add(Halo::Object* value);
	static void Add(Halo::Light* value);
	static void Add(Halo::ForceField* value);
	static void Add(Halo::Camera* value);

	static void Remove(Halo::Object* value);
	static void Remove(Halo::Light* value);
	static void Remove(Halo::Decal* value);
	static void Remove(Halo::EnvironmentProbe* value);
	static void Remove(Halo::ForceField* value);
	static void Remove(Halo::Camera* value);









};


*/
