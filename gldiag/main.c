#define _CRT_SECURE_NO_WARNINGS

#include "glcorearb.h"
#include "wglext.h"

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>

#include <stdio.h>

#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <conio.h>

#define BUFFER_SIZE 16384
#define MAX_PHYSICAL_DEVICES 16
#define MAX_QUEUE_FAMILIES 16

#define CHECK_RESULT(cond, fmt, ...)		\
	if(!(cond)){							\
		printf(fmt, ##__VA_ARGS__);			\
		_getch();							\
		exit(-1);							\
	}

typedef struct buffer_t{
	char data[BUFFER_SIZE];
	uint32_t writePos;
} buffer_t;

void BUFFER_Printf(buffer_t *buffer, const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	buffer->writePos += vsnprintf(buffer->data + buffer->writePos,
		BUFFER_SIZE - buffer->writePos, fmt, ap);
	va_end(ap);
	CHECK_RESULT(buffer->writePos < BUFFER_SIZE, "<ERROR> Buffer overflow!\n");
}

GLAPI const GLubyte *APIENTRY glGetString (GLenum name);

void GL_Info(buffer_t *buffer)
{
	// create dummy window
	HINSTANCE hinstance;
	HWND hwnd;
	WNDCLASSEX wndClass;
	const char *className = "dummy window";

	hinstance = GetModuleHandle(NULL);
	memset(&wndClass, 0, sizeof(WNDCLASSEX));
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.lpfnWndProc = DefWindowProc;
	wndClass.hInstance = hinstance;
	wndClass.lpszClassName = className;
	CHECK_RESULT(RegisterClassEx(&wndClass) != 0, "[OpenGL]<ERROR> Failed to register dummy window class! (%lX)\n", GetLastError());

	RECT rc = {0, 0, 100, 100};
	AdjustWindowRect(&rc, 0, FALSE);
	hwnd = CreateWindowEx(0, className, className, 0, 0, 0,
		rc.right - rc.left, rc.bottom - rc.top, NULL, NULL, hinstance, NULL);
	CHECK_RESULT(hwnd != NULL, "[OpenGL]<ERROR> Failed to create dummy window! (%lX)\n", GetLastError());

	// create gl context
	HDC hdc;
	HGLRC glcontext;
	PIXELFORMATDESCRIPTOR pfd;
	int format;

	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;

	hdc = GetDC(hwnd);
	CHECK_RESULT(hdc != NULL, "[OpenGL]<ERROR> Failed to retrieve device context! (%lX)\n", GetLastError());
	format = ChoosePixelFormat(hdc, &pfd);
	SetPixelFormat(hdc, format, &pfd);
	glcontext = wglCreateContext(hdc);
	CHECK_RESULT(glcontext != NULL, "[OpenGL]<ERROR> Failed to create OpenGL context! (%lX)\n", GetLastError());
	CHECK_RESULT(wglMakeCurrent(hdc, glcontext) != FALSE, "[OpenGL]<ERROR> Failed to make context current! (%lX)", GetLastError());

	// write info to buffer
	const char *extensions;
	uint32_t i;
	size_t len;

	BUFFER_Printf(buffer,
		"OpenGL Info\n"
		"=============================\n"
		"Vendor: %s\n"
		"Renderer: %s\n"
		"Version: %s\n"
		"Shading Language Version: %s\n"
		"\n\n"
		"Supported Extensions\n"
		"=============================\n",
		glGetString(GL_VENDOR), glGetString(GL_RENDERER),
		glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

	// TODO: get limits too

	extensions = glGetString(GL_EXTENSIONS);
	len = strlen(extensions);
	memcpy(buffer->data + buffer->writePos, extensions, len);
	for(i = buffer->writePos; i < (buffer->writePos + len); ++i){
		// change space for \n
		if(buffer->data[i] == 0x20)
			buffer->data[i] = 0x0A;
	}
	buffer->writePos += len;

	BUFFER_Printf(buffer,
		"\n\n"
		"#############################\n"
		"#############################\n"
		"\n\n");

	// cleanup
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(glcontext);
	ReleaseDC(hwnd, hdc);

	DestroyWindow(hwnd);
	UnregisterClass(className, hinstance);
}

static const char *VK_DeviceTypeString(VkPhysicalDeviceType deviceType)
{
	switch(deviceType){
		case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:	return "VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU";
		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:		return "VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU";
		case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:		return "VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU";
		case VK_PHYSICAL_DEVICE_TYPE_CPU:				return "VK_PHYSICAL_DEVICE_TYPE_CPU";
	}

	return "VK_PHYSICAL_DEVICE_TYPE_OTHER";
}

static PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr = NULL;
static PFN_vkCreateInstance vkCreateInstance = NULL;
static PFN_vkDestroyInstance vkDestroyInstance = NULL;
static PFN_vkEnumeratePhysicalDevices vkEnumeratePhysicalDevices = NULL;
static PFN_vkGetPhysicalDeviceProperties vkGetPhysicalDeviceProperties = NULL;
static PFN_vkGetPhysicalDeviceMemoryProperties vkGetPhysicalDeviceMemoryProperties = NULL;
static PFN_vkGetPhysicalDeviceFeatures vkGetPhysicalDeviceFeatures = NULL;
static PFN_vkGetPhysicalDeviceQueueFamilyProperties vkGetPhysicalDeviceQueueFamilyProperties = NULL;

#define VK_GET_INSTANCE_PROC_ADDR(instance, entrypoint)									\
	if(vk##entrypoint == NULL && vkGetInstanceProcAddr != NULL){						\
		vk##entrypoint =																\
			(PFN_vk##entrypoint)vkGetInstanceProcAddr(instance, "vk" #entrypoint);		\
		CHECK_RESULT(vk##entrypoint != NULL,											\
			"[Vulkan]<ERROR> Failed to load vk" #entrypoint "!");						\
	}

void VK_Info(buffer_t *buffer)
{
	VkResult res;
	VkInstance instance;
	VkPhysicalDevice devices[MAX_PHYSICAL_DEVICES];
	VkPhysicalDeviceProperties props;
	VkPhysicalDeviceMemoryProperties memProps;
	VkPhysicalDeviceFeatures features;
	VkQueueFamilyProperties familyProps[MAX_QUEUE_FAMILIES];
	uint32_t deviceCount, familyCount, i, k;


	// load vulkan dll
	HMODULE vkModule;
	vkModule = LoadLibrary("vulkan-1.dll");
	if(!vkModule)
		BUFFER_Printf(buffer, "No compatible Vulkan driver found.");

	else{

		//
		vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)GetProcAddress(vkModule, "vkGetInstanceProcAddr");
		CHECK_RESULT(vkGetInstanceProcAddr != NULL, "[Vulkan]<ERROR> Failed to load vkGetInstanceProcAddr");

		//
		VK_GET_INSTANCE_PROC_ADDR(NULL, CreateInstance);


		BUFFER_Printf(buffer,
			"Vulkan Info\n"
			"=============================\n\n");

		VkInstanceCreateInfo instanceCI = {
			.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
			.pNext = NULL,
			.flags = 0,
			.pApplicationInfo = NULL,
			.enabledLayerCount = 0,
			.ppEnabledLayerNames = NULL,
			.enabledExtensionCount = 0,
			.ppEnabledExtensionNames = NULL,
		};

		res = vkCreateInstance(&instanceCI, NULL, &instance);
		if(res != VK_SUCCESS){
			CHECK_RESULT(res == VK_ERROR_INCOMPATIBLE_DRIVER, "[Vulkan]<ERROR> Unexpected error: %d.\n", res);
			BUFFER_Printf(buffer, "No compatible Vulkan driver found.");

		}
		else{

			// load the other functions to be used
			VK_GET_INSTANCE_PROC_ADDR(instance, DestroyInstance);
			VK_GET_INSTANCE_PROC_ADDR(instance, EnumeratePhysicalDevices);
			VK_GET_INSTANCE_PROC_ADDR(instance, GetPhysicalDeviceProperties);
			VK_GET_INSTANCE_PROC_ADDR(instance, GetPhysicalDeviceMemoryProperties);
			VK_GET_INSTANCE_PROC_ADDR(instance, GetPhysicalDeviceFeatures);
			VK_GET_INSTANCE_PROC_ADDR(instance, GetPhysicalDeviceQueueFamilyProperties);

			res = vkEnumeratePhysicalDevices(instance, &deviceCount, NULL);
			CHECK_RESULT(res == VK_SUCCESS, "[Vulkan]<ERROR> Failed to enumerate physical devices!\n");

			if(deviceCount == 0)
				BUFFER_Printf(buffer, "[Vulkan]<ERROR> !\n");

			else {
				if(deviceCount > MAX_PHYSICAL_DEVICES)
					deviceCount = MAX_PHYSICAL_DEVICES;
				res = vkEnumeratePhysicalDevices(instance, &deviceCount, devices);
				CHECK_RESULT(res == VK_SUCCESS || res == VK_INCOMPLETE, "[Vulkan]<ERROR> Failed to retrieve physical device list!\n");

				for(i = 0; i < deviceCount; ++i){
					// write properties
					vkGetPhysicalDeviceProperties(devices[i], &props);
					BUFFER_Printf(buffer,
						"Device #%d\n"
						"=============================\n\n"
						"[Properties]\n"
						"* apiVersion: %d.%d.%d\n"
						"* driverVersion: %d\n"
						"* vendorID: %d\n"
						"* deviceID: %d\n"
						"* deviceType: %s\n"
						"* deviceName: %s\n"
						"\n\n"
						"[Memory Properties]\n",
						i,
						VK_VERSION_MAJOR(props.apiVersion),
						VK_VERSION_MINOR(props.apiVersion),
						VK_VERSION_PATCH(props.apiVersion),
						props.driverVersion,
						props.vendorID,
						props.deviceID,
						VK_DeviceTypeString(props.deviceType),
						props.deviceName);

					// TODO: get limits too

					// write memory properties
					vkGetPhysicalDeviceMemoryProperties(devices[i], &memProps);
					for(k = 0; k < memProps.memoryHeapCount; ++k){
						BUFFER_Printf(buffer,
							"* Heap #%d: size = %lld, flags = %d\n",
							k,
							memProps.memoryHeaps[k].size,
							memProps.memoryHeaps[k].flags);
					}
					// BUFFER_Printf(buffer, "\n");
					for(k = 0; k < memProps.memoryTypeCount; ++k){
						BUFFER_Printf(buffer,
							"* Type #%d: propertyFlags = %d, heapIndex = %d\n",
							k,
							memProps.memoryTypes[k].propertyFlags,
							memProps.memoryTypes[k].heapIndex);
					}

					// write features
					vkGetPhysicalDeviceFeatures(devices[i], &features);
					BUFFER_Printf(buffer,
						"\n\n"
						"[Features]\n"
						"* robustBufferAccess: %d\n"
						"* fullDrawIndexUint32: %d\n"
						"* imageCubeArray: %d\n"
						"* independentBlend: %d\n"
						"* geometryShader: %d\n"
						"* tessellationShader: %d\n"
						"* sampleRateShading: %d\n"
						"* dualSrcBlend: %d\n"
						"* logicOp: %d\n"
						"* multiDrawIndirect: %d\n"
						"* drawIndirectFirstInstance: %d\n"
						"* depthClamp: %d\n"
						"* depthBiasClamp: %d\n"
						"* fillModeNonSolid: %d\n"
						"* depthBounds: %d\n"
						"* wideLines: %d\n"
						"* largePoints: %d\n"
						"* alphaToOne: %d\n"
						"* multiViewport: %d\n"
						"* samplerAnisotropy: %d\n"
						"* textureCompressionETC2: %d\n"
						"* textureCompressionASTC_LDR: %d\n"
						"* textureCompressionBC: %d\n"
						"* occlusionQueryPrecise: %d\n"
						"* pipelineStatisticsQuery: %d\n"
						"* vertexPipelineStoresAndAtomics: %d\n"
						"* fragmentStoresAndAtomics: %d\n"
						"* shaderTessellationAndGeometryPointSize: %d\n"
						"* shaderImageGatherExtended: %d\n"
						"* shaderStorageImageExtendedFormats: %d\n"
						"* shaderStorageImageMultisample: %d\n"
						"* shaderStorageImageReadWithoutFormat: %d\n"
						"* shaderStorageImageWriteWithoutFormat: %d\n"
						"* shaderUniformBufferArrayDynamicIndexing: %d\n"
						"* shaderSampledImageArrayDynamicIndexing: %d\n"
						"* shaderStorageBufferArrayDynamicIndexing: %d\n"
						"* shaderStorageImageArrayDynamicIndexing: %d\n"
						"* shaderClipDistance: %d\n"
						"* shaderCullDistance: %d\n"
						"* shaderFloat64: %d\n"
						"* shaderInt64: %d\n"
						"* shaderInt16: %d\n"
						"* shaderResourceResidency: %d\n"
						"* shaderResourceMinLod: %d\n"
						"* sparseBinding: %d\n"
						"* sparseResidencyBuffer: %d\n"
						"* sparseResidencyImage2D: %d\n"
						"* sparseResidencyImage3D: %d\n"
						"* sparseResidency2Samples: %d\n"
						"* sparseResidency4Samples: %d\n"
						"* sparseResidency8Samples: %d\n"
						"* sparseResidency16Samples: %d\n"
						"* sparseResidencyAliased: %d\n"
						"* variableMultisampleRate: %d\n"
						"* inheritedQueries: %d\n"
						"\n\n"
						"[Queue Family Properties]\n\n",
						features.robustBufferAccess,
						features.fullDrawIndexUint32,
						features.imageCubeArray,
						features.independentBlend,
						features.geometryShader,
						features.tessellationShader,
						features.sampleRateShading,
						features.dualSrcBlend,
						features.logicOp,
						features.multiDrawIndirect,
						features.drawIndirectFirstInstance,
						features.depthClamp,
						features.depthBiasClamp,
						features.fillModeNonSolid,
						features.depthBounds,
						features.wideLines,
						features.largePoints,
						features.alphaToOne,
						features.multiViewport,
						features.samplerAnisotropy,
						features.textureCompressionETC2,
						features.textureCompressionASTC_LDR,
						features.textureCompressionBC,
						features.occlusionQueryPrecise,
						features.pipelineStatisticsQuery,
						features.vertexPipelineStoresAndAtomics,
						features.fragmentStoresAndAtomics,
						features.shaderTessellationAndGeometryPointSize,
						features.shaderImageGatherExtended,
						features.shaderStorageImageExtendedFormats,
						features.shaderStorageImageMultisample,
						features.shaderStorageImageReadWithoutFormat,
						features.shaderStorageImageWriteWithoutFormat,
						features.shaderUniformBufferArrayDynamicIndexing,
						features.shaderSampledImageArrayDynamicIndexing,
						features.shaderStorageBufferArrayDynamicIndexing,
						features.shaderStorageImageArrayDynamicIndexing,
						features.shaderClipDistance,
						features.shaderCullDistance,
						features.shaderFloat64,
						features.shaderInt64,
						features.shaderInt16,
						features.shaderResourceResidency,
						features.shaderResourceMinLod,
						features.sparseBinding,
						features.sparseResidencyBuffer,
						features.sparseResidencyImage2D,
						features.sparseResidencyImage3D,
						features.sparseResidency2Samples,
						features.sparseResidency4Samples,
						features.sparseResidency8Samples,
						features.sparseResidency16Samples,
						features.sparseResidencyAliased,
						features.variableMultisampleRate,
						features.inheritedQueries);

					// write queue family properties
					vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &familyCount, NULL);
					CHECK_RESULT(familyCount > 0, "[Vulkan]<ERROR> Failed to enumerate device queue families!\n");
					if(familyCount > MAX_QUEUE_FAMILIES)
						familyCount = MAX_QUEUE_FAMILIES;
					vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &familyCount, familyProps);

					for(k = 0; k < familyCount; ++k){
						BUFFER_Printf(buffer,
							"Family #%d:\n"
							"* queueFlags: %d\n"
							"* queueCount: %d\n"
							"* timestampValidBits: %d\n"
							"* minImageTransferGranularity: width = %d, height = %d, depth = %d\n\n",
							k,
							familyProps[k].queueFlags,
							familyProps[k].queueCount,
							familyProps[k].timestampValidBits,
							familyProps[k].minImageTransferGranularity.width,
							familyProps[k].minImageTransferGranularity.height,
							familyProps[k].minImageTransferGranularity.depth);
					}
				}
			}

			vkDestroyInstance(instance, NULL);
		}

		FreeLibrary(vkModule);
	}

	BUFFER_Printf(buffer,
		"\n\n"
		"#############################\n"
		"#############################\n"
		"\n\n");

}

int main(int argc, char **argv)
{
	FILE *output;
	buffer_t glbuffer;
	buffer_t vkbuffer;

	glbuffer.writePos = 0;
	GL_Info(&glbuffer);

	vkbuffer.writePos = 0;
	VK_Info(&vkbuffer);

	output = fopen("info.txt", "w+");
	CHECK_RESULT(output != NULL, "<ERROR> Failed to open output file for writing!\n");

	if(fwrite(glbuffer.data, glbuffer.writePos, 1, output) != 1)
		printf("<ERROR> Failed to output OpenGL info!\n");

	if(fwrite(vkbuffer.data, vkbuffer.writePos, 1, output) != 1)
		printf("<ERROR> Failed to output vulkan info!\n");
	fclose(output);

	printf("File 'info.txt' generated. Press any key to close...");
	_getch();
	return 0;
}
