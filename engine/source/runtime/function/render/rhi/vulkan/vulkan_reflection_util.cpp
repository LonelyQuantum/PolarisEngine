#include "runtime/function/render/rhi/vulkan/vulkan_reflection_util.h"
#include <stdexcept>
#include <iostream>

namespace Polaris {

	const std::vector<const char*> VulkanReflectionUtil::physicalDeviceFeatures2Vector = {
			"robustBufferAccess", "fullDrawIndexUint32", "imageCubeArray",
			"independentBlend", "geometryShader", "tessellationShader",
			"sampleRateShading", "dualSrcBlend", "logicOp",
			"multiDrawIndirect", "drawIndirectFirstInstance", "depthClamp",
			"depthBiasClamp", "fillModeNonSolid", "depthBounds",
			"wideLines", "largePoints", "alphaToOne",
			"multiViewport", "samplerAnisotropy", "textureCompressionETC2",
			"textureCompressionASTC_LDR", "textureCompressionBC", "occlusionQueryPrecise",
			"pipelineStatisticsQuery", "vertexPipelineStoresAndAtomics", "fragmentStoresAndAtomics",
			"shaderTessellationAndGeometryPointSize", "shaderImageGatherExtended", "shaderStorageImageExtendedFormats",
			"shaderStorageImageMultisample", "shaderStorageImageReadWithoutFormat", "shaderStorageImageWriteWithoutFormat",
			"shaderUniformBufferArrayDynamicIndexing", "shaderSampledImageArrayDynamicIndexing", "shaderStorageBufferArrayDynamicIndexing",
			"shaderStorageImageArrayDynamicIndexing", "shaderClipDistance",  "shaderCullDistance",
			"shaderFloat64", "shaderInt64", "shaderInt16",
			"shaderResourceResidency", "shaderResourceMinLod", "sparseBinding",
			"sparseResidencyBuffer", "sparseResidencyImage2D", "sparseResidencyImage3D",
			"sparseResidency2Samples", "sparseResidency4Samples", "sparseResidency8Samples",
			"sparseResidency16Samples", "sparseResidencyAliased", "variableMultisampleRate",
			"inheritedQueries"
	};

