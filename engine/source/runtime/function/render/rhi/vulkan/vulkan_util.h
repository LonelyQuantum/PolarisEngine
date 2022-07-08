#pragma once

#include <vulkan/vulkan.h>
#include <vector>


namespace Polaris
{
	class VulkanUtil
	{
	public:
		static void addGLFWRequiredExtensions(std::vector<const char*>& instanceExtensions);
		static void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo, 
													 PFN_vkDebugUtilsMessengerCallbackEXT debugCallback, 
													 bool debug = false);
		static VkResult createDebugUtilsMessengerEXT(VkInstance instance,
													 const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, 
													 const VkAllocationCallbacks* pAllocator, 
													 VkDebugUtilsMessengerEXT* pDebugMessenger);
		static void destroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);

		static VKAPI_ATTR VkBool32 VKAPI_CALL plainDebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT instance,
																 VkDebugUtilsMessageTypeFlagsEXT pCreateInfo,
																 const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
																 void* pUserData);
		static uint32_t findMemoryType(VkPhysicalDevice physical_device, uint32_t type_filter, VkMemoryPropertyFlags properties_flag);
		static void createImage(VkPhysicalDevice      physical_device,
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
								uint32_t              miplevels);
		static VkImageView createImageView(VkDevice           device,
										   VkImage& image,
										   VkFormat           format,
										   VkImageAspectFlags imageAspectFlags,
										   VkImageViewType    viewType,
										   uint32_t           layoutCount,
										   uint32_t           miplevels);
	};
	
} // namespace Polaris
