#pragma once

#include <vulkan/vulkan_core.h>
#include <vector>
#include <map>
#include <string>

namespace Polaris
{
    struct VulkanStructCommon
    {
        VkStructureType sType;
        void* pNext;
    };

    typedef VulkanStructCommon VulkanExtensionHeader;

    class VulkanReflectionUtil{
    public:
        // Physical device feature struct getters
        static std::string getVkBool32StructName(void* pStructFeatures);
        static std::vector<const char*> getVkBool32StructVector(void* pStructFeatures);
        static std::map<const char*, int> getVkBool32StructMap(void* pStructFeatures);
        static VkBool32 getVkBool32StructValue(void* pStructFeatures, const char* fieldName);
        static std::vector<VkBool32> getVkBool32StructValues(void* pStructFeatures);

        static VkBool32 getVkBool32StructValue(VkPhysicalDeviceFeatures vkStruct, const char* fieldName);
        static VkBool32 getVkBool32StructValue(VkPhysicalDeviceFeatures2* pVkStruct, const char* fieldName);
        static VkBool32 getVkBool32StructValue(VkPhysicalDeviceVulkan11Features* pVkStruct, const char* fieldName);
        static VkBool32 getVkBool32StructValue(VkPhysicalDeviceVulkan12Features* pVkStruct, const char* fieldName);
        static VkBool32 getVkBool32StructValue(VkPhysicalDeviceAccelerationStructureFeaturesKHR* pVkStruct, const char* fieldName);
        static VkBool32 getVkBool32StructValue(VkPhysicalDeviceRayTracingPipelineFeaturesKHR* pVkStruct, const char* fieldName);
        static std::vector<VkBool32> getVkBool32StructValues(VkPhysicalDeviceFeatures vkStruct);
        static std::vector<VkBool32> getVkBool32StructValues(VkPhysicalDeviceFeatures2* pVkStruct);
        static std::vector<VkBool32> getVkBool32StructValues(VkPhysicalDeviceVulkan11Features* pVkStruct);
        static std::vector<VkBool32> getVkBool32StructValues(VkPhysicalDeviceVulkan12Features* pVkStruct);
        static std::vector<VkBool32> getVkBool32StructValues(VkPhysicalDeviceAccelerationStructureFeaturesKHR* pVkStruct);
        static std::vector<VkBool32> getVkBool32StructValues(VkPhysicalDeviceRayTracingPipelineFeaturesKHR* pVkStruct);

        static const std::vector<const char*> physicalDeviceFeatures2Vector;
        static const std::map<const char*, int> physicalDeviceFeatures2Map;
        static const std::vector<const char*> physicalDeviceVulkan11FeaturesVector;
        static const std::map<const char*, int> physicalDeviceVulkan11FeaturesMap;
        static const std::vector<const char*> physicalDeviceVulkan12FeaturesVector;
        static const std::map<const char*, int> physicalDeviceVulkan12FeaturesMap;
        static const std::vector<const char*> physicalDeviceAccelerationStructureFeaturesKHRVector;
        static const std::map<const char*, int> physicalDeviceAccelerationStructureFeaturesKHRMap;
        static const std::vector<const char*> physicalDeviceRayTracingPipelineFeaturesKHRVector;
        static const std::map<const char*, int> physicalDeviceRayTracingPipelineFeaturesKHRMap;

    };
} // namespace Polaris