	const std::map<const char*, int> VulkanReflectionUtil::physicalDeviceFeatures2Map = {
		{ "robustBufferAccess", 0 }, { "fullDrawIndexUint32", 1 }, { "imageCubeArray", 2 },
		{ "independentBlend", 3 }, { "geometryShader", 4 }, { "tessellationShader", 5 },
		{ "sampleRateShading", 6 }, { "dualSrcBlend", 7 }, { "logicOp", 8 },
		{ "multiDrawIndirect", 9 }, { "drawIndirectFirstInstance", 10 }, { "depthClamp", 11 },
		{ "depthBiasClamp", 12 }, { "fillModeNonSolid", 13 }, { "depthBounds", 14 },
		{ "wideLines", 15 }, { "largePoints", 16 }, { "alphaToOne", 17 },
		{ "multiViewport", 18 }, { "samplerAnisotropy", 19 }, { "textureCompressionETC2", 20 },
		{ "textureCompressionASTC_LDR", 21 }, { "textureCompressionBC", 22 }, { "occlusionQueryPrecise", 23 },
		{ "pipelineStatisticsQuery", 24 }, { "vertexPipelineStoresAndAtomics", 25 }, { "fragmentStoresAndAtomics", 26 },
		{ "shaderTessellationAndGeometryPointSize", 27 }, { "shaderImageGatherExtended", 28 }, { "shaderStorageImageExtendedFormats", 29 },
		{ "shaderStorageImageMultisample", 30 }, { "shaderStorageImageReadWithoutFormat", 31 }, { "shaderStorageImageWriteWithoutFormat", 32 },
		{ "shaderUniformBufferArrayDynamicIndexing", 33 }, { "shaderSampledImageArrayDynamicIndexing", 34 }, { "shaderStorageBufferArrayDynamicIndexing", 35 },
		{ "shaderStorageImageArrayDynamicIndexing", 36 }, { "shaderClipDistance", 37 }, { "shaderCullDistance", 38 },
		{ "shaderFloat64", 39 }, { "shaderInt64", 40 }, { "shaderInt16", 41 },
		{ "shaderResourceResidency", 42 }, { "shaderResourceMinLod", 43 }, { "sparseBinding", 44 },
		{ "sparseResidencyBuffer", 45 }, { "sparseResidencyImage2D", 46 }, { "sparseResidencyImage3D", 47 },
		{ "sparseResidency2Samples", 48 }, { "sparseResidency4Samples", 49 }, { "sparseResidency8Samples", 50 },
		{ "sparseResidency16Samples", 51 }, { "sparseResidencyAliased", 52 }, { "variableMultisampleRate", 53 },
		{ "inheritedQueries", 54 }
	};
	const std::vector<const char*> VulkanReflectionUtil::physicalDeviceVulkan11FeaturesVector{
			"storageBuffer16BitAccess", "uniformAndStorageBuffer16BitAccess", "storagePushConstant16",
			"storageInputOutput16", "multiview", "multiviewGeometryShader",
			"multiviewTessellationShader", "variablePointersStorageBuffer", "variablePointers",
			"protectedMemory", "samplerYcbcrConversion", "shaderDrawParameters"
	};
	const std::map<const char*, int> VulkanReflectionUtil::physicalDeviceVulkan11FeaturesMap = {
		{ "storageBuffer16BitAccess", 0 }, { "uniformAndStorageBuffer16BitAccess", 1 }, { "storagePushConstant16", 2 },
		{ "storageInputOutput16", 3 }, { "multiview", 4 }, { "multiviewGeometryShader", 5 },
		{ "multiviewTessellationShader", 6 }, { "variablePointersStorageBuffer", 7 }, { "variablePointers", 8 },
		{ "protectedMemory", 9 }, { "samplerYcbcrConversion", 10 }, { "shaderDrawParameters", 11 }
	};
	const std::vector<const char*> VulkanReflectionUtil::physicalDeviceVulkan12FeaturesVector{
		"samplerMirrorClampToEdge", "drawIndirectCount", "storageBuffer8BitAccess",
		"uniformAndStorageBuffer8BitAccess", "storagePushConstant8", "shaderBufferInt64Atomics",
		"shaderSharedInt64Atomics", "shaderFloat16", "shaderInt8",
		"descriptorIndexing", "shaderInputAttachmentArrayDynamicIndexing", "shaderUniformTexelBufferArrayDynamicIndexing",
		"shaderStorageTexelBufferArrayDynamicIndexing", "shaderUniformBufferArrayNonUniformIndexing", "shaderSampledImageArrayNonUniformIndexing",
		"shaderStorageBufferArrayNonUniformIndexing", "shaderStorageImageArrayNonUniformIndexing", "shaderInputAttachmentArrayNonUniformIndexing",
		"shaderUniformTexelBufferArrayNonUniformIndexing", "shaderStorageTexelBufferArrayNonUniformIndexing", "descriptorBindingUniformBufferUpdateAfterBind",
		"descriptorBindingSampledImageUpdateAfterBind", "descriptorBindingStorageImageUpdateAfterBind", "descriptorBindingStorageBufferUpdateAfterBind",
		"descriptorBindingUniformTexelBufferUpdateAfterBind", "descriptorBindingStorageTexelBufferUpdateAfterBind", "descriptorBindingUpdateUnusedWhilePending",
		"descriptorBindingPartiallyBound", "descriptorBindingVariableDescriptorCount", "runtimeDescriptorArray",
		"samplerFilterMinmax", "scalarBlockLayout", "imagelessFramebuffer",
		"uniformBufferStandardLayout", "shaderSubgroupExtendedTypes", "separateDepthStencilLayouts",
		"hostQueryReset", "timelineSemaphore", "bufferDeviceAddress",
		"bufferDeviceAddressCaptureReplay", "bufferDeviceAddressMultiDevice", "vulkanMemoryModel",
		"vulkanMemoryModelDeviceScope", "vulkanMemoryModelAvailabilityVisibilityChains", "shaderOutputViewportIndex",
		"shaderOutputLayer", "subgroupBroadcastDynamicId"
	};
	const std::map<const char*, int> VulkanReflectionUtil::physicalDeviceVulkan12FeaturesMap = {
		{ "samplerMirrorClampToEdge", 0 }, { "drawIndirectCount", 1 }, { "storageBuffer8BitAccess", 2 },
		{ "uniformAndStorageBuffer8BitAccess", 3 }, { "storagePushConstant8", 4 }, { "shaderBufferInt64Atomics", 5 },
		{ "shaderSharedInt64Atomics", 6 }, { "shaderFloat16", 7 }, { "shaderInt8", 8 },
		{ "descriptorIndexing", 9 }, { "shaderInputAttachmentArrayDynamicIndexing", 10 }, { "shaderUniformTexelBufferArrayDynamicIndexing", 11 },
		{ "shaderStorageTexelBufferArrayDynamicIndexing", 12 }, { "shaderUniformBufferArrayNonUniformIndexing", 13 }, { "shaderSampledImageArrayNonUniformIndexing", 14 },
		{ "shaderStorageBufferArrayNonUniformIndexing", 15 }, { "shaderStorageImageArrayNonUniformIndexing", 16 }, { "shaderInputAttachmentArrayNonUniformIndexing", 17 },
		{ "shaderUniformTexelBufferArrayNonUniformIndexing", 18 }, { "shaderStorageTexelBufferArrayNonUniformIndexing", 19 }, { "descriptorBindingUniformBufferUpdateAfterBind", 20 },
		{ "descriptorBindingSampledImageUpdateAfterBind", 21 }, { "descriptorBindingStorageImageUpdateAfterBind", 22 }, { "descriptorBindingStorageBufferUpdateAfterBind", 23 },
		{ "descriptorBindingUniformTexelBufferUpdateAfterBind", 24 }, { "descriptorBindingStorageTexelBufferUpdateAfterBind", 25 }, { "descriptorBindingUpdateUnusedWhilePending", 26 },
		{ "descriptorBindingPartiallyBound", 27 }, { "descriptorBindingVariableDescriptorCount", 28 }, { "runtimeDescriptorArray", 29 },
		{ "samplerFilterMinmax", 30 }, { "scalarBlockLayout", 31 }, { "imagelessFramebuffer", 32 },
		{ "uniformBufferStandardLayout", 33 }, { "shaderSubgroupExtendedTypes", 34 }, { "separateDepthStencilLayouts", 35 },
		{ "hostQueryReset", 36 }, { "timelineSemaphore", 37 }, { "bufferDeviceAddress", 38 },
		{ "bufferDeviceAddressCaptureReplay", 39 }, { "bufferDeviceAddressMultiDevice", 40 }, { "vulkanMemoryModel", 41 },
		{ "vulkanMemoryModelDeviceScope", 42 }, { "vulkanMemoryModelAvailabilityVisibilityChains", 43 }, { "shaderOutputViewportIndex", 44 },
		{ "shaderOutputLayer", 45 }, { "subgroupBroadcastDynamicId", 46 }
	};
	const std::vector<const char*> VulkanReflectionUtil::physicalDeviceAccelerationStructureFeaturesKHRVector{
		"accelerationStructure", "accelerationStructureCaptureReplay", "accelerationStructureIndirectBuild",
		"accelerationStructureHostCommands", "descriptorBindingAccelerationStructureUpdateAfterBind"
	};
	const std::map<const char*, int> VulkanReflectionUtil::physicalDeviceAccelerationStructureFeaturesKHRMap = {
		{ "accelerationStructure", 0 }, { "accelerationStructureCaptureReplay", 1 }, { "accelerationStructureIndirectBuild", 2 },
		{ "accelerationStructureHostCommands", 3 }, { "descriptorBindingAccelerationStructureUpdateAfterBind", 4 }
	};
	const std::vector<const char*> VulkanReflectionUtil::physicalDeviceRayTracingPipelineFeaturesKHRVector{
		"rayTracingPipeline", "rayTracingPipelineShaderGroupHandleCaptureReplay", "rayTracingPipelineShaderGroupHandleCaptureReplayMixed",
		"rayTracingPipelineTraceRaysIndirect", "rayTraversalPrimitiveCulling"
	};
	const std::map<const char*, int> VulkanReflectionUtil::physicalDeviceRayTracingPipelineFeaturesKHRMap = {
		{ "rayTracingPipeline", 0 }, { "rayTracingPipelineShaderGroupHandleCaptureReplay", 1 }, { "rayTracingPipelineShaderGroupHandleCaptureReplayMixed", 2 },
		{ "rayTracingPipelineTraceRaysIndirect", 3 }, { "rayTraversalPrimitiveCulling", 4 }
	};

