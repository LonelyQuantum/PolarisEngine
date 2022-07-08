#include "runtime/function/render/rhi/vulkan/vulkan_util.h"
#include "GLFW/glfw3.h"
#include <iostream>


namespace Polaris
{
    /*
    * Add instance extension names for glfw
    */
    void VulkanUtil::addGLFWRequiredExtensions(std::vector<const char*>& instanceExtensions)
    {
        uint32_t count{ 0 };
        const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&count);
        for (int extId = 0; extId < count; ++extId)
            instanceExtensions.push_back(glfwExtensions[extId]);
    }

    /*
    * Populate debug messenger according to whether it is debug mode
    */
    void VulkanUtil::populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo, 
                                                      PFN_vkDebugUtilsMessengerCallbackEXT debugCallback, 
                                                      bool debug)
    {
        createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
        if (debug)
        {
            createInfo.messageSeverity = 
                VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            createInfo.messageType = 
                VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        }
        else
        {
            createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
            createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
        }
        createInfo.pfnUserCallback = debugCallback;
    }

    /*
    * Create debug messenger for the Vulkan instance
    */
    VkResult VulkanUtil::createDebugUtilsMessengerEXT(VkInstance instance, 
                                                      const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
                                                      const VkAllocationCallbacks* pAllocator, 
                                                      VkDebugUtilsMessengerEXT* pDebugMessenger) 
    {
        auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
        if (func != nullptr) {
            return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
        }
        else {
            return VK_ERROR_EXTENSION_NOT_PRESENT;
        }
    }

    /*
    * Destroy the debug messenger for Vulkan instance
    */
    void VulkanUtil::destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
    {
        auto func =
            (PFN_vkDestroyDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
        if (func != nullptr)
        {
            func(instance, debugMessenger, pAllocator);
        }
    }

    /*
    * Plain debug callback that only outputs the message
    */
    VKAPI_ATTR VkBool32 VKAPI_CALL VulkanUtil::plainDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT instance,
                                                                  VkDebugUtilsMessageTypeFlagsEXT pCreateInfo,
                                                                  const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
                                                                  void* pUserData)
    {
        std::cerr << "validation layer: " << pCallbackData->pMessage << std::endl;
        return VK_FALSE;
    }

    uint32_t VulkanUtil::findMemoryType(VkPhysicalDevice      physical_device,
                                        uint32_t              type_filter,
                                        VkMemoryPropertyFlags properties_flag)
    {
        VkPhysicalDeviceMemoryProperties physical_device_memory_properties;
        vkGetPhysicalDeviceMemoryProperties(physical_device, &physical_device_memory_properties);
        for (uint32_t i = 0; i < physical_device_memory_properties.memoryTypeCount; i++)
        {
            if (type_filter & (1 << i) && (physical_device_memory_properties.memoryTypes[i].propertyFlags & properties_flag) == properties_flag)
            {
                return i;
            }
        }
        throw std::runtime_error("Failed to find memory type");
    }

    void VulkanUtil::createImage(VkPhysicalDevice      physical_device,
                                 VkDevice              device,
                                 uint32_t              image_width,
                                 uint32_t              image_height,
                                 VkFormat              format,
                                 VkImageTiling         image_tiling,
                                 VkImageUsageFlags     image_usage_flags,
                                 VkMemoryPropertyFlags memory_property_flags,
                                 VkImage& image,
                                 VkDeviceMemory& memory,
                                 VkImageCreateFlags    image_create_flags,
                                 uint32_t              array_layers,
                                 uint32_t              miplevels)
    {
        VkImageCreateInfo image_create_info{};
        image_create_info.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
        image_create_info.flags = image_create_flags;
        image_create_info.imageType = VK_IMAGE_TYPE_2D;
        image_create_info.extent.width = image_width;
        image_create_info.extent.height = image_height;
        image_create_info.extent.depth = 1;
        image_create_info.mipLevels = miplevels;
        image_create_info.arrayLayers = array_layers;
        image_create_info.format = format;
        image_create_info.tiling = image_tiling;
        image_create_info.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        image_create_info.usage = image_usage_flags;
        image_create_info.samples = VK_SAMPLE_COUNT_1_BIT;
        image_create_info.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

        if (vkCreateImage(device, &image_create_info, nullptr, &image) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create image!");
        }

        VkMemoryRequirements memRequirements;
        vkGetImageMemoryRequirements(device, image, &memRequirements);

        VkMemoryAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
        allocInfo.allocationSize = memRequirements.size;
        allocInfo.memoryTypeIndex = findMemoryType(physical_device, memRequirements.memoryTypeBits, memory_property_flags);

        if (vkAllocateMemory(device, &allocInfo, nullptr, &memory) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate image memory!");
        }

        vkBindImageMemory(device, image, memory, 0);
    }

    VkImageView VulkanUtil::createImageView(VkDevice           device,
                                            VkImage& image,
                                            VkFormat           format,
                                            VkImageAspectFlags image_aspect_flags,
                                            VkImageViewType    view_type,
                                            uint32_t           layout_count,
                                            uint32_t           miplevels)
    {
        VkImageViewCreateInfo viewInfo{};
        viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        viewInfo.image = image;
        viewInfo.viewType = view_type;
        viewInfo.format = format;
        viewInfo.subresourceRange.aspectMask = image_aspect_flags;
        viewInfo.subresourceRange.baseMipLevel = 0;
        viewInfo.subresourceRange.levelCount = miplevels;
        viewInfo.subresourceRange.baseArrayLayer = 0;
        viewInfo.subresourceRange.layerCount = layout_count;

        VkImageView imageView;
        if (vkCreateImageView(device, &viewInfo, nullptr, &imageView) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create image view!");
        }

        return imageView;
    }

} // namespace Polaris