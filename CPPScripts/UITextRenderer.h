#pragma once
#include "pubh.h"
#include "Component.h"

namespace ZXEngine
{
	// 固定的字形Mesh顶点采样纹理UV
	const Vector2 GlyphCoords[4] = { Vector2(0, 1), Vector2(0, 0), Vector2(1, 1), Vector2(1, 0) };
	// 固定的字形Mesh三角形顶点数组
	const vector<unsigned int> GlyphIndices =
	{
		1, 0, 2,
		1, 2, 3,
	};

	class UITextRenderer : public Component
	{
	public:
		// 当前渲染的文本
		string text;
		// 字符颜色
		Vector4 color;

		UITextRenderer();
		~UITextRenderer() {};

		static ComponentType GetType();

		void Render();
	};
}