	std::string VulkanReflectionUtil::getVkBool32StructName(void* pStructFeatures) {
		VulkanStructCommon* features = reinterpret_cast<VulkanStructCommon*>(pStructFeatures);
		VkStructureType sType = features->sType;
		std::string structName;
		if (sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2) {
			structName = "VkPhysicalDeviceFeatures2";
		}
		else if (sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES) {
			structName = "VkPhysicalDeviceVulkan11Features";
		}
		else if (sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES) {
			structName = "VkPhysicalDeviceVulkan12Features";
		}
		else if (sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR) {
			structName = "VkPhysicalDeviceAccelerationStructureFeaturesKHR";
		}
		else if (sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR) {
			structName = "VkPhysicalDeviceRayTracingPipelineFeaturesKHR";
		}
		else
		{
			std::cout << "No name" << std::endl;
		}
		return structName;
	}

	std::vector<const char*> VulkanReflectionUtil::getVkBool32StructVector(void* pStructFeatures) {
		VulkanStructCommon* features = reinterpret_cast<VulkanStructCommon*>(pStructFeatures);
		VkStructureType sType = features->sType;
		std::vector<const char*> structVector;
		if (sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2) {
			structVector = physicalDeviceFeatures2Vector;
		}
		else if (sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES) {
			structVector = physicalDeviceVulkan11FeaturesVector;
		}
		else if (sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES) {
			structVector = physicalDeviceVulkan12FeaturesVector;
		}
		else if (sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR) {
			structVector = physicalDeviceAccelerationStructureFeaturesKHRVector;
		}
		else if (sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR) {
			structVector = physicalDeviceRayTracingPipelineFeaturesKHRVector;
		}
		return structVector;
	}

	std::map<const char*, int> VulkanReflectionUtil::getVkBool32StructMap(void* pStructFeatures) {
		VulkanStructCommon* features = reinterpret_cast<VulkanStructCommon*>(pStructFeatures);
		VkStructureType sType = features->sType;
		std::map<const char*, int> structMap;
		if (sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2) {
			structMap = physicalDeviceFeatures2Map;
		}
		else if (sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES) {
			structMap = physicalDeviceVulkan11FeaturesMap;
		}
		else if (sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES) {
			structMap = physicalDeviceVulkan12FeaturesMap;
		}
		else if (sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR) {
			structMap = physicalDeviceAccelerationStructureFeaturesKHRMap;
		}
		else if (sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR) {
			structMap = physicalDeviceRayTracingPipelineFeaturesKHRMap;
		}
		return structMap;
	}

	VkBool32 VulkanReflectionUtil::getVkBool32StructValue(void* pStructFeatures, const char* fieldName) {
		VulkanStructCommon* features = reinterpret_cast<VulkanStructCommon*>(pStructFeatures);
		VkStructureType sType = features->sType;
		VkBool32 structValue;
		if (sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2) {
			structValue = getVkBool32StructValue(reinterpret_cast<VkPhysicalDeviceFeatures2*>(features), fieldName);
		}
		else if (sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES) {
			structValue = getVkBool32StructValue(reinterpret_cast<VkPhysicalDeviceVulkan11Features*>(features), fieldName);
		}
		else if (sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES) {
			structValue = getVkBool32StructValue(reinterpret_cast<VkPhysicalDeviceVulkan12Features*>(features), fieldName);
		}
		else if (sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR) {
			structValue = getVkBool32StructValue(reinterpret_cast<VkPhysicalDeviceAccelerationStructureFeaturesKHR*>(features), fieldName);
		}
		else if (sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR) {
			structValue = getVkBool32StructValue(reinterpret_cast<VkPhysicalDeviceRayTracingPipelineFeaturesKHR*>(features), fieldName);
		}
		else throw std::runtime_error("Structure type " + std::to_string(sType) + "not supported");
		return structValue;
	}

