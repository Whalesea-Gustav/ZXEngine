#pragma once
#include "RenderAPI.h"
#include "Vulkan/VulkanEnumStruct.h"

namespace ZXEngine
{
    class RenderAPIVulkan : public RenderAPI
    {
        friend class EditorGUIManager;
    /// <summary>
    /// 标准RenderAPI接口
    /// </summary>
    public:
        RenderAPIVulkan();
        ~RenderAPIVulkan() {};

        virtual void BeginFrame();
        virtual void EndFrame();

        // 渲染状态设置
        virtual void SetRenderState(RenderStateSetting* state);
        virtual void SetViewPort(unsigned int width, unsigned int height, unsigned int xOffset = 0, unsigned int yOffset = 0);

        // FrameBuffer相关
        virtual void SwitchFrameBuffer(uint32_t id);
        virtual void ClearFrameBuffer(const ClearInfo& clearInfo);
        virtual FrameBufferObject* CreateFrameBufferObject(FrameBufferType type, unsigned int width = 0, unsigned int height = 0);

        // 资源加载相关
        virtual unsigned int LoadTexture(const char* path, int& width, int& height);
        virtual void DeleteTexture(unsigned int id);
        virtual unsigned int LoadCubeMap(vector<string> faces);
        virtual unsigned int GenerateTextTexture(unsigned int width, unsigned int height, unsigned char* data);
        virtual ShaderReference* LoadAndSetUpShader(const char* path, FrameBufferType type);
        virtual void SetUpMaterial(ShaderReference* shaderReference, const map<string, uint32_t>& textures);
        virtual void DeleteShader(unsigned int id);

        // Draw
        virtual uint32_t AllocateDrawCommand();
        virtual void Draw(uint32_t VAO);
        virtual void GenerateDrawCommand(uint32_t id);

        // Mesh设置
        virtual void DeleteMesh(unsigned int VAO);
        virtual void SetUpStaticMesh(unsigned int& VAO, vector<Vertex> vertices, vector<unsigned int> indices);
        virtual void SetUpDynamicMesh(unsigned int& VAO, unsigned int vertexSize, unsigned int indexSize);
        virtual void UpdateDynamicMesh(unsigned int VAO, vector<Vertex> vertices, vector<unsigned int> indices);
        virtual void GenerateParticleMesh(unsigned int& VAO);

        // Shader设置
        virtual void UseShader(unsigned int ID);
        virtual void SetShaderScalar(ShaderReference* reference, const string& name, bool value);
        virtual void SetShaderScalar(ShaderReference* reference, const string& name, int value);
        virtual void SetShaderScalar(ShaderReference* reference, const string& name, float value);
        virtual void SetShaderVector(ShaderReference* reference, const string& name, const Vector2& value);
        virtual void SetShaderVector(ShaderReference* reference, const string& name, const Vector2& value, uint32_t idx);
        virtual void SetShaderVector(ShaderReference* reference, const string& name, const Vector3& value);
        virtual void SetShaderVector(ShaderReference* reference, const string& name, const Vector3& value, uint32_t idx);
        virtual void SetShaderVector(ShaderReference* reference, const string& name, const Vector4& value);
        virtual void SetShaderVector(ShaderReference* reference, const string& name, const Vector4& value, uint32_t idx);
        virtual void SetShaderMatrix(ShaderReference* reference, const string& name, const Matrix3& value);
        virtual void SetShaderMatrix(ShaderReference* reference, const string& name, const Matrix3& value, uint32_t idx);
        virtual void SetShaderMatrix(ShaderReference* reference, const string& name, const Matrix4& value);
        virtual void SetShaderMatrix(ShaderReference* reference, const string& name, const Matrix4& value, uint32_t idx);
        virtual void SetShaderTexture(ShaderReference* reference, const string& name, uint32_t ID, uint32_t idx, bool isBuffer = false);
        virtual void SetShaderCubeMap(ShaderReference* reference, const string& name, uint32_t ID, uint32_t idx, bool isBuffer = false);


    /// <summary>
    /// 仅启动时一次性初始化的核心Vulkan组件及相关变量
    /// </summary>
    private:
        // 是否开启验证层
        bool validationLayersEnabled = false;
        // MSAA采样点数量
        VkSampleCountFlagBits msaaSamplesCount = VK_SAMPLE_COUNT_1_BIT;
        // 硬件支持多少倍的各项异性采样
        float maxSamplerAnisotropy = 1.0f;
        // 当前是MAX_FRAMES_IN_FLIGHT中的第几帧
        uint32_t currentFrame = 0;
        VkDeviceSize minUniformBufferOffsetAlignment = 8;

