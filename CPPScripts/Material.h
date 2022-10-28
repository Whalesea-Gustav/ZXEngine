#pragma once
#include "ZShader.h"
#include "Texture.h"
#include "Resources.h"

namespace ZXEngine
{
	class Material
	{
	public:
		Shader* shader = nullptr;
		vector<Texture*> textures;

		Material() {};
		Material(MaterialStruct* matStruct);
		~Material() {};

		int GetRenderQueue();
	};
}