	std::vector<VkBool32> VulkanReflectionUtil::getVkBool32StructValues(void* pStructFeatures) {
		VulkanStructCommon* features = reinterpret_cast<VulkanStructCommon*>(pStructFeatures);
		VkStructureType sType = features->sType;
		std::vector<VkBool32> structValues;
		if (sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2) {
			structValues = getVkBool32StructValues(reinterpret_cast<VkPhysicalDeviceFeatures2*>(features));
		}
		else if (sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES) {
			structValues = getVkBool32StructValues(reinterpret_cast<VkPhysicalDeviceVulkan11Features*>(features));
		}
		else if (sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES) {
			structValues = getVkBool32StructValues(reinterpret_cast<VkPhysicalDeviceVulkan12Features*>(features));
		}
		else if (sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR) {
			structValues = getVkBool32StructValues(reinterpret_cast<VkPhysicalDeviceAccelerationStructureFeaturesKHR*>(features));
		}
		else if (sType == VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR) {
			structValues = getVkBool32StructValues(reinterpret_cast<VkPhysicalDeviceRayTracingPipelineFeaturesKHR*>(features));
		}
		return structValues;
	}

	VkBool32 VulkanReflectionUtil::getVkBool32StructValue(VkPhysicalDeviceFeatures vkStruct, const char* fieldName) {
		if (!strcmp(fieldName, "robustBufferAccess")) return vkStruct.robustBufferAccess;
		else if (!strcmp(fieldName, "fullDrawIndexUint32")) return vkStruct.fullDrawIndexUint32;
		else if (!strcmp(fieldName, "imageCubeArray")) return vkStruct.imageCubeArray;
		else if (!strcmp(fieldName, "independentBlend")) return vkStruct.independentBlend;
		else if (!strcmp(fieldName, "geometryShader")) return vkStruct.geometryShader;
		else if (!strcmp(fieldName, "tessellationShader")) return vkStruct.tessellationShader;
		else if (!strcmp(fieldName, "sampleRateShading")) return vkStruct.sampleRateShading;
		else if (!strcmp(fieldName, "dualSrcBlend")) return vkStruct.dualSrcBlend;
		else if (!strcmp(fieldName, "logicOp")) return vkStruct.logicOp;
		else if (!strcmp(fieldName, "multiDrawIndirect")) return vkStruct.multiDrawIndirect;
		else if (!strcmp(fieldName, "drawIndirectFirstInstance")) return vkStruct.drawIndirectFirstInstance;
		else if (!strcmp(fieldName, "depthClamp")) return vkStruct.depthClamp;
		else if (!strcmp(fieldName, "depthBiasClamp")) return vkStruct.depthBiasClamp;
		else if (!strcmp(fieldName, "fillModeNonSolid")) return vkStruct.fillModeNonSolid;
		else if (!strcmp(fieldName, "depthBounds")) return vkStruct.depthBounds;
		else if (!strcmp(fieldName, "wideLines")) return vkStruct.wideLines;
		else if (!strcmp(fieldName, "largePoints")) return vkStruct.largePoints;
		else if (!strcmp(fieldName, "alphaToOne")) return vkStruct.alphaToOne;
		else if (!strcmp(fieldName, "multiViewport")) return vkStruct.multiViewport;
		else if (!strcmp(fieldName, "samplerAnisotropy")) return vkStruct.samplerAnisotropy;
		else if (!strcmp(fieldName, "textureCompressionETC2")) return vkStruct.textureCompressionETC2;
		else if (!strcmp(fieldName, "textureCompressionASTC_LDR")) return vkStruct.textureCompressionASTC_LDR;
		else if (!strcmp(fieldName, "textureCompressionBC")) return vkStruct.textureCompressionBC;
		else if (!strcmp(fieldName, "occlusionQueryPrecise")) return vkStruct.occlusionQueryPrecise;
		else if (!strcmp(fieldName, "pipelineStatisticsQuery")) return vkStruct.pipelineStatisticsQuery;
		else if (!strcmp(fieldName, "vertexPipelineStoresAndAtomics")) return vkStruct.vertexPipelineStoresAndAtomics;
		else if (!strcmp(fieldName, "fragmentStoresAndAtomics")) return vkStruct.fragmentStoresAndAtomics;
		else if (!strcmp(fieldName, "shaderTessellationAndGeometryPointSize")) return vkStruct.shaderTessellationAndGeometryPointSize;
		else if (!strcmp(fieldName, "shaderImageGatherExtended")) return vkStruct.shaderImageGatherExtended;
		else if (!strcmp(fieldName, "shaderStorageImageExtendedFormats")) return vkStruct.shaderStorageImageExtendedFormats;
		else if (!strcmp(fieldName, "shaderStorageImageMultisample")) return vkStruct.shaderStorageImageMultisample;
		else if (!strcmp(fieldName, "shaderStorageImageReadWithoutFormat")) return vkStruct.shaderStorageImageReadWithoutFormat;
		else if (!strcmp(fieldName, "shaderStorageImageWriteWithoutFormat")) return vkStruct.shaderStorageImageWriteWithoutFormat;
		else if (!strcmp(fieldName, "shaderUniformBufferArrayDynamicIndexing")) return vkStruct.shaderUniformBufferArrayDynamicIndexing;
		else if (!strcmp(fieldName, "shaderSampledImageArrayDynamicIndexing")) return vkStruct.shaderSampledImageArrayDynamicIndexing;
		else if (!strcmp(fieldName, "shaderStorageBufferArrayDynamicIndexing")) return vkStruct.shaderStorageBufferArrayDynamicIndexing;
		else if (!strcmp(fieldName, "shaderStorageImageArrayDynamicIndexing")) return vkStruct.shaderStorageImageArrayDynamicIndexing;
		else if (!strcmp(fieldName, "shaderClipDistance")) return vkStruct.shaderClipDistance;
		else if (!strcmp(fieldName, "shaderCullDistance")) return vkStruct.shaderCullDistance;
		else if (!strcmp(fieldName, "shaderFloat64")) return vkStruct.shaderFloat64;
		else if (!strcmp(fieldName, "shaderInt64")) return vkStruct.shaderInt64;
		else if (!strcmp(fieldName, "shaderInt16")) return vkStruct.shaderInt16;
		else if (!strcmp(fieldName, "shaderResourceResidency")) return vkStruct.shaderResourceResidency;
		else if (!strcmp(fieldName, "shaderResourceMinLod")) return vkStruct.shaderResourceMinLod;
		else if (!strcmp(fieldName, "sparseBinding")) return vkStruct.sparseBinding;
		else if (!strcmp(fieldName, "sparseResidencyBuffer")) return vkStruct.sparseResidencyBuffer;
		else if (!strcmp(fieldName, "sparseResidencyImage2D")) return vkStruct.sparseResidencyImage2D;
		else if (!strcmp(fieldName, "sparseResidencyImage3D")) return vkStruct.sparseResidencyImage3D;
		else if (!strcmp(fieldName, "sparseResidency2Samples")) return vkStruct.sparseResidency2Samples;
		else if (!strcmp(fieldName, "sparseResidency4Samples")) return vkStruct.sparseResidency4Samples;
		else if (!strcmp(fieldName, "sparseResidency8Samples")) return vkStruct.sparseResidency8Samples;
		else if (!strcmp(fieldName, "sparseResidency16Samples")) return vkStruct.sparseResidency16Samples;
		else if (!strcmp(fieldName, "sparseResidencyAliased")) return vkStruct.sparseResidencyAliased;
		else if (!strcmp(fieldName, "variableMultisampleRate")) return vkStruct.variableMultisampleRate;
		else if (!strcmp(fieldName, "inheritedQueries")) return vkStruct.inheritedQueries;
		else throw std::runtime_error("The struct does not have field " + std::string(fieldName));
	}

