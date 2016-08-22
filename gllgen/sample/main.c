#include <stdlib.h>
#include "vk_loader.h"
#include "log.h"

#define CHECK_RESULT(res, fmt, ...)		\
	if(!(res)){							\
		LOG_ERROR(fmt, __VA_ARGS__);	\
		getchar();						\
		exit(-1);						\
	}

int main(int argc, char **argv)
{
	VkResult res;
	VkInstance instance;

	// Load Base API
	// =============================
	CHECK_RESULT(VK_LoadBaseAPI() == VK_TRUE, "Failed to load base API!\n");

	// Create Instance
	// =============================
	uint32_t extCount = 3;
	const char *extensions[] = {
		VK_EXT_DEBUG_REPORT_EXTENSION_NAME,
		VK_KHR_SURFACE_EXTENSION_NAME,
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
	};

	VkInstanceCreateInfo instance_ci = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext = NULL,
		.flags = 0,
		.pApplicationInfo = NULL,
		.enabledLayerCount = 0,
		.ppEnabledLayerNames = NULL,
		.enabledExtensionCount = extCount,
		.ppEnabledExtensionNames = extensions
	};
	res = vkCreateInstance(&instance_ci, NULL, &instance);
	CHECK_RESULT(res == VK_SUCCESS, "Failed to create instance\n");

	// Load Instance API
	// =============================
	CHECK_RESULT(VK_LoadInstanceAPI(instance, extCount, extensions) == VK_TRUE, "Failed to load instance API!\n");

	// Enumerate Physical Devices
	// =============================

	// Choose Queue Family Index
	// =============================

	// Create Device
	// =============================

	LOG("SUCCESS\n");
	getchar();
	return 0;
}