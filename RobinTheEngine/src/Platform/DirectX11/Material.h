#pragma once
#include <DirectXMath.h>
#include <memory>
#include "Platform/DirectX11/Shaders.h"
#include "Platform/DirectX11/ConstantBuffer.h"
#include "Platform/DirectX11/Texture.h"
#include "RobinTheEngine/Scene/BaseResource.h"


namespace RTE {



	enum  class MaterialType
	{
		None,
		ColoredMaterial,
		TexturedMaterial,
		TexturedExtendedMaterial

	};

	struct MaterialDescriptor
	{
		std::string key;
		int MaterialType;
		float color[4];
		std::string vsKey;
		std::string psKey;
		std::string vsPath;
		std::string psPath;
		std::string textureKey;

		XMFLOAT4  AmbientColor;
		XMFLOAT4  EmissiveColor;
		XMFLOAT4  DiffuseColor;
		XMFLOAT4  SpecularColor;
		XMFLOAT4  Reflectance;
		float   Opacity;
		float   SpecularPower;
		float   IndexOfRefraction;

		std::string AmbientTextureKey;
		std::string EmissiveTextureKey;
		std::string DiffuseTextureKey;
		std::string SpecularTextureKey;
		std::string SpecularPowerTextureKey;
		std::string NormalTextureKey;
		std::string BumpTextureKey;
		std::string OpacityTextureKey;

		float   BumpIntensity;
		float   SpecularScale;
		float   AlphaThreshold;


		MaterialDescriptor()
			: AmbientColor(0, 0, 0, 1)
			, EmissiveColor(0, 0, 0, 1)
			, DiffuseColor(1, 1, 1, 1)
			, SpecularColor(0, 0, 0, 1)
			, Reflectance(0, 0, 0, 0)
			, Opacity(1.0f)
			, SpecularPower(-1.0f)
			, IndexOfRefraction(-1.0f)
			, BumpIntensity(5.0f)
			, SpecularScale(128.0f)
			, AlphaThreshold(0.1f)
		{}

	};

	//class ColoredMaterialBase;
	class MaterialBase {

	public:

		virtual void ApplyMaterial() = 0;
		virtual MaterialType GetMaterialType() = 0;
		virtual ~MaterialBase() {};
		virtual void DrawImGui() {};

	};

	//Default material for all objects without materials.
	//We just draw objects with collor.
	class ColoredMaterialBase : public MaterialBase
	{

	public:
		ColoredMaterialBase(MaterialDescriptor desc) {

			std::string vsPath = desc.vsPath;
			const D3D11_INPUT_ELEMENT_DESC layout[] =
			{
				D3D11_INPUT_ELEMENT_DESC {"POSITION", 0,DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,0 ,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0},
				D3D11_INPUT_ELEMENT_DESC {"NORMAL", 0,DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA, 0}

			};
			UINT numElements = ARRAYSIZE(layout);
			vs.init(vsPath, layout, numElements);
			std::wstring wstr(desc.psPath.begin(), desc.psPath.end());
			ps.init(wstr);

			color.InitializeUniqueBuffer();
			color.data.vec = XMFLOAT4(desc.color[0], desc.color[1], desc.color[2], desc.color[3]);
			color.WriteBuffer();

		}

		void SetColor(DirectX::XMFLOAT4 colorVec);

		// Inherited via Material
		virtual void ApplyMaterial() override;
		~ColoredMaterialBase()override {};


		MaterialType GetMaterialType() override;
	private:
		vertexShader vs;
		pixelShader ps;
		ConstantBuffer<CB_VS_VEC4F> color;

	};


	//TODO: create texture sampler class and add it to that material base
	class TexturedMaterialBase : public MaterialBase
	{

	public:
		TexturedMaterialBase(MaterialDescriptor desc);
		void SetTexture(std::string textureKey);

		// Inherited via Material
		virtual void ApplyMaterial() override;
		//TODO: just empty destructor?
		~TexturedMaterialBase()override {};


		MaterialType GetMaterialType() override;

	private:
		vertexShader vs;
		pixelShader ps;
		Texture diffuseTexture;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> repeatSampler;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> clampSampler;

	};

	//TODO: create texture sampler class and add it to that material base
	class TexturedExtentedMaterialBase : public MaterialBase
	{

	public:
		TexturedExtentedMaterialBase(MaterialDescriptor desc);

		// Inherited via Material
		virtual void ApplyMaterial() override;
		//TODO: just empty destructor?
		~TexturedExtentedMaterialBase()override {};


		MaterialType GetMaterialType() override;
		void DrawImGui() override;

	private:

		void SetTextureByKey(Texture* txt, std::string* key, std::string* localKey);
		bool isKeyNotNull(std::string* key);
		vertexShader vs;
		pixelShader ps;

		Texture ambientTexture;
		Texture emissiveTexture;
		Texture diffuseTexture;
		Texture specularTexture;
		Texture specularPowerTexture;
		Texture normalTexture;
		Texture bumpTexture;
		Texture opacityTexture;

		std::string AmbientTextureKey;
		std::string EmissiveTextureKey;
		std::string DiffuseTextureKey;
		std::string SpecularTextureKey;
		std::string SpecularPowerTextureKey;
		std::string NormalTextureKey;
		std::string BumpTextureKey;
		std::string OpacityTextureKey;

		ConstantBuffer<CB_PS_MATERIAL> cb;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> repeatSampler;
		Microsoft::WRL::ComPtr<ID3D11SamplerState> clampSampler;
	};






	class Material : public BaseResource {

	public:

		Material() : BaseResource("invalid") {}
		Material(std::string id) : BaseResource(id) {}

		Material(MaterialDescriptor desc) : BaseResource(desc.key) {
			switch (desc.MaterialType)
			{
			case 1:
				matPtr = std::shared_ptr<ColoredMaterialBase>(new ColoredMaterialBase(desc));
				break;
			case 2:
				matPtr = std::shared_ptr<TexturedMaterialBase>(new TexturedMaterialBase(desc));
				break;
			case 3:
				matPtr = std::shared_ptr<TexturedExtentedMaterialBase>(new TexturedExtentedMaterialBase(desc));
				break;

			default:
				RTE_CORE_ASSERT(false, "Bad material type with value:{0}", desc.MaterialType);
				break;
			}
		}
		std::shared_ptr<MaterialBase> matPtr;

	};











}