	VkBool32 VulkanReflectionUtil::getVkBool32StructValue(VkPhysicalDeviceFeatures2* pVkStruct, const char* fieldName) {
		return getVkBool32StructValue(pVkStruct->features, fieldName);
	}

	VkBool32 VulkanReflectionUtil::getVkBool32StructValue(VkPhysicalDeviceVulkan11Features* pVkStruct, const char* fieldName) {
		if (!strcmp(fieldName, "storageBuffer16BitAccess")) return pVkStruct->storageBuffer16BitAccess;
		else if (!strcmp(fieldName, "uniformAndStorageBuffer16BitAccess")) return pVkStruct->uniformAndStorageBuffer16BitAccess;
		else if (!strcmp(fieldName, "storagePushConstant16")) return pVkStruct->storagePushConstant16;
		else if (!strcmp(fieldName, "storageInputOutput16")) return pVkStruct->storageInputOutput16;
		else if (!strcmp(fieldName, "multiview")) return pVkStruct->multiview;
		else if (!strcmp(fieldName, "multiviewGeometryShader")) return pVkStruct->multiviewGeometryShader;
		else if (!strcmp(fieldName, "multiviewTessellationShader")) return pVkStruct->multiviewTessellationShader;
		else if (!strcmp(fieldName, "variablePointersStorageBuffer")) return pVkStruct->variablePointersStorageBuffer;
		else if (!strcmp(fieldName, "variablePointers")) return pVkStruct->variablePointers;
		else if (!strcmp(fieldName, "protectedMemory")) return pVkStruct->protectedMemory;
		else if (!strcmp(fieldName, "samplerYcbcrConversion")) return pVkStruct->samplerYcbcrConversion;
		else if (!strcmp(fieldName, "shaderDrawParameters")) return pVkStruct->shaderDrawParameters;
		else throw std::runtime_error("The struct does not have field " + std::string(fieldName));
	}