        // Vulkan实例
        VkInstance vkInstance = VK_NULL_HANDLE;
        // Debug工具
        VkDebugUtilsMessengerEXT debugMessenger = VK_NULL_HANDLE;
        // 物理设备
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        // 逻辑设备
        VkDevice device = VK_NULL_HANDLE;
        // Vulkan Memory Allocator(来自AMD的GPUOpen团队:https://github.com/GPUOpen-LibrariesAndSDKs/VulkanMemoryAllocator)
        VmaAllocator vmaAllocator;
        // Surface
        VkSurfaceKHR surface = VK_NULL_HANDLE;
        // 图形队列
        VkQueue graphicsQueue = VK_NULL_HANDLE;
        // 展示队列
        VkQueue presentQueue = VK_NULL_HANDLE;
        // 队列簇ID记录
        QueueFamilyIndices queueFamilyIndices;
        // 提供给交换链显示画面的Frame Buffer
        uint32_t presentFBOIdx = 0;

        // 交换链
        VkSwapchainKHR swapChain = VK_NULL_HANDLE;
        // 交换链Image(创建交换链的时候自动创建，销毁交换链的时候自动销毁)
        vector<VkImage> swapChainImages;
        // 交换链ImageView，手动创建，手动销毁
        vector<VkImageView> swapChainImageViews;
        // 交换链上图像的格式
        VkFormat swapChainImageFormat;
        // 交换链上图像的大小
        VkExtent2D swapChainExtent;
        // 当前这一帧所用的Image在交换链里的索引
        uint32_t curPresentImageIdx = 0;
        // 交换链Image可用的信号量
        vector<VkSemaphore> presentImageAvailableSemaphores;

        // 命令池
        VkCommandPool commandPool = VK_NULL_HANDLE;

        // ------------------------------------------建立各种Vulkan对象--------------------------------------------

        void CreateVkInstance();
        void CreateDebugMessenger();
        void CreatePhysicalDevice();
        void CreateLogicalDevice();
        void CreateMemoryAllocator();
        void CreateCommandPool();
        void CreateSurface();
        void CreateSwapChain();
        void CreatePresentFrameBuffer();

        // ----------------------------------------建立Vulkan对象的辅助函数----------------------------------------

        bool CheckValidationLayerSupport();
        vector<const char*> GetRequiredExtensions();
        // 创建调试用的VkDebugUtilsMessenger
        VkResult CreateDebugUtilsMessengerEXT(const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
        // 销毁VkDebugUtilsMessenger
        void DestroyDebugUtilsMessengerEXT(VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
        // 检查物理设备是否符合需求
        bool IsPhysicalDeviceSuitable(VkPhysicalDevice device);
        // 获取物理设备队列簇索引
        QueueFamilyIndices GetQueueFamilyIndices(VkPhysicalDevice device);
        // 检查物理设备是否支持所需要的扩展
        bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
        // 获取物理设备所支持的交换链信息
        SwapChainSupportDetails GetSwapChainSupportDetails(VkPhysicalDevice device);
        // 获取硬件设备属性
        void GetPhysicalDeviceProperties();
        // 选择交换链图像格式和色彩空间
        VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const vector<VkSurfaceFormatKHR>& availableFormats);
        // 选择交换链的展示模式
        VkPresentModeKHR ChooseSwapPresentMode(const vector<VkPresentModeKHR> availablePresentModes);
        // 选择交换链图像分辨率
        VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);


    /// <summary>
    /// Vulkan资源创建相关接口(这些接口Create出来的需要手动Destroy)
    /// </summary>
    private:
        vector<VulkanVAO*> VulkanVAOArray;
        vector<VulkanFBO*> VulkanFBOArray;
        vector<VulkanAttachmentBuffer*> VulkanAttachmentBufferArray;
        vector<VulkanTexture*> VulkanTextureArray;
        vector<VulkanPipeline*> VulkanPipelineArray;
        vector<VulkanDrawCommand*> VulkanDrawCommandArray;

        vector<pair<uint32_t, uint32_t>> drawIndexes;

