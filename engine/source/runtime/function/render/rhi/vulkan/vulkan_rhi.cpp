#include "runtime/function/render/rhi/vulkan/vulkan_rhi.h"
#include "runtime/function/render/rhi/vulkan/vulkan_util.h"
#include "runtime/function/render/rhi/vulkan/vulkan_reflection_util.h"

#include "runtime/function/render/window_system.h"

#include "runtime/core/base/templates.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <set>
#include <stdexcept>


namespace Polaris
{
    /*
    * Clear Vulkan instances and resources
    */
    VulkanRHI::~VulkanRHI()
    {
        cleanupFramebufferImageResources();
        cleanupSwapchain();

        vkDestroyDescriptorPool(m_device, m_descriptorPool, m_defaultAllocator);

        for (size_t i = 0; i < m_maxFrameInFlight; i++) {
            vkDestroySemaphore(m_device, m_imageAvaliableForRenderSemaphore[i], m_defaultAllocator);
            vkDestroySemaphore(m_device, m_imageRenderFinishedForPresentSemaphores[i], m_defaultAllocator);
            vkDestroyFence(m_device, m_imageInFlightFences[i], m_defaultAllocator);
        }

        vkDestroyCommandPool(m_device, m_commandPool, m_defaultAllocator);

        vkDestroyDevice(m_device, m_defaultAllocator);
        vkDestroySurfaceKHR(m_instance, m_surface, m_defaultAllocator);
        VulkanUtil::destroyDebugUtilsMessengerEXT(m_instance, m_debugMessenger, m_defaultAllocator);
        vkDestroyInstance(m_instance, m_defaultAllocator);
    }

    void VulkanRHI::initialize(RHIInitInfo initInfo)
    {
        //Specify vulkan api settings
        m_window = initInfo.window_system->getWindow();
        m_apiMajor = 1;
        m_apiMinor = 2;
        m_apiVersion = VK_MAKE_API_VERSION(0, m_apiMajor, m_apiMinor, 0);
        m_instanceLayers = { "VK_LAYER_KHRONOS_validation" };
        m_enableValidationLayers = !m_instanceLayers.empty();
        m_debugMode = true;

        // Memory allocator
        m_defaultAllocator = nullptr;

        // Instance extensions
        m_instanceExtensions = {};

        // GLFW extensions
        uint32_t count{ 0 };
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&count);
        for (int extId = 0; extId < count; ++extId)
        {
            m_instanceExtensions.push_back(glfwExtensions[extId]);
        }

        // Debug extension
        if (m_enableValidationLayers)
        {
            m_instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
        }

        // Device extensions and physical device features
        addDeviceExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
        VkPhysicalDeviceAccelerationStructureFeaturesKHR accelFeature{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR };
        addDeviceExtension(VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME, &accelFeature, { "accelerationStructure" });
        VkPhysicalDeviceRayTracingPipelineFeaturesKHR rtPipelineFeature{ VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR };
        addDeviceExtension(VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME, &rtPipelineFeature, { "rayTracingPipeline" });  // To use vkCmdTraceRaysKHR
        addDeviceExtension(VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME);  // Required by ray tracing pipeline
        addPhysicalDeviceFeatureRequirement(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2, "samplerAnisotropy");
        addPhysicalDeviceFeatureRequirement(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2, "fragmentStoresAndAtomics");  // support inefficient readback storage buffer
        addPhysicalDeviceFeatureRequirement(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2, "independentBlend");          // support independent blending
        addPhysicalDeviceFeatureRequirement(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2, "geometryShader");            // support geometry shader
        addPhysicalDeviceFeatureRequirement(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES, "multiviewGeometryShader"); // Test struct chain
        addPhysicalDeviceFeatureRequirement(VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES, "imagelessFramebuffer");    // Test struct chain
        constructStructChain(); // Construct the struct chain for physical device features  

        // Command buffer setting
        m_maxFrameInFlight = 3;
        m_currentFrameIndex = 0;