	VkBool32 VulkanReflectionUtil::getVkBool32StructValue(VkPhysicalDeviceVulkan12Features* pVkStruct, const char* fieldName) {
		if (!strcmp(fieldName, "samplerMirrorClampToEdge")) return pVkStruct->samplerMirrorClampToEdge;
		else if (!strcmp(fieldName, "drawIndirectCount")) return pVkStruct->drawIndirectCount;
		else if (!strcmp(fieldName, "storageBuffer8BitAccess")) return pVkStruct->storageBuffer8BitAccess;
		else if (!strcmp(fieldName, "uniformAndStorageBuffer8BitAccess")) return pVkStruct->uniformAndStorageBuffer8BitAccess;
		else if (!strcmp(fieldName, "storagePushConstant8")) return pVkStruct->storagePushConstant8;
		else if (!strcmp(fieldName, "shaderBufferInt64Atomics")) return pVkStruct->shaderBufferInt64Atomics;
		else if (!strcmp(fieldName, "shaderSharedInt64Atomics")) return pVkStruct->shaderSharedInt64Atomics;
		else if (!strcmp(fieldName, "shaderFloat16")) return pVkStruct->shaderFloat16;
		else if (!strcmp(fieldName, "shaderInt8")) return pVkStruct->shaderInt8;
		else if (!strcmp(fieldName, "descriptorIndexing")) return pVkStruct->descriptorIndexing;
		else if (!strcmp(fieldName, "shaderInputAttachmentArrayDynamicIndexing")) return pVkStruct->shaderInputAttachmentArrayDynamicIndexing;
		else if (!strcmp(fieldName, "shaderUniformTexelBufferArrayDynamicIndexing")) return pVkStruct->shaderUniformTexelBufferArrayDynamicIndexing;
		else if (!strcmp(fieldName, "shaderStorageTexelBufferArrayDynamicIndexing")) return pVkStruct->shaderStorageTexelBufferArrayDynamicIndexing;
		else if (!strcmp(fieldName, "shaderUniformBufferArrayNonUniformIndexing")) return pVkStruct->shaderUniformBufferArrayNonUniformIndexing;
		else if (!strcmp(fieldName, "shaderSampledImageArrayNonUniformIndexing")) return pVkStruct->shaderSampledImageArrayNonUniformIndexing;
		else if (!strcmp(fieldName, "shaderStorageBufferArrayNonUniformIndexing")) return pVkStruct->shaderStorageBufferArrayNonUniformIndexing;
		else if (!strcmp(fieldName, "shaderStorageImageArrayNonUniformIndexing")) return pVkStruct->shaderStorageImageArrayNonUniformIndexing;
		else if (!strcmp(fieldName, "shaderInputAttachmentArrayNonUniformIndexing")) return pVkStruct->shaderInputAttachmentArrayNonUniformIndexing;
		else if (!strcmp(fieldName, "shaderUniformTexelBufferArrayNonUniformIndexing")) return pVkStruct->shaderUniformTexelBufferArrayNonUniformIndexing;
		else if (!strcmp(fieldName, "shaderStorageTexelBufferArrayNonUniformIndexing")) return pVkStruct->shaderStorageTexelBufferArrayNonUniformIndexing;
		else if (!strcmp(fieldName, "descriptorBindingUniformBufferUpdateAfterBind")) return pVkStruct->descriptorBindingUniformBufferUpdateAfterBind;
		else if (!strcmp(fieldName, "descriptorBindingSampledImageUpdateAfterBind")) return pVkStruct->descriptorBindingSampledImageUpdateAfterBind;
		else if (!strcmp(fieldName, "descriptorBindingStorageImageUpdateAfterBind")) return pVkStruct->descriptorBindingStorageImageUpdateAfterBind;
		else if (!strcmp(fieldName, "descriptorBindingStorageBufferUpdateAfterBind")) return pVkStruct->descriptorBindingStorageBufferUpdateAfterBind;
		else if (!strcmp(fieldName, "descriptorBindingUniformTexelBufferUpdateAfterBind")) return pVkStruct->descriptorBindingUniformTexelBufferUpdateAfterBind;
		else if (!strcmp(fieldName, "descriptorBindingStorageTexelBufferUpdateAfterBind")) return pVkStruct->descriptorBindingStorageTexelBufferUpdateAfterBind;
		else if (!strcmp(fieldName, "descriptorBindingUpdateUnusedWhilePending")) return pVkStruct->descriptorBindingUpdateUnusedWhilePending;
		else if (!strcmp(fieldName, "descriptorBindingPartiallyBound")) return pVkStruct->descriptorBindingPartiallyBound;
		else if (!strcmp(fieldName, "descriptorBindingVariableDescriptorCount")) return pVkStruct->descriptorBindingVariableDescriptorCount;
		else if (!strcmp(fieldName, "runtimeDescriptorArray")) return pVkStruct->runtimeDescriptorArray;
		else if (!strcmp(fieldName, "samplerFilterMinmax")) return pVkStruct->samplerFilterMinmax;
		else if (!strcmp(fieldName, "scalarBlockLayout")) return pVkStruct->scalarBlockLayout;
		else if (!strcmp(fieldName, "imagelessFramebuffer")) return pVkStruct->imagelessFramebuffer;
		else if (!strcmp(fieldName, "uniformBufferStandardLayout")) return pVkStruct->uniformBufferStandardLayout;
		else if (!strcmp(fieldName, "shaderSubgroupExtendedTypes")) return pVkStruct->shaderSubgroupExtendedTypes;
		else if (!strcmp(fieldName, "separateDepthStencilLayouts")) return pVkStruct->separateDepthStencilLayouts;
		else if (!strcmp(fieldName, "hostQueryReset")) return pVkStruct->hostQueryReset;
		else if (!strcmp(fieldName, "timelineSemaphore")) return pVkStruct->timelineSemaphore;
		else if (!strcmp(fieldName, "bufferDeviceAddress")) return pVkStruct->bufferDeviceAddress;
		else if (!strcmp(fieldName, "bufferDeviceAddressCaptureReplay")) return pVkStruct->bufferDeviceAddressCaptureReplay;
		else if (!strcmp(fieldName, "bufferDeviceAddressMultiDevice")) return pVkStruct->bufferDeviceAddressMultiDevice;
		else if (!strcmp(fieldName, "vulkanMemoryModel")) return pVkStruct->vulkanMemoryModel;
		else if (!strcmp(fieldName, "vulkanMemoryModelDeviceScope")) return pVkStruct->vulkanMemoryModelDeviceScope;
		else if (!strcmp(fieldName, "vulkanMemoryModelAvailabilityVisibilityChains")) return pVkStruct->vulkanMemoryModelAvailabilityVisibilityChains;
		else if (!strcmp(fieldName, "shaderOutputViewportIndex")) return pVkStruct->shaderOutputViewportIndex;
		else if (!strcmp(fieldName, "shaderOutputLayer")) return pVkStruct->shaderOutputLayer;
		else if (!strcmp(fieldName, "subgroupBroadcastDynamicId")) return pVkStruct->subgroupBroadcastDynamicId;
		else throw std::runtime_error("The struct does not have field " + std::string(fieldName));
	}

