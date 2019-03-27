#pragma once
#include <cassert>
#include <cstdint>





namespace PRE
{
	enum class ViewLayerType
	{
		e3D = 0,
		eSkybox,
		eHighest = eSkybox + 1
	};

	enum class TranslucencyType
	{
		eSubtractive = 0,
	    eAdditive,
        eNormal,
		eOpaque
	};

	struct MaterialId
	{
		inline MaterialId(uint32_t id, uint32_t pass) :
			id(id)
			, pass(pass)
		{
		}

		uint64_t id;
		uint64_t pass;
	};

#pragma pack(push,1)

	struct DrawKey
	{
		static const uint64_t kOpaqueModeMask = 0x1F80000000000000;
		static const uint64_t kOpaqueModeValue= 0x0B00000000000000;
		static const uint32_t kPriorityBits = 7;


		struct Transparent
		{
			uint64_t matrtialId : 31;
			uint64_t depth : 24;
			uint64_t cmd : 1;

			uint64_t :3+3+2;

		};

		struct Opaque
		{
			uint64_t depth : 24;
			uint64_t materialId : 31;
			uint64_t cmd : 1;


		    uint64_t:3 + 2 + 2;
		};

		struct Custom
		{
			uint64_t dummy : 48;
			uint64_t priority : kPriorityBits;
			uint64_t enabled : 1;

		    uint64_t:3 + 3 + 2;
		};

		union {
			struct
			{
			uint64_t:64 - (3 + 3 + 2);

				uint64_t translucency : 2;
				uint64_t viewLayer : 3;
				uint64_t viewportId : 3;
			};

			Transparent transparent;

			Opaque opaque;
			Custom custom;
			uint64_t value;
		};
		
		DrawKey();
		explicit DrawKey(uint64_t value);
		explicit DrawKey(PRE::ViewLayerType viewlayer);
		DrawKey(PRE::ViewLayerType viewLayer, PRE::TranslucencyType translucency);

		void setView(uint32_t viewportId, PRE::ViewLayerType viewLayer, PRE::TranslucencyType translucency);
		void setViewLayer(PRE::ViewLayerType viewlayer, PRE::TranslucencyType translucency);
		void SetTranslucency(PRE::TranslucencyType  translucency);
		void setMaterial(uint32_t materialId);
		void SetDepth(uint32_t depth);
		void SetMaterialDepth(uint32_t materialId, uint32_t depth);


		PRE::MaterialId material() const;
		bool isOpaqueMode() const;

		bool operator <(DrawKey other) const;
		
		explicit operator uint64_t();

		static DrawKey makedefault(uint32_t viewportId, PRE::ViewLayerType viewLayer = ViewLayerType::e3D);
		static DrawKey makeCustom(PRE::ViewLayerType viewLayer, uint32_t priority);
		static DrawKey GenerateKey(uint64_t viewportId, ViewLayerType viewLayer, uint64_t priority, uint64_t materialID, uint64_t depth,TranslucencyType translucency,bool CustomMode);
		static void sanityChecks();
	};
#pragma  pack(pop)

	inline DrawKey::DrawKey()
	{
		static_assert(sizeof(DrawKey) == sizeof(uint64_t), "INVALID_DRAW_KEY_SIZE");
	}

	inline DrawKey::DrawKey(uint64_t value):value(value)
	{
		this->viewLayer = static_cast<uint32_t>(viewLayer);
	}

	inline DrawKey::DrawKey(PRE::ViewLayerType viewLayer):value(0)
	{
		this->viewLayer = static_cast<uint32_t>(viewLayer);
	}

	inline DrawKey::DrawKey(PRE::ViewLayerType viewLayer, PRE::TranslucencyType translucency)
	{
		setViewLayer(viewLayer, translucency);
	}

	inline void DrawKey::setView(uint32_t viewportId, ViewLayerType viewLayer, PRE::TranslucencyType translucency)
	{
		this->viewportId = viewportId;
		this->viewLayer = static_cast<uint32_t>(viewLayer);
		this->translucency = static_cast<uint32_t>(translucency);
	}

	inline void DrawKey::setViewLayer(ViewLayerType viewLayer,PRE::TranslucencyType translucency)
	{
		this->viewLayer = static_cast<uint32_t>(viewLayer);
		this->translucency = static_cast<uint32_t>(translucency);
	}

	inline void DrawKey::setMaterial(uint32_t materialId)
	{
		 if (isOpaqueMode())
		 {
			 opaque.materialId = materialId;
		 }
		 else
		 {
			
			 transparent.matrtialId = materialId;
		 }
	}

	inline void DrawKey::SetDepth(uint32_t depth)
	{
	    if (isOpaqueMode())
	    {
			opaque.depth = depth;
	    }
		else
		{
			transparent.depth = depth;
		}
	}

	inline void DrawKey::SetMaterialDepth(uint32_t materialId, uint32_t depth)
	{
		if (isOpaqueMode())
		{
			opaque.depth = depth;
			opaque.materialId = materialId;
		}
		else
		{
			transparent.depth = depth;
			transparent.matrtialId = materialId;
		}
	}

	inline void DrawKey::SetTranslucency(TranslucencyType translucency)
	{
		this->translucency = static_cast<uint32_t>(translucency);
	}