        // Descriptor pool settings
        m_maxVertexBlendingMeshCount = 256;
        m_maxMaterialCount = 256;
        setup(initInfo);
        createInstance();
        createDebugMessenger();
        createSurface();
        createPhysicalDevice();
        createLogicalDevice();
        initializeCommandPools();
        initializeCommandBuffers();
        createSyncObjects();
        createDescriptorPools();
        createSwapchain();
        createSwapchainImageViews();
        createFramebufferImageResources();
        createAssetAllocator();
    }

    /*
    * Run every frame
    */
    void VulkanRHI::tick()
    {

    }

    void VulkanRHI::setup(RHIInitInfo initInfo)
    {
        //TODO: fix bug
    }

    /*
    * Create the Vulkan instance that has given layers, and setup debug messenger if validation layers are enabled
    */
    void VulkanRHI::createInstance()
    {
        // Check the availability for required validation layers
        if (m_enableValidationLayers && !checkValidationLayerSupport()) 
        {
            throw std::runtime_error("validation layers requested, but not available!");
        }

        // App info
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "MyApp";
        appInfo.pEngineName = "No Engine";
        appInfo.apiVersion = m_apiVersion;

        // Instance create info
        VkInstanceCreateInfo instanceCreateInfo{};
        instanceCreateInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        instanceCreateInfo.pApplicationInfo = &appInfo;
        instanceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(m_instanceExtensions.size());
        instanceCreateInfo.ppEnabledExtensionNames = m_instanceExtensions.data(); //Instance extensions
        
        // Instance layers
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{};
        if (m_enableValidationLayers)
        {
            instanceCreateInfo.enabledLayerCount = static_cast<uint32_t>(m_instanceLayers.size());
            instanceCreateInfo.ppEnabledLayerNames = m_instanceLayers.data();
            VulkanUtil::populateDebugMessengerCreateInfo(debugCreateInfo, VulkanUtil::plainDebugCallback, m_debugMode);
            instanceCreateInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT*)&debugCreateInfo;
        }
        else
        {
            instanceCreateInfo.enabledLayerCount = 0;
            instanceCreateInfo.pNext = nullptr;
        }

        // Create instance
        if (vkCreateInstance(&instanceCreateInfo, m_defaultAllocator, &m_instance) != VK_SUCCESS) 
        {
            throw std::runtime_error("failed to create instance!");
        }
    }

    /*
    * Create debug messenger if validation layers are enabled
    */
    void VulkanRHI::createDebugMessenger()
    {
        if (m_enableValidationLayers)
        {
            VkDebugUtilsMessengerCreateInfoEXT createInfo;
            VulkanUtil::populateDebugMessengerCreateInfo(createInfo, VulkanUtil::plainDebugCallback, m_debugMode);
            if (VK_SUCCESS != VulkanUtil::createDebugUtilsMessengerEXT(m_instance, &createInfo, m_defaultAllocator, &m_debugMessenger))
            {
                throw std::runtime_error("failed to set up debug messenger!");
            }
        }
    }

    void VulkanRHI::createSurface()
    {
        if (glfwCreateWindowSurface(m_instance, m_window, nullptr, &m_surface) != VK_SUCCESS)
        {
            throw std::runtime_error("glfwCreateWindowSurface");
        }
    }

    /*
    * Create physical device from a suitable physical device and get queue families
    */
    void VulkanRHI::createPhysicalDevice()
    {
        //Check if there are any physical device available
        uint32_t availableDeviceCount = 0;
        vkEnumeratePhysicalDevices(m_instance, &availableDeviceCount, nullptr);
        if (availableDeviceCount == 0) 
        {
            throw std::runtime_error("Failed to find physical device with Vulkan support!");
        }

        std::vector<VkPhysicalDevice> compatibleDevices{};
        std::vector<VkPhysicalDevice> availableDevices(availableDeviceCount);
        vkEnumeratePhysicalDevices(m_instance, &availableDeviceCount, availableDevices.data());

        //Find all suitable devices	
        for (const VkPhysicalDevice device : availableDevices) 
        {
            if (isDeviceSuitable(device)) 
            {
                compatibleDevices.push_back(device);
                break;
            }
        }

        if (compatibleDevices.size() > 0)
        {
            m_physicalDevice = compatibleDevices[0];
        }
        else
        {
            throw std::runtime_error("Cannot find any compatible physical device");
        }

        m_queueFamilyIndices = findQueueFamilies(m_physicalDevice);
    }

    /*
    * Create logical device, efficient function pointers and queues
    */
    void VulkanRHI::createLogicalDevice()
    {
        // Initialize queue create infos
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = {
            m_queueFamilyIndices.graphicsFamily.value(), m_queueFamilyIndices.computeFamily.value(),
            m_queueFamilyIndices.transferFamily.value(), m_queueFamilyIndices.presentFamily.value()
        };

        float queuePriority = 1.0f; // TODO compute queue priority
        for (uint32_t queueFamily : uniqueQueueFamilies) 
        {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority;
            queueCreateInfos.push_back(queueCreateInfo);
        }

        // Device create info
        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data(); // Add queue create infos
        createInfo.pEnabledFeatures = nullptr;
        createInfo.pNext = &m_physicalFeaturesStructChain; // When version >= vulkan1.1 we use pNext to add physical device features
        createInfo.enabledExtensionCount = static_cast<uint32_t>(m_deviceExtensions.size());
        createInfo.ppEnabledExtensionNames = m_deviceExtensions.data(); // Add device extensions
        createInfo.enabledLayerCount = 0;

        // Create logical device
        if (vkCreateDevice(m_physicalDevice, &createInfo, nullptr, &m_device) != VK_SUCCESS) 
        {
            throw std::runtime_error("failed to create logical device!");
        }

        // Create efficient function pointers
        m_pfn_vkSetDebugUtilsObjectNameEXT = (PFN_vkSetDebugUtilsObjectNameEXT)vkGetInstanceProcAddr(m_instance, "vkSetDebugUtilsObjectNameEXT");

        // Create queues
        vkGetDeviceQueue(m_device, m_queueFamilyIndices.graphicsFamily.value(), 0, &m_graphicsQueue);
        setObjectName(m_graphicsQueue, "Graphics Queue");
        vkGetDeviceQueue(m_device, m_queueFamilyIndices.computeFamily.value(), 0, &m_computeQueue);
        setObjectName(m_computeQueue, "Compute Queue");
        vkGetDeviceQueue(m_device, m_queueFamilyIndices.transferFamily.value(), 0, &m_transferQueue);
        setObjectName(m_transferQueue, "Transfer Queue");
        vkGetDeviceQueue(m_device, m_queueFamilyIndices.presentFamily.value(), 0, &m_presentQueue);
        setObjectName(m_presentQueue, "Present Queue");
    }

    /*
    * Initialize a command pool on the graphics queue family
    */
    void VulkanRHI::initializeCommandPools()
    {
        // Create the default command pool of the graphics queue family
        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType              = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.queueFamilyIndex   = m_queueFamilyIndices.graphicsFamily.value();
        poolInfo.flags              = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

        if (vkCreateCommandPool(m_device, &poolInfo, nullptr, &m_commandPool) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create graphics command pool!");
        }

        setObjectName(m_commandPool, "Default Graphics CommandPool");
    }

    /*
    * Create command buffer for each frame
    */
    void VulkanRHI::initializeCommandBuffers()
    {   
        // Create graphics command buffers
        m_commandBuffers.resize(m_maxFrameInFlight);
        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = m_commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = (uint32_t)m_commandBuffers.size();

        if (vkAllocateCommandBuffers(m_device, &allocInfo, m_commandBuffers.data()) != VK_SUCCESS) 
        {
            throw std::runtime_error("failed to allocate command buffers!");
        }

        for (int i = 0; i < m_commandBuffers.size(); ++i)
        {
            setObjectName(m_commandBuffers[i], "Default Command Buffer [" + std::to_string(i) + "]");
        }
    }

    /*
    * Create descriptor pool for descriptor resources
    */
    void VulkanRHI::createDescriptorPools()
    {
        VkDescriptorPoolSize pool_sizes[5];
        pool_sizes[0].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
        pool_sizes[0].descriptorCount = 3 + 2 + 2 + 2 + 1 + 1 + 3 + 3;
        pool_sizes[1].type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        pool_sizes[1].descriptorCount = 1 + 1 + 1 * m_maxVertexBlendingMeshCount;
        pool_sizes[2].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        pool_sizes[2].descriptorCount = 1 * m_maxMaterialCount;
        pool_sizes[3].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        pool_sizes[3].descriptorCount = 3 + 5 * m_maxMaterialCount + 1 + 1; // ImGui_ImplVulkan_CreateDeviceObjects
        pool_sizes[4].type = VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT;
        pool_sizes[4].descriptorCount = 4 + 1 + 1 + 2;

        VkDescriptorPoolCreateInfo pool_info{};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.poolSizeCount = sizeof(pool_sizes) / sizeof(pool_sizes[0]);
        pool_info.pPoolSizes = pool_sizes;
        pool_info.maxSets =
            1 + 1 + 1 + m_maxMaterialCount + m_maxVertexBlendingMeshCount + 1 + 1; // +skybox + axis descriptor set
        pool_info.flags = 0U;

        if (vkCreateDescriptorPool(m_device, &pool_info, nullptr, &m_descriptorPool) != VK_SUCCESS)
        {
            throw std::runtime_error("create descriptor pool");
        }

        setObjectName(m_descriptorPool, "Global Descriptor Pool");
    }

    /*
    * Create the image available and render finished semaphores and image in flight fences
    */
    void VulkanRHI::createSyncObjects()
    {
        m_imageAvaliableForRenderSemaphore.resize(m_maxFrameInFlight);
        m_imageRenderFinishedForPresentSemaphores.resize(m_maxFrameInFlight);
        m_imageInFlightFences.resize(m_maxFrameInFlight);

        VkSemaphoreCreateInfo semaphore_create_info{};
        semaphore_create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

        VkFenceCreateInfo fence_create_info{};
        fence_create_info.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
        fence_create_info.flags = VK_FENCE_CREATE_SIGNALED_BIT; // the fence is initialized as signaled

        for (uint32_t i = 0; i < m_maxFrameInFlight; i++)
        {
            if (vkCreateSemaphore(m_device, &semaphore_create_info, nullptr, &m_imageAvaliableForRenderSemaphore[i]) != VK_SUCCESS ||
                vkCreateSemaphore(m_device, &semaphore_create_info, nullptr, &m_imageRenderFinishedForPresentSemaphores[i]) != VK_SUCCESS ||
                vkCreateFence(m_device, &fence_create_info, nullptr, &m_imageInFlightFences[i]) != VK_SUCCESS)
            {
                throw std::runtime_error("vk create semaphore & fence");
            }

            setObjectName(m_imageAvaliableForRenderSemaphore[i], "Image Available For Render Semaphore [" + std::to_string(i) + "]");
            setObjectName(m_imageRenderFinishedForPresentSemaphores[i], "Image Render Finished For Present Semaphore [" + std::to_string(i) + "]");
            setObjectName(m_imageInFlightFences[i], "Image In Flight Fence [" + std::to_string(i) + "]");
        }
    }

    void VulkanRHI::createSwapchain()
    {
        uint32_t imageCount;
        // Query supports of this physical device and surface and choose format, present mode and extent
        SwapChainSupportDetails swapchainSupport = querySwapChainSupport(m_physicalDevice, m_surface);
        VkSurfaceFormatKHR surfaceFormat = chooseSwapchainSurfaceFormat(swapchainSupport.formats);
        VkPresentModeKHR presentMode = chooseSwapchainPresentMode(swapchainSupport.presentModes);
        VkExtent2D extent = chooseSwapchainExtent(swapchainSupport.capabilities);

        imageCount = swapchainSupport.capabilities.minImageCount + 1;
        if (swapchainSupport.capabilities.maxImageCount > 0 && imageCount > swapchainSupport.capabilities.maxImageCount)
        {
            imageCount = swapchainSupport.capabilities.maxImageCount;
        }

        VkSwapchainCreateInfoKHR createInfo{};
        createInfo.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
        createInfo.surface          = m_surface;
        createInfo.minImageCount    = imageCount;
        createInfo.imageFormat      = surfaceFormat.format;
        createInfo.imageColorSpace  = surfaceFormat.colorSpace;
        createInfo.imageExtent      = extent;
        createInfo.imageArrayLayers = 1;
        createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

        uint32_t queueFamilyIndices[] = { m_queueFamilyIndices.graphicsFamily.value(), m_queueFamilyIndices.presentFamily.value() };

        if (m_queueFamilyIndices.graphicsFamily != m_queueFamilyIndices.presentFamily) 
        {
            createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices   = queueFamilyIndices;
        }
        else 
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        }

        createInfo.preTransform     = swapchainSupport.capabilities.currentTransform;
        createInfo.compositeAlpha   = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
        createInfo.presentMode      = presentMode;
        createInfo.clipped          = VK_TRUE;

        if (vkCreateSwapchainKHR(m_device, &createInfo, nullptr, &m_swapchain) != VK_SUCCESS) 
        {
            throw std::runtime_error("failed to create swapchain!");
        }

        vkGetSwapchainImagesKHR(m_device, m_swapchain, &imageCount, nullptr);
        m_swapchainImages.resize(imageCount);
        vkGetSwapchainImagesKHR(m_device, m_swapchain, &imageCount, m_swapchainImages.data());

        for (int i = 0; i < m_swapchainImages.size(); ++i)
        {
            setObjectName(m_swapchainImages[i], "Swapchain Image [" + std::to_string(i) + "]");
        }

        m_swapchainImageFormat = surfaceFormat.format;
        m_swapchainExtent = extent;

        m_scissor = { {0, 0}, {m_swapchainExtent.width, m_swapchainExtent.height} };
    }

    void VulkanRHI::createSwapchainImageViews()
    {
        m_swapchainImageViews.resize(m_swapchainImages.size());
        // create imageview (one for each this time) for all swapchain images
        for (size_t i = 0; i < m_swapchainImages.size(); i++)
        {
            m_swapchainImageViews[i] = VulkanUtil::createImageView(m_device,
                                                                   m_swapchainImages[i],
                                                                   m_swapchainImageFormat,
                                                                   VK_IMAGE_ASPECT_COLOR_BIT,
                                                                   VK_IMAGE_VIEW_TYPE_2D,
                                                                   1,
                                                                   1);
            setObjectName(m_swapchainImageViews[i], "Swapchain ImageView [" + std::to_string(i) + "]");
        }
    }

    /*
    * Create framebuffer image resources
    */
    void VulkanRHI::createFramebufferImageResources()
    {
        // Set depth format
        m_depthFormat = findDepthFormat(m_physicalDevice);

        VulkanUtil::createImage(m_physicalDevice,
                                m_device,
                                m_swapchainExtent.width,
                                m_swapchainExtent.height,
                                m_depthFormat,
                                VK_IMAGE_TILING_OPTIMAL,
                                VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT |
                                VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT,
                                VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                                m_depthImage,
                                m_depthImageMemory,
                                0,
                                1,
                                1);

        m_depthImageView = VulkanUtil::createImageView(m_device, 
                                                       m_depthImage,
                                                       m_depthFormat,
                                                       VK_IMAGE_ASPECT_DEPTH_BIT, 
                                                       VK_IMAGE_VIEW_TYPE_2D, 
                                                       1, 
                                                       1);
        setObjectName(m_depthImage, "Depth Image");
        setObjectName(m_depthImageMemory, "Depth Image Memory");
        setObjectName(m_depthImageView, "Depth Image View");
    }

    /*
    * Create the asset allocator using VMA library
    */
    void VulkanRHI::createAssetAllocator()
    {
        VmaVulkanFunctions vulkanFunctions = {};
        vulkanFunctions.vkGetInstanceProcAddr = &vkGetInstanceProcAddr;
        vulkanFunctions.vkGetDeviceProcAddr = &vkGetDeviceProcAddr;

        VmaAllocatorCreateInfo allocatorCreateInfo = {};
        allocatorCreateInfo.vulkanApiVersion = m_apiVersion;
        allocatorCreateInfo.physicalDevice = m_physicalDevice;
        allocatorCreateInfo.device = m_device;
        allocatorCreateInfo.instance = m_instance;
        allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;

        vmaCreateAllocator(&allocatorCreateInfo, &m_assetAllocator);
    }



    void VulkanRHI::cleanupSwapchain()
    {
        for (auto imageView : m_swapchainImageViews)
        {
            vkDestroyImageView(m_device, imageView, m_defaultAllocator);
        }
        vkDestroySwapchainKHR(m_device, m_swapchain, m_defaultAllocator);
    }

    void VulkanRHI::cleanupFramebufferImageResources()
    {
        vkDestroyImageView(m_device, m_depthImageView, m_defaultAllocator);
        vkDestroyImage(m_device, m_depthImage, m_defaultAllocator);
        vkFreeMemory(m_device, m_depthImageMemory, m_defaultAllocator);
    }

    /*
    * Recreate the swapchain and its images and imageviews when the present extent changes
    */
    void VulkanRHI::recreateSwapchain()
    {
        int width = 0;
        int height = 0;
        glfwGetFramebufferSize(m_window, &width, &height);
        while (width == 0 || height == 0) // minimized 0,0, pause for now
        {
            glfwGetFramebufferSize(m_window, &width, &height);
            glfwWaitEvents();
        }

        VkResult res_wait_for_fences =
            vkWaitForFences(m_device, m_maxFrameInFlight, m_imageInFlightFences.data(), VK_TRUE, UINT64_MAX);
        assert(VK_SUCCESS == res_wait_for_fences);

        cleanupFramebufferImageResources();
        cleanupSwapchain();

        createSwapchain();
        createSwapchainImageViews();
        createFramebufferImageResources();
    }

    /*
    * Add device extension requirement and corresponding physical device feature requirements
    */
    void VulkanRHI::addDeviceExtension(const char* extension, void* pPhysicalDeviceFeatureStruct, std::vector<const char*> featureRequirements) {
        add_unique(m_deviceExtensions, extension);
        if (pPhysicalDeviceFeatureStruct != nullptr) 
        {
            m_EXTPhysicalDeviceFeatureStructs.push_back(pPhysicalDeviceFeatureStruct);
            VkStructureType sType = reinterpret_cast<VulkanExtensionHeader*>(pPhysicalDeviceFeatureStruct)->sType;
            if (m_physicalDeviceFeatureRequirements.find(sType) != m_physicalDeviceFeatureRequirements.end())
            {
                m_physicalDeviceFeatureRequirements[sType].insert(m_physicalDeviceFeatureRequirements[sType].end(), 
                                                                  featureRequirements.begin(), 
                                                                  featureRequirements.end());
            }
            else if (featureRequirements.size() > 0)
            {
                m_physicalDeviceFeatureRequirements[sType] = featureRequirements;
            }

         }
    }

    /*
    * Add physical feature requirement
    */
    void VulkanRHI::addPhysicalDeviceFeatureRequirement(VkStructureType featureStructType, const char* feature) {
        if (m_physicalDeviceFeatureRequirements.find(featureStructType) != m_physicalDeviceFeatureRequirements.end())
            add_unique(m_physicalDeviceFeatureRequirements[featureStructType], feature);
        else
            m_physicalDeviceFeatureRequirements[featureStructType] = std::vector<const char*>{ feature };
    }

    /*
    * Check if the required validation layers are supported
    */
    bool VulkanRHI::checkValidationLayerSupport() {
        uint32_t layerCount;
        vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

        std::vector<VkLayerProperties> availableLayers(layerCount);
        vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

        for (const char* layerName : m_instanceLayers) 
        {
            bool layerFound = false;

            for (const auto& layerProperties : availableLayers) 
            {
                if (strcmp(layerName, layerProperties.layerName) == 0) 
                {
                    layerFound = true;
                    break;
                }
            }

            if (!layerFound) 
                return false;
        }

        return true;
    }

    /*
    * Construct the struct chain of physical device features
    */
    void VulkanRHI::constructStructChain() {
        m_physicalFeaturesStructChain.features = m_features10;
        // Only support Vulkan 1.2 for now
        if (m_apiMajor == 1 && m_apiMinor >= 2)
        {
            m_physicalFeaturesStructChain.pNext = &m_features11;
            m_features11.pNext = &m_features12;
            m_features12.pNext = nullptr;
        }
        // use the physicalFeaturesStructChain to append extensions
        if (!m_EXTPhysicalDeviceFeatureStructs.empty())
        {
            // build up chain of all used extension features
            for (size_t i = 0; i < m_EXTPhysicalDeviceFeatureStructs.size(); i++)
            {
                auto* header = reinterpret_cast<VulkanExtensionHeader*>(m_EXTPhysicalDeviceFeatureStructs[i]);
                header->pNext = i < m_EXTPhysicalDeviceFeatureStructs.size() - 1 ? m_EXTPhysicalDeviceFeatureStructs[i + 1] : nullptr;
            }

            // append to the end of current feature2 struct
            VulkanExtensionHeader* lastCoreFeature = (VulkanExtensionHeader*)&m_physicalFeaturesStructChain;
            while (lastCoreFeature->pNext != nullptr)
            {
                lastCoreFeature = (VulkanExtensionHeader*)lastCoreFeature->pNext;
            }
            lastCoreFeature->pNext = m_EXTPhysicalDeviceFeatureStructs[0];
        }
    }

    /*
    * Check whether the physical device has required features and support required extensions and queue families
    */
    bool VulkanRHI::isDeviceSuitable(VkPhysicalDevice device)
    {
        m_queueFamilyIndices = findQueueFamilies(device);               //Initialize queue family indices and check queue family support
        bool extensionsSupported = checkDeviceExtensionSupport(device, m_deviceExtensions); //Check device extension support

        //Check presentation support if surface is assigned
        uint32_t formatCount;
        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, m_surface, &formatCount, nullptr);
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, m_surface, &presentModeCount, nullptr);
        bool presentSupported = formatCount > 0 && presentModeCount > 0;

        //Check physical device features support
        vkGetPhysicalDeviceFeatures2(device, &m_physicalFeaturesStructChain);
        bool requiredFeaturesSupported = checkDeviceFeaturesSupport(device, m_physicalDeviceFeatureRequirements);

        return extensionsSupported && requiredFeaturesSupported && m_queueFamilyIndices.isComplete() && presentSupported ;
    }

   /*
   * Return the queue families supported by give physical device
   */
    QueueFamilyIndices  VulkanRHI::findQueueFamilies(VkPhysicalDevice device) {
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        for (const auto& queueFamily : queueFamilies) {
            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                m_queueFamilyIndices.graphicsFamily = i;
            }

            if (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) {
                m_queueFamilyIndices.computeFamily = i;
            }

            if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) {
                m_queueFamilyIndices.transferFamily = i;
            }

            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, m_surface, &presentSupport);
            if (presentSupport) {
                m_queueFamilyIndices.presentFamily = i;
            }

            if (m_queueFamilyIndices.isComplete()) break;
            i++;
        }

        return m_queueFamilyIndices;
    }

    /*
    * Check whether the required extensions are supported by the physical device
    */
    bool VulkanRHI::checkDeviceExtensionSupport(VkPhysicalDevice device, std::vector<const char*> deviceExtensions) {
        uint32_t extensionCount;
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

        std::vector<VkExtensionProperties> availableExtensions(extensionCount);
        vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

        std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end());

        for (const auto& extension : availableExtensions)
            requiredExtensions.erase(extension.extensionName);


        return requiredExtensions.empty();
    }
    
    /*
    * Check whether the required features are supported by the physical device
    */
    bool VulkanRHI::checkDeviceFeaturesSupport(VkPhysicalDevice device, std::map<VkStructureType, std::vector<const char*>> physicalDeviceFeatureRequirements) {
        bool res = true;
        VulkanExtensionHeader* pStructChainIterator = reinterpret_cast<VulkanExtensionHeader*>(&m_physicalFeaturesStructChain);
        while (pStructChainIterator != nullptr) 
        {
            VkStructureType sType = pStructChainIterator->sType;
            if (physicalDeviceFeatureRequirements.find(sType) != physicalDeviceFeatureRequirements.end())
                res = res && checkDeviceFeaturesSupport(device, pStructChainIterator, physicalDeviceFeatureRequirements[sType]);
            pStructChainIterator = (VulkanExtensionHeader*)pStructChainIterator->pNext;
        }
        return true;
    }

    /*
    * Check whether the required features of certain feature structure are supported by the physical device
    */
    bool VulkanRHI::checkDeviceFeaturesSupport(VkPhysicalDevice device, void* pFeatures, std::vector<const char*> requiredFeatures) {
        std::string structName = VulkanReflectionUtil::getVkBool32StructName(pFeatures);
        bool res = true;
        for (const char* featureName : requiredFeatures)
            res = res && VulkanReflectionUtil::getVkBool32StructValue(pFeatures, featureName);
        return res;
    }

    /*
    * Set debug name of the object
    */
    void VulkanRHI::setObjectName(const uint64_t object, const std::string& name, VkObjectType t)
    {
        VkDebugUtilsObjectNameInfoEXT s{ VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT, nullptr, t, object, name.c_str() };
        m_pfn_vkSetDebugUtilsObjectNameEXT(m_device, &s);
    }

    /*
    * Return the supported swapchain types of the physical device and the surface
    */
    SwapChainSupportDetails VulkanRHI::querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface) {
        SwapChainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.capabilities);
        
        //formats
        uint32_t formatCount;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
        if (formatCount != 0) 
        {
            details.formats.resize(formatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.formats.data());
        }

        //present modes
        uint32_t presentModeCount;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
        if (presentModeCount != 0) 
        {
            details.presentModes.resize(presentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data());
        }

        return details;
    }

    /*
    * Specify the surface format for the currrent Swapchain
    */
    VkSurfaceFormatKHR VulkanRHI::chooseSwapchainSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {
        for (const auto& availableFormat : availableFormats) 
        {
            if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) 
            {
                return availableFormat;
            }
        }
        return availableFormats[0];
    }

    /*
    * Specify the present mode format for the currrent Swapchain, prefer VK_PRESENT_MODE_MAILBOX_KHR
    */
    VkPresentModeKHR VulkanRHI::chooseSwapchainPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes) {
        for (const auto& availablePresentMode : availablePresentModes) 
        {
            if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) 
            {
                return availablePresentMode;
            }
        }
        return VK_PRESENT_MODE_FIFO_KHR;
    }

    /*
    * Specify the extent for the currrent Swapchain given the current window shape
    */
    VkExtent2D VulkanRHI::chooseSwapchainExtent(const VkSurfaceCapabilitiesKHR& capabilities) {
        if (capabilities.currentExtent.width != UINT32_MAX) 
        {
            return capabilities.currentExtent;
        }
        else 
        {
            int width, height;
            glfwGetFramebufferSize(m_window, &width, &height);
            VkExtent2D actualExtent = { static_cast<uint32_t>(width), static_cast<uint32_t>(height) };

            actualExtent.width = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
            actualExtent.height = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

            return actualExtent;
        }
    }
    
    /*
    * Select a depth format supported by given physical device
    */
    VkFormat VulkanRHI::findDepthFormat(VkPhysicalDevice physicalDevice) {
        return findSupportedFormat(
            physicalDevice,
            { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
            VK_IMAGE_TILING_OPTIMAL,
            VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
        );
    }

    /*
    * Select a format supported by current physical device with certain flags
    */
    VkFormat VulkanRHI::findSupportedFormat(VkPhysicalDevice physicalDevice, const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features) {
        for (VkFormat format : candidates) {
            VkFormatProperties props;
            vkGetPhysicalDeviceFormatProperties(physicalDevice, format, &props);

            if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
                return format;
            }
            else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
                return format;
            }
        }

        throw std::runtime_error("failed to find supported format!");
    }

} // namespace Polaris