	VkBool32 VulkanReflectionUtil::getVkBool32StructValue(VkPhysicalDeviceAccelerationStructureFeaturesKHR* pVkStruct, const char* fieldName) {
		if (!strcmp(fieldName, "accelerationStructure")) return pVkStruct->accelerationStructure;
		else if (!strcmp(fieldName, "accelerationStructureCaptureReplay")) return pVkStruct->accelerationStructureCaptureReplay;
		else if (!strcmp(fieldName, "accelerationStructureIndirectBuild")) return pVkStruct->accelerationStructureIndirectBuild;
		else if (!strcmp(fieldName, "accelerationStructureHostCommands")) return pVkStruct->accelerationStructureHostCommands;
		else if (!strcmp(fieldName, "descriptorBindingAccelerationStructureUpdateAfterBind")) return pVkStruct->descriptorBindingAccelerationStructureUpdateAfterBind;
		else throw std::runtime_error("The struct does not have field " + std::string(fieldName));
	}

	VkBool32 VulkanReflectionUtil::getVkBool32StructValue(VkPhysicalDeviceRayTracingPipelineFeaturesKHR* pVkStruct, const char* fieldName) {
		if (!strcmp(fieldName, "rayTracingPipeline")) return pVkStruct->rayTracingPipeline;
		else if (!strcmp(fieldName, "rayTracingPipelineShaderGroupHandleCaptureReplay")) return pVkStruct->rayTracingPipelineShaderGroupHandleCaptureReplay;
		else if (!strcmp(fieldName, "rayTracingPipelineShaderGroupHandleCaptureReplayMixed")) return pVkStruct->rayTracingPipelineShaderGroupHandleCaptureReplayMixed;
		else if (!strcmp(fieldName, "rayTracingPipelineTraceRaysIndirect")) return pVkStruct->rayTracingPipelineTraceRaysIndirect;
		else if (!strcmp(fieldName, "rayTraversalPrimitiveCulling")) return pVkStruct->rayTraversalPrimitiveCulling;
		else throw std::runtime_error("The struct does not have field " + std::string(fieldName));
	}

	std::vector<VkBool32> VulkanReflectionUtil::getVkBool32StructValues(VkPhysicalDeviceFeatures2* pVkStruct) {
		return getVkBool32StructValues(pVkStruct->features);
	}

	std::vector<VkBool32> VulkanReflectionUtil::getVkBool32StructValues(VkPhysicalDeviceFeatures pVkStruct) {
		std::vector<VkBool32> v{
		pVkStruct.robustBufferAccess, pVkStruct.fullDrawIndexUint32, pVkStruct.imageCubeArray,
		pVkStruct.independentBlend, pVkStruct.geometryShader, pVkStruct.tessellationShader,
		pVkStruct.sampleRateShading, pVkStruct.dualSrcBlend, pVkStruct.logicOp,
		pVkStruct.multiDrawIndirect, pVkStruct.drawIndirectFirstInstance, pVkStruct.depthClamp,
		pVkStruct.depthBiasClamp, pVkStruct.fillModeNonSolid, pVkStruct.depthBounds,
		pVkStruct.wideLines, pVkStruct.largePoints, pVkStruct.alphaToOne,
		pVkStruct.multiViewport, pVkStruct.samplerAnisotropy, pVkStruct.textureCompressionETC2,
		pVkStruct.textureCompressionASTC_LDR, pVkStruct.textureCompressionBC, pVkStruct.occlusionQueryPrecise,
		pVkStruct.pipelineStatisticsQuery, pVkStruct.vertexPipelineStoresAndAtomics, pVkStruct.fragmentStoresAndAtomics,
		pVkStruct.shaderTessellationAndGeometryPointSize, pVkStruct.shaderImageGatherExtended, pVkStruct.shaderStorageImageExtendedFormats,
		pVkStruct.shaderStorageImageMultisample, pVkStruct.shaderStorageImageReadWithoutFormat, pVkStruct.shaderStorageImageWriteWithoutFormat,
		pVkStruct.shaderUniformBufferArrayDynamicIndexing, pVkStruct.shaderSampledImageArrayDynamicIndexing, pVkStruct.shaderStorageBufferArrayDynamicIndexing,
		pVkStruct.shaderStorageImageArrayDynamicIndexing, pVkStruct.shaderClipDistance, pVkStruct.shaderCullDistance,
		pVkStruct.shaderFloat64, pVkStruct.shaderInt64, pVkStruct.shaderInt16,
		pVkStruct.shaderResourceResidency, pVkStruct.shaderResourceMinLod, pVkStruct.sparseBinding,
		pVkStruct.sparseResidencyBuffer, pVkStruct.sparseResidencyImage2D, pVkStruct.sparseResidencyImage3D,
		pVkStruct.sparseResidency2Samples, pVkStruct.sparseResidency4Samples, pVkStruct.sparseResidency8Samples,
		pVkStruct.sparseResidency16Samples, pVkStruct.sparseResidencyAliased, pVkStruct.variableMultisampleRate,
		pVkStruct.inheritedQueries };
		return v;
	}

