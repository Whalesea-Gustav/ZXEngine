#include "ZShader.h"
#include "RenderAPI.h"
#include "Resources.h"
#include "GlobalData.h"

namespace ZXEngine
{
	vector<ShaderReference*> Shader::loadedShaders;

	// 第二个参数是因为Vulkan创建Pipeline的时候需要设置合适的RenderPass，如果之后Vulkan改成了Dynamic Rendering，这个参数就可以去掉了
	Shader::Shader(const string& path, FrameBufferType type)
	{
		name = Resources::GetAssetName(path);
		renderQueue = (int)RenderQueueType::Qpaque;

		for (auto shaderReference : loadedShaders)
		{
			if (path == shaderReference->path)
			{
				// 如果已加载过，直接引用
				reference = shaderReference;
				// 引用计数+1
				reference->referenceCount++;
				break;
			}
		}
		// 如果没有加载过，执行真正的加载和编译
		if (reference == nullptr)
		{
			reference = RenderAPI::GetInstance()->LoadAndSetUpShader(path.c_str(), type);
			reference->path = path;
			loadedShaders.push_back(reference);
		}
	}

	Shader::~Shader()
	{
		reference->referenceCount--;
		// 引用计数归零后执行真正的删除操作
		if (reference->referenceCount == 0)
		{
			size_t pos = -1;
			for (size_t i = 0; i < loadedShaders.size(); i++)
			{
				if (loadedShaders[i]->ID == reference->ID)
				{
					pos = i;
					break;
				}
			}
			if (pos != -1)
			{
				// 执行清理操作
				loadedShaders.erase(loadedShaders.begin() + pos);
				RenderAPI::GetInstance()->DeleteShader(reference->ID);
				delete reference;
			}
			else
			{
				Debug::LogWarning("Free shader failed: " + reference->path);
			}
		}
	}

	unsigned int Shader::GetID()
	{
		return reference->ID;
	}

	LightType Shader::GetLightType()
	{
		return reference->shaderInfo.lightType;
	}

	ShadowType Shader::GetShadowType()
	{
		return reference->shaderInfo.shadowType;
	}

	int Shader::GetRenderQueue()
	{
		return renderQueue;
	}

	void Shader::Use()
	{
		RenderAPI::GetInstance()->UseShader(reference->ID);
	}
}