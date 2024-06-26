#pragma once
#include "RenderPass.h"

namespace ZXEngine
{
	class TextRenderer;
	class RenderPassUIRendering : public RenderPass
	{
	public:
		RenderPassUIRendering();
		~RenderPassUIRendering() {};

		virtual void Render(Camera* camera);

	private:
		uint32_t drawCommandID = 0;
	};
}