        vector<VkRenderPass> allVulkanRenderPass;

        uint32_t GetNextVAOIndex();
        VulkanVAO* GetVAOByIndex(uint32_t idx);
        uint32_t GetNextFBOIndex();
        VulkanFBO* GetFBOByIndex(uint32_t idx);
        uint32_t GetNextAttachmentBufferIndex();
        VulkanAttachmentBuffer* GetAttachmentBufferByIndex(uint32_t idx);
        uint32_t GetNextTextureIndex();
        VulkanTexture* GetTextureByIndex(uint32_t idx);
        uint32_t GetNextPipelineIndex();
        VulkanPipeline* GetPipelineByIndex(uint32_t idx);
        uint32_t GetNextDrawCommandIndex();
        VulkanDrawCommand* GetDrawCommandByIndex(uint32_t idx);

        void* GetShaderPropertyAddress(ShaderReference* reference, const string& name, uint32_t idx = 0);

        VulkanBuffer CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage, bool map = false);
        void DestroyBuffer(VulkanBuffer buffer);

        UniformBuffer CreateUniformBuffer(const vector<ShaderProperty>& properties);

        void AllocateCommandBuffers(vector<VkCommandBuffer>& commandBuffers);

        VkFence CreateFence();

        VulkanImage CreateImage(uint32_t width, uint32_t height, uint32_t mipLevels, uint32_t layers, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VmaMemoryUsage memoryUsage);
        void DestroyImage(VulkanImage image);
        void GenerateMipMaps(VkImage image, VkFormat format, int32_t width, int32_t height, uint32_t mipLevels);

        VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageViewType viewType);
        void DestroyImageView(VkImageView imageView);

        VkSampler CreateSampler(uint32_t mipLevels);

        uint32_t CreateVulkanTexture(VulkanImage image, VkImageView imageView, VkSampler sampler);

        void CreateAllRenderPass();
        VkRenderPass CreateRenderPass(RenderPassType type);
        VkRenderPass GetRenderPass(RenderPassType type);
        void DestroyRenderPass(VkRenderPass renderPass);

        VkPipeline CreatePipeline(const string& path, const ShaderInfo& shaderInfo, VkDescriptorSetLayout& descriptorSetLayout, VkPipelineLayout& pipelineLayout, RenderPassType renderPassType);
        void SetUpPipeline(VulkanPipeline* pipeline);
        
        VkDescriptorSetLayout CreateDescriptorSetLayout(const ShaderInfo& info);
        VkPipelineLayout CreatePipelineLayout(const VkDescriptorSetLayout& descriptorSetLayout);
        VkDescriptorPool CreateDescriptorPool(const ShaderInfo& info);
        vector<VkDescriptorSet> CreateDescriptorSets(VkDescriptorPool descriptorPool, const vector<VkDescriptorSetLayout>& descriptorSetLayouts);

        VkShaderModule CreateShaderModule(vector<char> code);
        ShaderModuleSet CreateShaderModules(const string& path, const ShaderInfo& info);
        void DestroyShaderModules(ShaderModuleSet shaderModules);


    /// <summary>
    /// 其它辅助接口
    /// </summary>
    public:
        bool windowResized = false;

    private:
        uint32_t curFBOIdx = 0;
        uint32_t curPipeLineIdx = 0;

        VkFence immediateExeFence;
        VkCommandBuffer immediateExeCmd;
        ViewPortInfo viewPortInfo;

        uint32_t GetCurFrameBufferIndex();
        uint32_t GetMipMapLevels(int width, int height);
        void InitImmediateCommand();
        void ImmediatelyExecute(std::function<void(VkCommandBuffer cmd)>&& function);
        void TransitionImageLayout(VkImage image, VkImageLayout oldLayout, VkImageLayout newLayout, VkImageAspectFlags aspectMask, VkPipelineStageFlags srcStage, VkAccessFlags srcAccessMask, VkPipelineStageFlags dstStage, VkAccessFlags dstAccessMask);

        VkPipelineInputAssemblyStateCreateInfo GetAssemblyInfo(VkPrimitiveTopology topology);
        VkPipelineRasterizationStateCreateInfo GetRasterizationInfo(VkCullModeFlagBits cullMode);
        VkPipelineMultisampleStateCreateInfo GetPipelineMultisampleInfo(VkSampleCountFlagBits rasterizationSamples);
    };
}