	std::vector<VkBool32> VulkanReflectionUtil::getVkBool32StructValues(VkPhysicalDeviceVulkan11Features* pVkStruct) {
		std::vector<VkBool32> v{
			pVkStruct->storageBuffer16BitAccess, pVkStruct->uniformAndStorageBuffer16BitAccess, pVkStruct->storagePushConstant16,
			pVkStruct->storageInputOutput16, pVkStruct->multiview, pVkStruct->multiviewGeometryShader,
			pVkStruct->multiviewTessellationShader, pVkStruct->variablePointersStorageBuffer, pVkStruct->variablePointers,
			pVkStruct->protectedMemory, pVkStruct->samplerYcbcrConversion, pVkStruct->shaderDrawParameters,
		};
		return v;
	}

	std::vector<VkBool32> VulkanReflectionUtil::getVkBool32StructValues(VkPhysicalDeviceVulkan12Features* pVkStruct) {
		std::vector<VkBool32> v{
			pVkStruct->samplerMirrorClampToEdge, pVkStruct->drawIndirectCount, pVkStruct->storageBuffer8BitAccess,
			pVkStruct->uniformAndStorageBuffer8BitAccess, pVkStruct->storagePushConstant8, pVkStruct->shaderBufferInt64Atomics,
			pVkStruct->shaderSharedInt64Atomics, pVkStruct->shaderFloat16, pVkStruct->shaderInt8,
			pVkStruct->descriptorIndexing, pVkStruct->shaderInputAttachmentArrayDynamicIndexing, pVkStruct->shaderUniformTexelBufferArrayDynamicIndexing,
			pVkStruct->shaderStorageTexelBufferArrayDynamicIndexing, pVkStruct->shaderUniformBufferArrayNonUniformIndexing, pVkStruct->shaderSampledImageArrayNonUniformIndexing,
			pVkStruct->shaderStorageBufferArrayNonUniformIndexing, pVkStruct->shaderStorageImageArrayNonUniformIndexing, pVkStruct->shaderInputAttachmentArrayNonUniformIndexing,
			pVkStruct->shaderUniformTexelBufferArrayNonUniformIndexing, pVkStruct->shaderStorageTexelBufferArrayNonUniformIndexing, pVkStruct->descriptorBindingUniformBufferUpdateAfterBind,
			pVkStruct->descriptorBindingSampledImageUpdateAfterBind, pVkStruct->descriptorBindingStorageImageUpdateAfterBind, pVkStruct->descriptorBindingStorageBufferUpdateAfterBind,
			pVkStruct->descriptorBindingUniformTexelBufferUpdateAfterBind, pVkStruct->descriptorBindingStorageTexelBufferUpdateAfterBind, pVkStruct->descriptorBindingUpdateUnusedWhilePending,
			pVkStruct->descriptorBindingPartiallyBound, pVkStruct->descriptorBindingVariableDescriptorCount, pVkStruct->runtimeDescriptorArray,
			pVkStruct->samplerFilterMinmax, pVkStruct->scalarBlockLayout, pVkStruct->imagelessFramebuffer,
			pVkStruct->uniformBufferStandardLayout, pVkStruct->shaderSubgroupExtendedTypes, pVkStruct->separateDepthStencilLayouts,
			pVkStruct->hostQueryReset, pVkStruct->timelineSemaphore, pVkStruct->bufferDeviceAddress,
			pVkStruct->bufferDeviceAddressCaptureReplay, pVkStruct->bufferDeviceAddressMultiDevice, pVkStruct->vulkanMemoryModel,
			pVkStruct->vulkanMemoryModelDeviceScope, pVkStruct->vulkanMemoryModelAvailabilityVisibilityChains, pVkStruct->shaderOutputViewportIndex,
			pVkStruct->shaderOutputLayer, pVkStruct->subgroupBroadcastDynamicId
		};
		return v;
	}

	std::vector<VkBool32> VulkanReflectionUtil::getVkBool32StructValues(VkPhysicalDeviceAccelerationStructureFeaturesKHR* pVkStruct) {
		std::vector<VkBool32> v{
			pVkStruct->accelerationStructure, pVkStruct->accelerationStructureCaptureReplay, pVkStruct->accelerationStructureIndirectBuild,
			pVkStruct->accelerationStructureHostCommands, pVkStruct->descriptorBindingAccelerationStructureUpdateAfterBind
		};
		return v;
	}

	std::vector<VkBool32> VulkanReflectionUtil::getVkBool32StructValues(VkPhysicalDeviceRayTracingPipelineFeaturesKHR* pVkStruct) {
		std::vector<VkBool32> v{
			pVkStruct->rayTracingPipeline, pVkStruct->rayTracingPipelineShaderGroupHandleCaptureReplay, pVkStruct->rayTracingPipelineShaderGroupHandleCaptureReplayMixed,
			pVkStruct->rayTracingPipelineTraceRaysIndirect, pVkStruct->rayTraversalPrimitiveCulling
		};
		return v;
	}

} // namespace Polaris