	inline PRE::MaterialId DrawKey::material() const 
	{
		MaterialId NullID = {0u,0u};
		if (custom.enabled)
		{
			if (isOpaqueMode())
			{
				return PRE::MaterialId(opaque.materialId, 0);
			}
			else
			{
				return PRE::MaterialId(transparent.matrtialId, 0);
			}
		}
		return NullID;
	}

	inline bool DrawKey::isOpaqueMode() const
	{
		/*uint64_t keyvalue = reinterpret_cast<const uint64_t&>(*this);
		return (keyvalue&kOpaqueModeMask) == kOpaqueModeValue;*/
		return this->translucency == 3;
	}

	inline DrawKey::operator uint64_t()
	{
		return value;
	}

	inline bool DrawKey::operator<(DrawKey other) const
	{
	    return value>other.value;
	}

	inline DrawKey DrawKey::makedefault(uint32_t viewportId, PRE::ViewLayerType viewLayer)
	{
		DrawKey key(0);
		key.viewportId = viewportId;
		key.viewLayer = static_cast<uint32_t>(viewLayer);
		return key;
	}

	inline DrawKey DrawKey::makeCustom(PRE::ViewLayerType viewLayer, uint32_t priority)
	{
		DrawKey key(viewLayer);
		key.custom.enabled = true;
		key.custom.priority = (2 << kPriorityBits) - 1 - priority;
		return key;
	}

	inline DrawKey DrawKey::GenerateKey(
		uint64_t viewportId,
		ViewLayerType viewLayer, 
		uint64_t priority,
		uint64_t materialID,
		uint64_t depth,
		TranslucencyType translucency,
		bool CustomMode)
	{
		DrawKey key;
		if (CustomMode)
		{
			key=makeCustom(viewLayer, priority);
		}
		else
		{
			key = makedefault(viewportId, viewLayer);
			key.SetTranslucency(translucency);
			key.SetMaterialDepth(materialID, depth);
		}
		
		return key;
	}

	inline void DrawKey::sanityChecks()
	{
		/*assert(PRE::isLittleEndian());

		DrawKey drawkey;
		drawkey.value = 0;
		drawkey.viewportId = 3;
		drawkey.viewLayer = 0x2;
		drawkey.translucency = (uint32_t)TranslucencyType::eNormal;
		drawkey.custom.enabled = 0;
		drawkey.transparent.depth = 0x1234;
		drawkey.transparent.matrtialId = 0x5678;
		drawkey.transparent.materialPass = 4;
		assert(drawkey.value = 0x6a00091A00567804);

		drawkey.opaque.depth = 0x1234;
		drawkey.opaque.materialId = 0x5678;
		drawkey.opaque.materialPass = 4;
		assert(drawkey.value == 0x6a00567804001234);

		drawkey.custom.enabled = 1;
		drawkey.custom.priority = 9;
		drawkey.custom.dummy = 0;
		assert(drawkey.value == 0x6a89000000000000);

		drawkey.value = 0;
		drawkey.translucency = 0x3;
		drawkey.viewLayer = 0x7;
		drawkey.custom.enabled = 1;
		assert(drawkey.value == DrawKey::kOpaqueModeMask);
		drawkey.custom.enabled = 0;
		drawkey.translucency = (uint32_t)TranslucencyType::eOpaque;
		drawkey.viewLayer = 0x2;
		assert(drawkey.value == DrawKey::kOpaqueModeValue);*/

	}

	inline const char* toString(PRE::ViewLayerType type)
	{
		switch (type)
		{
		case PRE::ViewLayerType::e3D:
			return "3D";
			break;
		case PRE::ViewLayerType::eSkybox:
			return "Skybox";
			break;
		case PRE::ViewLayerType::eHighest:
			return "Highest";
			break;
		default:
			assert(false);
			return "Unkonw";
		}
	}

	inline const char* toString(PRE::TranslucencyType type)
	{
		switch (type)
		{
		case PRE::TranslucencyType::eSubtractive:
			return "Subtractive";
			break;
		case PRE::TranslucencyType::eAdditive:
			return "Additive";
			break;
		case PRE::TranslucencyType::eNormal:
			return "Normal";
			break;
		case PRE::TranslucencyType::eOpaque:
			return "Opaque";
			break;
		default:
			assert(false);
			return "Unkonwn";
		}
	}

	inline std::ostream& operator<<(std::ostream& stream, PRE::DrawKey key)
	{
	/*	stream << key.viewportId << ",layer:" << PRE::toString(PRE::ViewLayerType(key.viewLayer)) << "-"
			<< PRE::toString(PRE::TranslucencyType(key.translucency)) << ",";

		if (key.custom.enabled)
		{
			stream << "custom command ,priority:" << key.custom.priority;
		}
		else if (PRE::TranslucencyType(key.translucency)!=PRE::TranslucencyType::eOpaque)
		{
			stream << ",depth:" << key.transparent.depth << ",material id:" << key.transparent.matrtialId;
		}
		else if (key.isOpaqueMode())
		{
			stream << ", depth:" << key.opaque.depth << ", material id:" << key.opaque.materialId;
		}*/
		return stream;
	}
}