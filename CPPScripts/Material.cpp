#include "Material.h"
#include "ZShader.h"
#include "Texture.h"
#include "RenderAPI.h"
#include "GlobalData.h"
#include "MaterialData.h"
#include "RenderEngineProperties.h"

namespace ZXEngine
{
	Material::Material(MaterialStruct* matStruct)
	{
		name = matStruct->name;
		path = matStruct->path;
		isShareShader = false;
		shader = new Shader(matStruct->shaderPath, FrameBufferType::Normal);
		data = new MaterialData();

		for (auto textureStruct : matStruct->textures)
		{
			Texture* texture = new Texture(textureStruct->path.c_str());
			data->textures.push_back(make_pair(textureStruct->uniformName, texture));
		}

		RenderAPI::GetInstance()->SetUpMaterial(shader->reference, data);
	}

	Material::Material(Shader* shader)
	{
		// 这里要注意一个问题，如果调用这个构造函数时，是直接把new Shader写在参数里的，如:
		// new Material(new Shader(...))
		// 那么会有一个问题，new的时候没有保存引用，最终会少一个delete，导致referenceCount至少大于1，这个shader永远不会被正真销毁
		isShareShader = true;
		shader->reference->referenceCount++;
		this->shader = shader;
		data = new MaterialData();
		RenderAPI::GetInstance()->SetUpMaterial(this->shader->reference, data);
	}

	Material::~Material()
	{
		delete data;

		if (isShareShader)
			shader->reference->referenceCount--;
		else
			delete shader;
	}

	void Material::Use()
	{
		data->Use();
		shader->Use();
	}

	int Material::GetRenderQueue()
	{
		return shader->GetRenderQueue();
	}

	void Material::SetEngineProperties()
	{
		for (auto& property : shader->reference->shaderInfo.vertProperties.baseProperties)
			SetEngineProperty(property.name, property.type);
		for (auto& property : shader->reference->shaderInfo.vertProperties.textureProperties)
			SetEngineProperty(property.name, property.type);
		for (auto& property : shader->reference->shaderInfo.fragProperties.baseProperties)
			SetEngineProperty(property.name, property.type);
		for (auto& property : shader->reference->shaderInfo.fragProperties.textureProperties)
			SetEngineProperty(property.name, property.type);
	}

	void Material::SetEngineProperty(const string& name, ShaderPropertyType type)
	{
		auto engineProperties = RenderEngineProperties::GetInstance();

		if (type == ShaderPropertyType::ENGINE_MODEL)
			SetMatrix(name, engineProperties->matM);
		else if (type == ShaderPropertyType::ENGINE_VIEW)
			SetMatrix(name, engineProperties->matV);
		else if (type == ShaderPropertyType::ENGINE_PROJECTION)
			SetMatrix(name, engineProperties->matP);
		else if (type == ShaderPropertyType::ENGINE_CAMERA_POS)
			SetVector(name, engineProperties->camPos);
		else if (type == ShaderPropertyType::ENGINE_LIGHT_POS)
			SetVector(name, engineProperties->lightPos);
		else if (type == ShaderPropertyType::ENGINE_LIGHT_DIR)
			SetVector(name, engineProperties->lightDir);
		else if (type == ShaderPropertyType::ENGINE_LIGHT_COLOR)
			SetVector(name, engineProperties->lightColor);
		else if (type == ShaderPropertyType::ENGINE_LIGHT_INTENSITY)
			SetScalar(name, engineProperties->lightIntensity);
		else if (type == ShaderPropertyType::ENGINE_FAR_PLANE)
			SetScalar(name, GlobalData::shadowCubeMapFarPlane);
		else if (type == ShaderPropertyType::ENGINE_DEPTH_CUBE_MAP)
		{
			// 先设置SetMaterialProperties获得引擎纹理的初始textureIdx，然后++
			SetCubeMap(name, engineProperties->shadowCubeMap, textureIdx, true);
			textureIdx++;
		}
	}

	void Material::SetMaterialProperties()
	{
		uint32_t textureNum = (uint32_t)data->textures.size();
		for (uint32_t i = 0; i < textureNum; i++)
			SetTexture(data->textures[i].first, data->textures[i].second->GetID(), i);

		textureIdx = textureNum;
	}

	void Material::SetMaterialProperty(const string& name, ShaderPropertyType type)
	{

	}

	void Material::SetScalar(string name, bool value, bool allBuffer)
	{
		RenderAPI::GetInstance()->SetShaderScalar(this, name, value, allBuffer);
	}
	void Material::SetScalar(string name, int value, bool allBuffer)
	{
		RenderAPI::GetInstance()->SetShaderScalar(this, name, value, allBuffer);
	}
	void Material::SetScalar(string name, float value, bool allBuffer)
	{
		RenderAPI::GetInstance()->SetShaderScalar(this, name, value, allBuffer);
	}
	void Material::SetVector(string name, Vector2 value, bool allBuffer)
	{
		RenderAPI::GetInstance()->SetShaderVector(this, name, value, allBuffer);
	}
	void Material::SetVector(string name, Vector2 value, uint32_t idx, bool allBuffer)
	{
		RenderAPI::GetInstance()->SetShaderVector(this, name, value, idx, allBuffer);
	}
	void Material::SetVector(string name, Vector3 value, bool allBuffer)
	{
		RenderAPI::GetInstance()->SetShaderVector(this, name, value, allBuffer);
	}
	void Material::SetVector(string name, Vector3 value, uint32_t idx, bool allBuffer)
	{
		RenderAPI::GetInstance()->SetShaderVector(this, name, value, idx, allBuffer);
	}
	void Material::SetVector(string name, Vector4 value, bool allBuffer)
	{
		RenderAPI::GetInstance()->SetShaderVector(this, name, value, allBuffer);
	}
	void Material::SetVector(string name, Vector4 value, uint32_t idx, bool allBuffer)
	{
		RenderAPI::GetInstance()->SetShaderVector(this, name, value, idx, allBuffer);
	}
	void Material::SetMatrix(string name, Matrix3 value, bool allBuffer)
	{
		RenderAPI::GetInstance()->SetShaderMatrix(this, name, value, allBuffer);
	}
	void Material::SetMatrix(string name, Matrix3 value, uint32_t idx, bool allBuffer)
	{
		RenderAPI::GetInstance()->SetShaderMatrix(this, name, value, idx, allBuffer);
	}
	void Material::SetMatrix(string name, Matrix4 value, bool allBuffer)
	{
		RenderAPI::GetInstance()->SetShaderMatrix(this, name, value, allBuffer);
	}
	void Material::SetMatrix(string name, Matrix4 value, uint32_t idx, bool allBuffer)
	{
		RenderAPI::GetInstance()->SetShaderMatrix(this, name, value, idx, allBuffer);
	}
	void Material::SetTexture(string name, uint32_t ID, uint32_t idx, bool isBuffer)
	{
		RenderAPI::GetInstance()->SetShaderTexture(this, name, ID, idx, isBuffer);
	}
	void Material::SetCubeMap(string name, uint32_t ID, uint32_t idx, bool isBuffer)
	{
		RenderAPI::GetInstance()->SetShaderCubeMap(this, name, ID, idx, isBuffer);
	}
}