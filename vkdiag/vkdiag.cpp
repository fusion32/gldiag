// stdlib includes
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// sdl includes
#include <SDL.h>
#include <SDL_vulkan.h>

// vulkan includes
#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

// local includes
#include "scopeguard.h"

// a few string overloads to help outputting
std::string &operator<<(std::string &self, const std::string &rhs){
	self += rhs;
	return self;
}

std::string &operator<<(std::string &self, const char *rhs){
	self += rhs;
	return self;
}

std::string &operator<<(std::string &self, const char rhs){
	self += rhs;
	return self;
}

std::string &operator<<(std::string &self, size_t rhs){
	self += std::to_string(rhs);
	return self;
}

std::string &operator<<(std::string &self, uint32_t rhs){
	self += std::to_string(rhs);
	return self;
}

std::string &operator<<(std::string &self, int rhs){
	self += std::to_string(rhs);
	return self;
}

std::string &operator<<(std::string &self, bool rhs){
	self += rhs ? "true" : "false";
	return self;
}

// helper macros
#define DEF_VK_PROC(proc)	\
	PFN_##proc proc
#define GET_VK_INSTANCE_PROC(inst, proc)	\
	proc = (PFN_##proc)vkGetInstanceProcAddr(inst, #proc)

// output diag to buf
void vkdiag(std::string &buf){
	DEF_VK_PROC(vkGetInstanceProcAddr);
	DEF_VK_PROC(vkCreateInstance);
	DEF_VK_PROC(vkDestroyInstance);
	DEF_VK_PROC(vkEnumeratePhysicalDevices);
	DEF_VK_PROC(vkGetPhysicalDeviceProperties);
	DEF_VK_PROC(vkGetPhysicalDeviceMemoryProperties);
	DEF_VK_PROC(vkGetPhysicalDeviceQueueFamilyProperties);
	DEF_VK_PROC(vkGetPhysicalDeviceFeatures);

	if(SDL_Init(SDL_INIT_VIDEO) != 0){
		std::cout << "Failed to initialize SDL video system\n";
		std::cout << "ERROR: " << SDL_GetError() << '\n';
		return;
	}
	auto sdl_guard = make_scope_guard(SDL_Quit);

	if(SDL_Vulkan_LoadLibrary(NULL) != 0){
		std::cout << "No Vulkan driver found\n";
		std::cout << "ERROR: " << SDL_GetError() << '\n';
		return;
	}
	auto vklib_guard = make_scope_guard(SDL_Vulkan_UnloadLibrary);

	// load vkGetInstanceProcAddr
	vkGetInstanceProcAddr =
		(PFN_vkGetInstanceProcAddr)SDL_Vulkan_GetVkGetInstanceProcAddr();
	if(vkGetInstanceProcAddr == nullptr){
		std::cout << "Failed to load `vkGetInstanceProcAddr` proc\n";
		return;
	}

	// load vkCreateInstance
	GET_VK_INSTANCE_PROC(nullptr, vkCreateInstance);
	if(vkCreateInstance == nullptr){
		std::cout << "Failed to load `vkCreateInstance` proc\n";
		return;
	}

	// create vulkan instance
	VkResult res;
	VkInstance instance;
	VkInstanceCreateInfo instance_create_info = {
		VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,	// sType
		NULL,					// pNext
		0,					// flags
		NULL,					// pApplicationInfo
		0,					// enabledLayerCount
		NULL,					// ppEnabledLayerNames
		0,					// enabledExtensionCount
		NULL,					// ppEnabledExtensionNames
	};
	res = vkCreateInstance(&instance_create_info, nullptr, &instance);
	if(res != VK_SUCCESS){
		std::cout << "Failed to create Vulkan instance\n";
		return;
	}

	// load vkDestroyInstance
	GET_VK_INSTANCE_PROC(instance, vkDestroyInstance);
	if(vkDestroyInstance == nullptr){
		std::cout << "FATAL: failed to load `vkDestroyInstance` proc address";
		std::terminate();
	}
	auto vkinstance_guard = make_scope_guard(
		[&](void){ vkDestroyInstance(instance, nullptr); });

	// load other vulkan functions
	GET_VK_INSTANCE_PROC(instance, vkEnumeratePhysicalDevices);
	GET_VK_INSTANCE_PROC(instance, vkGetPhysicalDeviceProperties);
	GET_VK_INSTANCE_PROC(instance, vkGetPhysicalDeviceMemoryProperties);
	GET_VK_INSTANCE_PROC(instance, vkGetPhysicalDeviceQueueFamilyProperties);
	GET_VK_INSTANCE_PROC(instance, vkGetPhysicalDeviceFeatures);
	if(vkEnumeratePhysicalDevices == nullptr ||
			vkGetPhysicalDeviceProperties == nullptr ||
			vkGetPhysicalDeviceMemoryProperties == nullptr ||
			vkGetPhysicalDeviceQueueFamilyProperties == nullptr ||
			vkGetPhysicalDeviceFeatures == nullptr){
		std::cout << "Failed to load vulkan procs! Unable to proceed";
		return;
	}

	// retrieve system information
	uint32_t count;
	res = vkEnumeratePhysicalDevices(instance, &count, NULL);
	if(res != VK_SUCCESS){
		std::cout << "Failed to enumerate physical devices\n";
		return;
	}

	std::vector<VkPhysicalDevice> devices(count);
	res = vkEnumeratePhysicalDevices(instance, &count, devices.data());
	if(res != VK_SUCCESS){
		std::cout << "Failed to retrieve physical devices\n";
		return;
	}

	int j = 0;
	for(auto device : devices){
		buf << "Device #" << j++
			<< "\n=============================\n";

		// device properties
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(device, &properties);
		buf << "[Properties]"
			<< "\nAPI Version: "
				<< VK_VERSION_MAJOR(properties.apiVersion) << '.'
				<< VK_VERSION_MINOR(properties.apiVersion) << '.'
				<< VK_VERSION_PATCH(properties.apiVersion)
			<< "\nDriver Version: " << properties.driverVersion
			<< "\nVendor ID: " << properties.vendorID
			<< "\nDevice ID: " << properties.deviceID
			<< "\nDevice Type: " << properties.deviceType
			<< "\nDevice Name: " << properties.deviceName
			<< "\n\n";

		// device memory properties
		VkPhysicalDeviceMemoryProperties mem_properties;
		vkGetPhysicalDeviceMemoryProperties(device, &mem_properties);

		// memory heaps
		buf << "[Memory Heaps]";
		for(uint32_t k = 0; k < mem_properties.memoryHeapCount; k++){
			buf << "\nHeap #" << k
				<< ": size = " << mem_properties.memoryHeaps[k].size
				<< ", flags = " << mem_properties.memoryHeaps[k].flags;
		}
		buf << "\n\n";

		// memory types
		buf << "[Memory Types]";
		for(uint32_t k = 0; k < mem_properties.memoryTypeCount; k++){
			buf << "\nType #" << k
				<< ": propertyFlags = " << mem_properties.memoryTypes[k].propertyFlags
				<< ", heapIndex = " << mem_properties.memoryTypes[k].heapIndex;
		}
		buf << "\n\n";

		// device queue family properties
		vkGetPhysicalDeviceQueueFamilyProperties(device, &count, NULL);
		buf << "[Queue Family Properties]";
		if(count == 0){
			buf << "\nFailed to query device queue family count";
		}else{
			std::vector<VkQueueFamilyProperties> qf_properties(count);
			vkGetPhysicalDeviceQueueFamilyProperties(device,
					&count, qf_properties.data());

			int k = 0;
			for(auto prop : qf_properties){
				buf << "\nQueue Family #" << k++
					<< "\n* queueFlags: " << prop.queueFlags
					<< "\n* queueCount: " << prop.queueCount
					<< "\n* timestampValidBits: " << prop.timestampValidBits
					<< "\n* minImageTransferGranularity: "
						<< "width = " << prop.minImageTransferGranularity.width
						<< ", height = " << prop.minImageTransferGranularity.height
						<< ", depth = " << prop.minImageTransferGranularity.depth;
			}
		}
		buf << "\n\n";

		// device features
		VkPhysicalDeviceFeatures features;
		vkGetPhysicalDeviceFeatures(device, &features);
		buf << "[Features]"
			<< "\n* robustBufferAccess: " << features.robustBufferAccess
			<< "\n* fullDrawIndexUint32: " << features.fullDrawIndexUint32
			<< "\n* imageCubeArray: " << features.imageCubeArray
			<< "\n* independentBlend: " << features.independentBlend
			<< "\n* geometryShader: " << features.geometryShader
			<< "\n* tessellationShader: " << features.tessellationShader
			<< "\n* sampleRateShading: " << features.sampleRateShading
			<< "\n* dualSrcBlend: " << features.dualSrcBlend
			<< "\n* logicOp: " << features.logicOp
			<< "\n* multiDrawIndirect: " << features.multiDrawIndirect
			<< "\n* drawIndirectFirstInstance: " << features.drawIndirectFirstInstance
			<< "\n* depthClamp: " << features.depthClamp
			<< "\n* depthBiasClamp: " << features.depthBiasClamp
			<< "\n* fillModeNonSolid: " << features.fillModeNonSolid
			<< "\n* depthBounds: " << features.depthBounds
			<< "\n* wideLines: " << features.wideLines
			<< "\n* largePoints: " << features.largePoints
			<< "\n* alphaToOne: " << features.alphaToOne
			<< "\n* multiViewport: " << features.multiViewport
			<< "\n* samplerAnisotropy: " << features.samplerAnisotropy
			<< "\n* textureCompressionETC2: " << features.textureCompressionETC2
			<< "\n* textureCompressionASTC_LDR: " << features.textureCompressionASTC_LDR
			<< "\n* textureCompressionBC: " << features.textureCompressionBC
			<< "\n* occlusionQueryPrecise: " << features.occlusionQueryPrecise
			<< "\n* pipelineStatisticsQuery: " << features.pipelineStatisticsQuery
			<< "\n* vertexPipelineStoresAndAtomics: " << features.vertexPipelineStoresAndAtomics
			<< "\n* fragmentStoresAndAtomics: " << features.fragmentStoresAndAtomics
			<< "\n* shaderTessellationAndGeometryPointSize: " << features.shaderTessellationAndGeometryPointSize
			<< "\n* shaderImageGatherExtended: " << features.shaderImageGatherExtended
			<< "\n* shaderStorageImageExtendedFormats: " << features.shaderStorageImageExtendedFormats
			<< "\n* shaderStorageImageMultisample: " << features.shaderStorageImageMultisample
			<< "\n* shaderStorageImageReadWithoutFormat: " << features.shaderStorageImageReadWithoutFormat
			<< "\n* shaderStorageImageWriteWithoutFormat: " << features.shaderStorageImageWriteWithoutFormat
			<< "\n* shaderUniformBufferArrayDynamicIndexing: " << features.shaderUniformBufferArrayDynamicIndexing
			<< "\n* shaderSampledImageArrayDynamicIndexing: " << features.shaderSampledImageArrayDynamicIndexing
			<< "\n* shaderStorageBufferArrayDynamicIndexing: " << features.shaderStorageBufferArrayDynamicIndexing
			<< "\n* shaderStorageImageArrayDynamicIndexing: " << features.shaderStorageImageArrayDynamicIndexing
			<< "\n* shaderClipDistance: " << features.shaderClipDistance
			<< "\n* shaderCullDistance: " << features.shaderCullDistance
			<< "\n* shaderFloat64: " << features.shaderFloat64
			<< "\n* shaderInt64: " << features.shaderInt64
			<< "\n* shaderInt16: " << features.shaderInt16
			<< "\n* shaderResourceResidency: " << features.shaderResourceResidency
			<< "\n* shaderResourceMinLod: " << features.shaderResourceMinLod
			<< "\n* sparseBinding: " << features.sparseBinding
			<< "\n* sparseResidencyBuffer: " << features.sparseResidencyBuffer
			<< "\n* sparseResidencyImage2D: " << features.sparseResidencyImage2D
			<< "\n* sparseResidencyImage3D: " << features.sparseResidencyImage3D
			<< "\n* sparseResidency2Samples: " << features.sparseResidency2Samples
			<< "\n* sparseResidency4Samples: " << features.sparseResidency4Samples
			<< "\n* sparseResidency8Samples: " << features.sparseResidency8Samples
			<< "\n* sparseResidency16Samples: " << features.sparseResidency16Samples
			<< "\n* sparseResidencyAliased: " << features.sparseResidencyAliased
			<< "\n* variableMultisampleRate: " << features.variableMultisampleRate
			<< "\n* inheritedQueries: " << features.inheritedQueries
			<< "\n\n";
	}
}

int main(int argc, char **argv){
	std::ofstream f;
	std::string buffer;

	// this will speedup cout
	std::ios_base::sync_with_stdio(false);
	// 32KB should be enough
	buffer.reserve(32768);

	// collect data
	vkdiag(buffer);

	// write buffer to file
	f.open("vkdiag.txt", std::ios_base::out | std::ios_base::trunc);
	if(f.fail()){
		std::cout << "failed to open file for writing\n";
		return -1;
	}
	f << buffer;
	f.close();
	std::cout << "File 'vkdiag.txt' generated\n";
	std::cin.get();
	return 0;
}
