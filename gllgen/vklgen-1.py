import os

# Enums
# ==================================

# API enums
VK_BASE						= 0
VK_INSTANCE					= 1
VK_DEVICE					= 2

# platform enums
VK_XLIB						= 0
VK_XCB						= 1
VK_WAYLAND					= 2
VK_MIR						= 3
VK_ANDROID					= 4
VK_WIN32					= 5

# extension enums

# instance extensions
VK_EXT_debug_report			= 0
VK_KHR_surface				= 1
VK_KHR_display				= 2
VK_KHR_xlib_surface			= 3
VK_KHR_xcb_surface			= 4
VK_KHR_wayland_surface		= 5
VK_KHR_mir_surface			= 6
VK_KHR_android_surface		= 7
VK_KHR_win32_surface		= 8

# device extensions
VK_KHR_swapchain			= 9
VK_KHR_display_swapchain	= 10

# Tables
# ==================================
VK_EXT_NAME = {
	VK_EXT_debug_report			: "VK_EXT_debug_report",
	VK_KHR_surface				: "VK_KHR_surface",
	VK_KHR_display				: "VK_KHR_display",
	VK_KHR_xlib_surface			: "VK_KHR_xlib_surface",
	VK_KHR_xcb_surface			: "VK_KHR_xcb_surface",
	VK_KHR_wayland_surface		: "VK_KHR_wayland_surface",
	VK_KHR_mir_surface			: "VK_KHR_mir_surface",
	VK_KHR_android_surface		: "VK_KHR_android_surface",
	VK_KHR_win32_surface		: "VK_KHR_win32_surface",
	VK_KHR_swapchain			: "VK_KHR_swapchain",
	VK_KHR_display_swapchain	: "VK_KHR_display_swapchain"
}

VK_WSI_EXT = {
	VK_XLIB		: VK_KHR_xlib_surface,
	VK_XCB		: VK_KHR_xcb_surface,
	VK_WAYLAND	: VK_KHR_wayland_surface,
	VK_MIR		: VK_KHR_mir_surface,
	VK_ANDROID	: VK_KHR_android_surface,
	VK_WIN32	: VK_KHR_win32_surface
}

VK_WSI_DEFINE = {
	VK_XLIB		: "VK_USE_PLATFORM_XLIB_KHR",
	VK_XCB		: "VK_USE_PLATFORM_XCB_KHR",
	VK_WAYLAND	: "VK_USE_PLATFORM_WAYLAND_KHR",
	VK_MIR		: "VK_USE_PLATFORM_MIR_KHR",
	VK_ANDROID	: "VK_USE_PLATFORM_ANDROID_KHR",
	VK_WIN32	: "VK_USE_PLATFORM_WIN32_KHR"
}

# Macros
# ==================================

VK_CORE = {}
VK_EXT = {}
VK_WSI = {}

def VK_CORE_PROC(API, name):
	if not API in VK_CORE:
		VK_CORE[API] = []

	VK_CORE[API].append(name)

def VK_EXT_PROC(API, ext, name):
	if not API in VK_EXT:
		VK_EXT[API] = {}

	if not ext in VK_EXT[API]:
		VK_EXT[API][ext] = []

	VK_EXT[API][ext].append(name)

def VK_WSI_PROC(platform, name):
	if not platform in VK_WSI:
		VK_WSI[platform] = []

	VK_WSI[platform].append(name)

# Vulkan 1.0 Core
# ==================================

# VK Core Base
VK_CORE_PROC(VK_BASE, "vkGetInstanceProcAddr")
VK_CORE_PROC(VK_BASE, "vkCreateInstance")
VK_CORE_PROC(VK_BASE, "vkEnumerateInstanceExtensionProperties")
VK_CORE_PROC(VK_BASE, "vkEnumerateInstanceLayerProperties")

# VK Core Instance
VK_CORE_PROC(VK_INSTANCE, "vkGetDeviceProcAddr")
VK_CORE_PROC(VK_INSTANCE, "vkDestroyInstance")
VK_CORE_PROC(VK_INSTANCE, "vkEnumeratePhysicalDevices")
VK_CORE_PROC(VK_INSTANCE, "vkGetPhysicalDeviceFeatures")
VK_CORE_PROC(VK_INSTANCE, "vkGetPhysicalDeviceFormatProperties")
VK_CORE_PROC(VK_INSTANCE, "vkGetPhysicalDeviceImageFormatProperties")
VK_CORE_PROC(VK_INSTANCE, "vkGetPhysicalDeviceProperties")
VK_CORE_PROC(VK_INSTANCE, "vkGetPhysicalDeviceQueueFamilyProperties")
VK_CORE_PROC(VK_INSTANCE, "vkGetPhysicalDeviceMemoryProperties")
VK_CORE_PROC(VK_INSTANCE, "vkCreateDevice")
VK_CORE_PROC(VK_INSTANCE, "vkEnumerateDeviceExtensionProperties")
VK_CORE_PROC(VK_INSTANCE, "vkEnumerateDeviceLayerProperties")
VK_CORE_PROC(VK_INSTANCE, "vkGetPhysicalDeviceSparseImageFormatProperties")

# VK Core Device
VK_CORE_PROC(VK_DEVICE, "vkDestroyDevice")
VK_CORE_PROC(VK_DEVICE, "vkGetDeviceQueue")
VK_CORE_PROC(VK_DEVICE, "vkQueueSubmit")
VK_CORE_PROC(VK_DEVICE, "vkQueueWaitIdle")
VK_CORE_PROC(VK_DEVICE, "vkDeviceWaitIdle")
VK_CORE_PROC(VK_DEVICE, "vkAllocateMemory")
VK_CORE_PROC(VK_DEVICE, "vkFreeMemory")
VK_CORE_PROC(VK_DEVICE, "vkMapMemory")
VK_CORE_PROC(VK_DEVICE, "vkUnmapMemory")
VK_CORE_PROC(VK_DEVICE, "vkFlushMappedMemoryRanges")
VK_CORE_PROC(VK_DEVICE, "vkInvalidateMappedMemoryRanges")
VK_CORE_PROC(VK_DEVICE, "vkGetDeviceMemoryCommitment")
VK_CORE_PROC(VK_DEVICE, "vkBindBufferMemory")
VK_CORE_PROC(VK_DEVICE, "vkBindImageMemory")
VK_CORE_PROC(VK_DEVICE, "vkGetBufferMemoryRequirements")
VK_CORE_PROC(VK_DEVICE, "vkGetImageMemoryRequirements")
VK_CORE_PROC(VK_DEVICE, "vkGetImageSparseMemoryRequirements")
VK_CORE_PROC(VK_DEVICE, "vkQueueBindSparse")
VK_CORE_PROC(VK_DEVICE, "vkCreateFence")
VK_CORE_PROC(VK_DEVICE, "vkDestroyFence")
VK_CORE_PROC(VK_DEVICE, "vkResetFences")
VK_CORE_PROC(VK_DEVICE, "vkGetFenceStatus")
VK_CORE_PROC(VK_DEVICE, "vkWaitForFences")
VK_CORE_PROC(VK_DEVICE, "vkCreateSemaphore")
VK_CORE_PROC(VK_DEVICE, "vkDestroySemaphore")
VK_CORE_PROC(VK_DEVICE, "vkCreateEvent")
VK_CORE_PROC(VK_DEVICE, "vkDestroyEvent")
VK_CORE_PROC(VK_DEVICE, "vkGetEventStatus")
VK_CORE_PROC(VK_DEVICE, "vkSetEvent")
VK_CORE_PROC(VK_DEVICE, "vkResetEvent")
VK_CORE_PROC(VK_DEVICE, "vkCreateQueryPool")
VK_CORE_PROC(VK_DEVICE, "vkDestroyQueryPool")
VK_CORE_PROC(VK_DEVICE, "vkGetQueryPoolResults")
VK_CORE_PROC(VK_DEVICE, "vkCreateBuffer")
VK_CORE_PROC(VK_DEVICE, "vkDestroyBuffer")
VK_CORE_PROC(VK_DEVICE, "vkCreateBufferView")
VK_CORE_PROC(VK_DEVICE, "vkDestroyBufferView")
VK_CORE_PROC(VK_DEVICE, "vkCreateImage")
VK_CORE_PROC(VK_DEVICE, "vkDestroyImage")
VK_CORE_PROC(VK_DEVICE, "vkGetImageSubresourceLayout")
VK_CORE_PROC(VK_DEVICE, "vkCreateImageView")
VK_CORE_PROC(VK_DEVICE, "vkDestroyImageView")
VK_CORE_PROC(VK_DEVICE, "vkCreateShaderModule")
VK_CORE_PROC(VK_DEVICE, "vkDestroyShaderModule")
VK_CORE_PROC(VK_DEVICE, "vkCreatePipelineCache")
VK_CORE_PROC(VK_DEVICE, "vkDestroyPipelineCache")
VK_CORE_PROC(VK_DEVICE, "vkGetPipelineCacheData")
VK_CORE_PROC(VK_DEVICE, "vkMergePipelineCaches")
VK_CORE_PROC(VK_DEVICE, "vkCreateGraphicsPipelines")
VK_CORE_PROC(VK_DEVICE, "vkCreateComputePipelines")
VK_CORE_PROC(VK_DEVICE, "vkDestroyPipeline")
VK_CORE_PROC(VK_DEVICE, "vkCreatePipelineLayout")
VK_CORE_PROC(VK_DEVICE, "vkDestroyPipelineLayout")
VK_CORE_PROC(VK_DEVICE, "vkCreateSampler")
VK_CORE_PROC(VK_DEVICE, "vkDestroySampler")
VK_CORE_PROC(VK_DEVICE, "vkCreateDescriptorSetLayout")
VK_CORE_PROC(VK_DEVICE, "vkDestroyDescriptorSetLayout")
VK_CORE_PROC(VK_DEVICE, "vkCreateDescriptorPool")
VK_CORE_PROC(VK_DEVICE, "vkDestroyDescriptorPool")
VK_CORE_PROC(VK_DEVICE, "vkResetDescriptorPool")
VK_CORE_PROC(VK_DEVICE, "vkAllocateDescriptorSets")
VK_CORE_PROC(VK_DEVICE, "vkFreeDescriptorSets")
VK_CORE_PROC(VK_DEVICE, "vkUpdateDescriptorSets")
VK_CORE_PROC(VK_DEVICE, "vkCreateFramebuffer")
VK_CORE_PROC(VK_DEVICE, "vkDestroyFramebuffer")
VK_CORE_PROC(VK_DEVICE, "vkCreateRenderPass")
VK_CORE_PROC(VK_DEVICE, "vkDestroyRenderPass")
VK_CORE_PROC(VK_DEVICE, "vkGetRenderAreaGranularity")
VK_CORE_PROC(VK_DEVICE, "vkCreateCommandPool")
VK_CORE_PROC(VK_DEVICE, "vkDestroyCommandPool")
VK_CORE_PROC(VK_DEVICE, "vkResetCommandPool")
VK_CORE_PROC(VK_DEVICE, "vkAllocateCommandBuffers")
VK_CORE_PROC(VK_DEVICE, "vkFreeCommandBuffers")
VK_CORE_PROC(VK_DEVICE, "vkBeginCommandBuffer")
VK_CORE_PROC(VK_DEVICE, "vkEndCommandBuffer")
VK_CORE_PROC(VK_DEVICE, "vkResetCommandBuffer")
VK_CORE_PROC(VK_DEVICE, "vkCmdBindPipeline")
VK_CORE_PROC(VK_DEVICE, "vkCmdSetViewport")
VK_CORE_PROC(VK_DEVICE, "vkCmdSetScissor")
VK_CORE_PROC(VK_DEVICE, "vkCmdSetLineWidth")
VK_CORE_PROC(VK_DEVICE, "vkCmdSetDepthBias")
VK_CORE_PROC(VK_DEVICE, "vkCmdSetBlendConstants")
VK_CORE_PROC(VK_DEVICE, "vkCmdSetDepthBounds")
VK_CORE_PROC(VK_DEVICE, "vkCmdSetStencilCompareMask")
VK_CORE_PROC(VK_DEVICE, "vkCmdSetStencilWriteMask")
VK_CORE_PROC(VK_DEVICE, "vkCmdSetStencilReference")
VK_CORE_PROC(VK_DEVICE, "vkCmdBindDescriptorSets")
VK_CORE_PROC(VK_DEVICE, "vkCmdBindIndexBuffer")
VK_CORE_PROC(VK_DEVICE, "vkCmdBindVertexBuffers")
VK_CORE_PROC(VK_DEVICE, "vkCmdDraw")
VK_CORE_PROC(VK_DEVICE, "vkCmdDrawIndexed")
VK_CORE_PROC(VK_DEVICE, "vkCmdDrawIndirect")
VK_CORE_PROC(VK_DEVICE, "vkCmdDrawIndexedIndirect")
VK_CORE_PROC(VK_DEVICE, "vkCmdDispatch")
VK_CORE_PROC(VK_DEVICE, "vkCmdDispatchIndirect")
VK_CORE_PROC(VK_DEVICE, "vkCmdCopyBuffer")
VK_CORE_PROC(VK_DEVICE, "vkCmdCopyImage")
VK_CORE_PROC(VK_DEVICE, "vkCmdBlitImage")
VK_CORE_PROC(VK_DEVICE, "vkCmdCopyBufferToImage")
VK_CORE_PROC(VK_DEVICE, "vkCmdCopyImageToBuffer")
VK_CORE_PROC(VK_DEVICE, "vkCmdUpdateBuffer")
VK_CORE_PROC(VK_DEVICE, "vkCmdFillBuffer")
VK_CORE_PROC(VK_DEVICE, "vkCmdClearColorImage")
VK_CORE_PROC(VK_DEVICE, "vkCmdClearDepthStencilImage")
VK_CORE_PROC(VK_DEVICE, "vkCmdClearAttachments")
VK_CORE_PROC(VK_DEVICE, "vkCmdResolveImage")
VK_CORE_PROC(VK_DEVICE, "vkCmdSetEvent")
VK_CORE_PROC(VK_DEVICE, "vkCmdResetEvent")
VK_CORE_PROC(VK_DEVICE, "vkCmdWaitEvents")
VK_CORE_PROC(VK_DEVICE, "vkCmdPipelineBarrier")
VK_CORE_PROC(VK_DEVICE, "vkCmdBeginQuery")
VK_CORE_PROC(VK_DEVICE, "vkCmdEndQuery")
VK_CORE_PROC(VK_DEVICE, "vkCmdResetQueryPool")
VK_CORE_PROC(VK_DEVICE, "vkCmdWriteTimestamp")
VK_CORE_PROC(VK_DEVICE, "vkCmdCopyQueryPoolResults")
VK_CORE_PROC(VK_DEVICE, "vkCmdPushConstants")
VK_CORE_PROC(VK_DEVICE, "vkCmdBeginRenderPass")
VK_CORE_PROC(VK_DEVICE, "vkCmdNextSubpass")
VK_CORE_PROC(VK_DEVICE, "vkCmdEndRenderPass")
VK_CORE_PROC(VK_DEVICE, "vkCmdExecuteCommands")

# Instance Extensions
# ==================================

# VK_EXT_debug_report
VK_EXT_PROC(VK_INSTANCE, VK_EXT_debug_report, "vkCreateDebugReportCallbackEXT")
VK_EXT_PROC(VK_INSTANCE, VK_EXT_debug_report, "vkDestroyDebugReportCallbackEXT")
VK_EXT_PROC(VK_INSTANCE, VK_EXT_debug_report, "vkDebugReportMessageEXT")

# VK_KHR_surface
VK_EXT_PROC(VK_INSTANCE, VK_KHR_surface, "vkDestroySurfaceKHR")
VK_EXT_PROC(VK_INSTANCE, VK_KHR_surface, "vkGetPhysicalDeviceSurfaceSupportKHR")
VK_EXT_PROC(VK_INSTANCE, VK_KHR_surface, "vkGetPhysicalDeviceSurfaceCapabilitiesKHR")
VK_EXT_PROC(VK_INSTANCE, VK_KHR_surface, "vkGetPhysicalDeviceSurfaceFormatsKHR")
VK_EXT_PROC(VK_INSTANCE, VK_KHR_surface, "vkGetPhysicalDeviceSurfacePresentModesKHR")

# VK_KHR_display
VK_EXT_PROC(VK_INSTANCE, VK_KHR_display, "vkGetPhysicalDeviceDisplayPropertiesKHR")
VK_EXT_PROC(VK_INSTANCE, VK_KHR_display, "vkGetPhysicalDeviceDisplayPlanePropertiesKHR")
VK_EXT_PROC(VK_INSTANCE, VK_KHR_display, "vkGetDisplayPlaneSupportedDisplaysKHR")
VK_EXT_PROC(VK_INSTANCE, VK_KHR_display, "vkGetDisplayModePropertiesKHR")
VK_EXT_PROC(VK_INSTANCE, VK_KHR_display, "vkCreateDisplayModeKHR")
VK_EXT_PROC(VK_INSTANCE, VK_KHR_display, "vkGetDisplayPlaneCapabilitiesKHR")
VK_EXT_PROC(VK_INSTANCE, VK_KHR_display, "vkCreateDisplayPlaneSurfaceKHR")

# WSI Extensions (Instance)
# ==================================

# VK_KHR_xlib_surface
VK_WSI_PROC(VK_XLIB, "vkCreateXlibSurfaceKHR");
VK_WSI_PROC(VK_XLIB, "vkGetPhysicalDeviceXlibPresentationSupportKHR")

# VK_KHR_xcb_surface
VK_WSI_PROC(VK_XCB, "vkCreateXcbSurfaceKHR")
VK_WSI_PROC(VK_XCB, "vkGetPhysicalDeviceXcbPresentationSupportKHR")

# VK_KHR_wayland_surface
VK_WSI_PROC(VK_WAYLAND, "vkCreateWaylandSurfaceKHR")
VK_WSI_PROC(VK_WAYLAND, "vkGetPhysicalDeviceWaylandPresentationSupportKHR")

# VK_KHR_mir_surface
VK_WSI_PROC(VK_MIR, "vkCreateMirSurfaceKHR")
VK_WSI_PROC(VK_MIR, "vkGetPhysicalDeviceMirPresentationSupportKHR")

# VK_KHR_android_surface
VK_WSI_PROC(VK_ANDROID, "vkCreateAndroidSurfaceKHR")

# VK_KHR_win32_surface
VK_WSI_PROC(VK_WIN32, "vkCreateWin32SurfaceKHR")
VK_WSI_PROC(VK_WIN32, "vkGetPhysicalDeviceWin32PresentationSupportKHR")

# Device Extensions
# ==================================

# VK_KHR_swapchain
VK_EXT_PROC(VK_DEVICE, VK_KHR_swapchain, "vkCreateSwapchainKHR")
VK_EXT_PROC(VK_DEVICE, VK_KHR_swapchain, "vkDestroySwapchainKHR")
VK_EXT_PROC(VK_DEVICE, VK_KHR_swapchain, "vkGetSwapchainImagesKHR")
VK_EXT_PROC(VK_DEVICE, VK_KHR_swapchain, "vkAcquireNextImageKHR")
VK_EXT_PROC(VK_DEVICE, VK_KHR_swapchain, "vkQueuePresentKHR")

# VK_KHR_display_swapchain
#VK_EXT_PROC(VK_DEVICE, VK_KHR_display_swapchain, "vkCreateSharedSwapchainsKHR")


# Generate Files
# ==================================

if not os.path.exists("vulkan"):
	os.makedirs("vulkan")

# TODO: Add other platforms defines

VK_EXT_INSTANCE_ITEMS = VK_EXT[VK_INSTANCE].items()
VK_EXT_DEVICE_ITEMS = VK_EXT[VK_DEVICE].items()
VK_WSI_ITEMS = VK_WSI.items()

# vk_loader.h
with open("vulkan/vk_loader.h", "wb") as f:
	f.write(br'''// this file has been automatically generated by vklgen-1.py

#ifndef __VK_LOADER_H__
#define __VK_LOADER_H__

#ifdef _WIN32
	#define VK_USE_PLATFORM_WIN32_KHR
#endif

#define VK_NO_PROTOTYPES
#include <vulkan/vulkan.h>
#include <vulkan/vk_sdk_platform.h>

typedef struct VkInfo{
''')

	f.write(b"\t// instance extensions\n")
	for ext, proc_list in VK_EXT_INSTANCE_ITEMS:
		f.write(("\tVkBool32 %s;\n" % VK_EXT_NAME[ext][3:]).encode("utf-8"))

	for platform, proc_list in VK_WSI_ITEMS:
		f.write(("\tVkBool32 %s;\n" % VK_EXT_NAME[VK_WSI_EXT[platform]][3:]).encode("utf-8"))

	f.write(b"\n\t// device extensions\n")
	for ext, proc_list in VK_EXT_DEVICE_ITEMS:
		f.write(("\tVkBool32 %s;\n" % VK_EXT_NAME[ext][3:]).encode("utf-8"))

	f.write(br'''
} VkInfo;

extern VkInfo vk_info;

// Vulkan 1.0 Core
// ==================================

''')


	if VK_BASE in VK_CORE:
		f.write(b"// Base API\n")
		for name in VK_CORE[VK_BASE]:
			f.write(("extern VKAPI_ATTR PFN_%s %s;\n" % (name, name)).encode("utf-8"))
		f.write(b"\n")

	if VK_INSTANCE in VK_CORE:
		f.write(b"// Instance API\n")
		for name in VK_CORE[VK_INSTANCE]:
			f.write(("extern VKAPI_ATTR PFN_%s %s;\n" % (name, name)).encode("utf-8"))
		f.write(b"\n")
	
	if VK_DEVICE in VK_CORE:
		f.write(b"// Device API\n")
		for name in VK_CORE[VK_DEVICE]:
			f.write(("extern VKAPI_ATTR PFN_%s %s;\n" % (name, name)).encode("utf-8"))
		f.write(b"\n")


	if VK_INSTANCE in VK_EXT:
		f.write(br'''
// Instance Extensions
// ==================================
''')
		for ext, proc_list in VK_EXT_INSTANCE_ITEMS:
			f.write(("\n// %s\n" % VK_EXT_NAME[ext]).encode("utf-8"))
			for name in proc_list:
				f.write(("extern VKAPI_ATTR PFN_%s %s;\n" % (name, name)).encode("utf-8"))


	if len(VK_WSI) > 0:
		f.write(br'''
// WSI Extensions (Instance)
// ==================================
''')
		for platform, proc_list in VK_WSI_ITEMS:
			f.write((r'''
// %s
#ifdef %s
''' % (VK_EXT_NAME[VK_WSI_EXT[platform]],
		VK_WSI_DEFINE[platform])).encode("utf-8"))

			for name in proc_list:
				f.write(("extern VKAPI_ATTR PFN_%s %s;\n" % (name, name)).encode("utf-8"))
			f.write(b"#endif\n")

	if VK_DEVICE in VK_EXT:
		f.write(br'''
// Device Extensions
// ==================================
''')
		for ext, proc_list in VK_EXT_DEVICE_ITEMS:
			f.write(("\n// %s\n" % VK_EXT_NAME[ext]).encode("utf-8"))
			for name in proc_list:
				f.write(("extern VKAPI_ATTR PFN_%s %s;\n" % (name, name)).encode("utf-8"))

	f.write(br'''
// API Loading
// ==================================
VkBool32	VK_LoadBaseAPI();
VkBool32	VK_LoadInstanceAPI(VkInstance instance, uint32_t enabledExtensionCount, const char **ppEnabledExtensions);
VkBool32	VK_LoadDeviceAPI(VkDevice device, uint32_t enabledExtensionCount, const char **ppEnabledExtensions);

void		VK_UnloadAPI();


#endif //__VK_LOADER_H__
''')

# vk_loader.c
with open("vulkan/vk_loader.c", "wb") as f:
	f.write(br'''// this file was generated automatically by vklgen-1.py

///////////////////////////////////////////////////////////////////////////////////////
// Overview:
//	The Vulkan Loader will load the Vulkan API by doing 3 steps:
//		1 - Loading the base API;
//		2 - Loading the instance API;
//		3 - Loading the device API.
//
//	1 - Loading the base API doesn't require any other preparation and it is required
//	to create any instance of Vulkan.
//
//	2 - Loading the instance API requires a valid Vulkan instance to be created before.
//	This can only be achieved by first loading the base API or by linking the vulkan
//	library into the application which in this case shouldn't be using this loader.
//
//	3 - Loading the device API requires a valid Vulkan device to be created before. This
//	can only be achieved by first setting up a valid Vulkan instance, querying physical
//	device information and then creating the device itself. These preparations can only
//	be done by first loading the base API and the instance API or by linking the vulkan
//	library into the application which in this case shouldn't be using this loader.
//
///////////////////////////////////////////////////////////////////////////////////////
// Base API: the functions needed for querying any extension or layer properties
// and for creating a valid Vulkan Instance. The instance and device APIs can't load
// without this.
//
// Instance API: the functions needed for querying information about the system hardware
// and the creation of the Vulkan Device. The device API can't load without this.
//
// Device API: the functions needed for the Vulkan Device management.
//
///////////////////////////////////////////////////////////////////////////////////////
// VK_LoadBaseAPI
//	- Info: Loads the base API.
//	- Return Value: In case of failure it will return VK_FALSE, otherwise VK_TRUE will
//	be returned.
//
// VK_LoadInstanceAPI
//	- Info: Loads the instance API.
//	- Params:
//		@instance: valid Vulkan instance
//		@enabledExtensionCount: same as used in instance creation
//		@ppEnabledExtensions: same as used in instance creation
//	- Return Value: In case of failure it will return VK_FALSE, otherwise VK_TRUE will
//	be returned.
//
// VK_LoadDeviceAPI
//	- Info: Loads the Device API.
//	- Params:
//		@device: valid Vulkan device
//		@enabledExtensionCount: same as used in device creation
//		@ppEnabledExtensions: same as used in device creation
//	- Return Value: In case of failure it will return VK_FALSE, otherwise VK_TRUE will
//	be returned.
//
// VK_UnloadAPI
//	- Info: Unloads the Vulkan library and reset the loader status back to unloaded.
//
///////////////////////////////////////////////////////////////////////////////////////

#include "vk_loader.h"

// to activate logging, LOG, LOG_WARNING, LOG_ERROR, and LOG_DEBUG
// macros should be defined using any logging interface

#ifndef LOG
	#define LOG(fmt, ...)
#endif

#ifndef LOG_WARNING
	#define LOG_WARNING(fmt, ...)
#endif

#ifndef LOG_ERROR
    #define LOG_ERROR(fmt, ...)
#endif

#ifndef LOG_DEBUG
	#define LOG_DEBUG(fmt, ...)
#endif


VkInfo vk_info;

// Vulkan Loader State
// 0 = unloaded
// 1 = base api loaded
// 2 = instance api loaded
// 3 = device api loaded
uint32_t l_state = 0;

// Vulkan 1.0 Core
// ==================================

''')
	if VK_BASE in VK_CORE:
		f.write(b"// Base API\n")
		for name in VK_CORE[VK_BASE]:
			f.write(("VKAPI_ATTR PFN_%s %s = NULL;\n" % (name, name)).encode("utf-8"))

		f.write(b"\n")

	if VK_INSTANCE in VK_CORE:
		f.write(b"// Instance API\n")
		for name in VK_CORE[VK_INSTANCE]:
			f.write(("VKAPI_ATTR PFN_%s %s = NULL;\n" % (name, name)).encode("utf-8"))
		f.write(b"\n")
	
	if VK_DEVICE in VK_CORE:
		f.write(b"// Device API\n")
		for name in VK_CORE[VK_DEVICE]:
			f.write(("VKAPI_ATTR PFN_%s %s = NULL;\n" % (name, name)).encode("utf-8"))
		f.write(b"\n")


	if VK_INSTANCE in VK_EXT:
		f.write(br'''
// Instance Extensions
// ==================================
''')
		for ext, proc_list in VK_EXT_INSTANCE_ITEMS:
			f.write(("\n// %s\n" % VK_EXT_NAME[ext]).encode("utf-8"))
			for name in proc_list:
				f.write(("VKAPI_ATTR PFN_%s %s = NULL;\n" % (name, name)).encode("utf-8"))


	if len(VK_WSI) > 0:
		f.write(br'''
// WSI Extensions (Instance)
// ==================================
''')
		for platform, proc_list in VK_WSI_ITEMS:
			f.write((r'''
// %s
#ifdef %s
''' % (VK_EXT_NAME[VK_WSI_EXT[platform]],
		VK_WSI_DEFINE[platform])).encode("utf-8"))

			for name in proc_list:
				f.write(("VKAPI_ATTR PFN_%s %s = NULL;\n" % (name, name)).encode("utf-8"))
			f.write(b"#endif\n")

	if VK_DEVICE in VK_EXT:
		f.write(br'''
// Device Extensions
// ==================================
''')
		for ext, proc_list in VK_EXT_DEVICE_ITEMS:
			f.write(("\n// %s\n" % VK_EXT_NAME[ext]).encode("utf-8"))
			for name in proc_list:
				f.write(("VKAPI_ATTR PFN_%s %s = NULL;\n" % (name, name)).encode("utf-8"))

	f.write(br'''
// Platform specific library loading
// ==================================

#if defined(_WIN32)
#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>

static HMODULE l_libvulkan = NULL;

static VkBool32 VK_OpenLib()
{
	if(l_libvulkan == NULL){
		l_libvulkan = LoadLibraryA("vulkan-1.dll");
		if(l_libvulkan == NULL)
			return VK_FALSE;
	}

	return VK_TRUE;
}

static void VK_CloseLib()
{
	if(l_libvulkan != NULL){
		FreeLibrary(l_libvulkan);
		l_libvulkan = NULL;
	}
}

static VkBool32 VK_IsLibOpen()
{
	return (l_libvulkan != NULL);
}

static PFN_vkVoidFunction VK_GetProcAddr(const char *procName)
{
	if(l_libvulkan == NULL){
		LOG_ERROR("VK_GetProcAddr: Vulkan library is not loaded!");
		return NULL;
	}

	return (PFN_vkVoidFunction)GetProcAddress(l_libvulkan, procName);
}
#endif

// API Loading
// ==================================

VkBool32 VK_LoadBaseAPI()
{
	// LOG("Updating your drivers may solve this issue.");
	VkBool32 res;

	if(VK_IsLibOpen() == VK_FALSE){
		if(VK_OpenLib() == VK_FALSE){
			LOG_ERROR("VK_LoadBaseAPI: Failed to open Vulkan library.");
			return VK_FALSE;
		}
	}

	if(l_state >= 1){
		LOG_WARNING("VK_LoadBaseAPI: Base API is already loaded!");
		return VK_TRUE;
	}

	vkGetInstanceProcAddr = (PFN_vkGetInstanceProcAddr) VK_GetProcAddr("vkGetInstanceProcAddr");
	if(vkGetInstanceProcAddr == NULL){
		LOG_ERROR("VK_LoadBaseAPI: Failed to load vkGetInstanceProcAddr! Unable to proceed.");
		VK_CloseLib();
		return VK_FALSE;
	}

	//=================================================================
	#define VK_GET_PROC_ADDR(proc_name)												\
		proc_name = (PFN_##proc_name) vkGetInstanceProcAddr(NULL, #proc_name);		\
		if(proc_name == NULL){														\
			LOG_ERROR("VK_LoadBaseAPI: Failed to load '%s'", #proc_name);			\
			res = VK_FALSE;															\
		}

	res = VK_TRUE;
''')

	# vkGetInstanceProcAddr needs to be loaded directly from the DLL
	# and if we use it to load itself, NULL is returned (at least in
	# driver implementation)

	for name in VK_CORE[VK_BASE]:
		if name != "vkGetInstanceProcAddr":
			f.write(("\tVK_GET_PROC_ADDR(%s);\n" % name).encode("utf-8"))

	f.write(br'''
	#undef VK_GET_PROC_ADDR
	//=================================================================


	if(res == VK_FALSE){
		LOG_ERROR("VK_LoadBaseAPI: Failed to load all procs.");
		VK_CloseLib();
		return VK_FALSE;
	}

	l_state = 1;
	return VK_TRUE;
}

VkBool32 VK_LoadInstanceAPI(VkInstance instance, uint32_t enabledExtensionCount, const char **ppEnabledExtensions)
{
	VkBool32 ret;
	uint32_t i;

	if(VK_IsLibOpen() == VK_FALSE || l_state == 0){
		LOG_ERROR("VK_LoadInstanceAPI: Base API not loaded!");
		return VK_FALSE;
	}

	if(l_state >= 2){
		LOG_WARNING("VK_LoadInstanceAPI: Instance API is already loaded!");
		return VK_TRUE;
	}

	//=================================================================
	#define VK_GET_PROC_ADDR(proc_name)													\
		proc_name = (PFN_##proc_name) vkGetInstanceProcAddr(instance, #proc_name);		\
		if(proc_name == NULL){															\
			LOG_ERROR("VK_LoadInstanceAPI: Failed to load '%s'", #proc_name);			\
			ret = VK_FALSE;																\
		}

	// load core
	ret = VK_TRUE;
''')

	for name in VK_CORE[VK_INSTANCE]:
		f.write(("\tVK_GET_PROC_ADDR(%s);\n" % name).encode("utf-8"))

	f.write(br'''

	if(ret == VK_FALSE){
		LOG_ERROR("VK_LoadInstanceAPI: Failed to load core instance procs.");
		return VK_FALSE;
	}

	// load extensions
	for(i = 0; i < enabledExtensionCount; i++){
''')

	if_str = "if"
	first = True

	for ext, proc_list in VK_EXT_INSTANCE_ITEMS:
		f.write((r'''
		%s(strcmp(ppEnabledExtensions[i], "%s") == 0){
			ret = VK_TRUE;
''' % (if_str,
		VK_EXT_NAME[ext]
		)).encode("utf-8"))

		for name in proc_list:
			f.write(("\t\t\tVK_GET_PROC_ADDR(%s);\n" % name).encode("utf-8"))

		f.write((r'''
			vk_info.%s = ret;
			if(ret == VK_FALSE)
				LOG_WARNING("VK_LoadInstanceAPI: Failed to load extension '%%s'", "%s");
		}
''' % (VK_EXT_NAME[ext][3:],
		VK_EXT_NAME[ext]
		)).encode("utf-8"))

		if first:
			if_str = "else if"
			first = False


	for platform, proc_list in VK_WSI_ITEMS:
		f.write((r'''
#ifdef %s
		%s(strcmp(ppEnabledExtensions[i], "%s") == 0){
			ret = VK_TRUE;
''' % (VK_WSI_DEFINE[platform],
		if_str,
		VK_EXT_NAME[VK_WSI_EXT[platform]]
		)).encode("utf-8"))

		for name in proc_list:
			f.write(("\t\t\tVK_GET_PROC_ADDR(%s);\n" % name).encode("utf-8"))

		f.write((r'''
			vk_info.%s = ret;
			if(ret == VK_FALSE)
				LOG_WARNING("VK_LoadInstanceAPI: Failed to load extension '%%s'", "%s");
		}
#endif
''' % (VK_EXT_NAME[VK_WSI_EXT[platform]][3:],
		VK_EXT_NAME[VK_WSI_EXT[platform]]
		)).encode("utf-8"))

		if first:
			if_str = "else if"
			first = False

	f.write(br'''
	}

	#undef VK_GET_PROC_ADDR
	//=================================================================

	l_state = 2;
	return VK_TRUE;
}

VkBool32 VK_LoadDeviceAPI(VkDevice device, uint32_t enabledExtensionCount, const char **ppEnabledExtensions)
{
	VkBool32 ret;
	uint32_t i;

	if(VK_IsLibOpen() == VK_FALSE || l_state < 2){
		LOG_ERROR("VK_LoadDeviceAPI: Instance API not loaded!");
		return VK_FALSE;
	}

	if(l_state >= 3){
		LOG_WARNING("VK_LoadDeviceAPI: Device API is already loaded!");
		return VK_TRUE;
	}

	//=================================================================
	#define VK_GET_PROC_ADDR(proc_name)													\
		proc_name = (PFN_##proc_name) vkGetDeviceProcAddr(device, #proc_name);			\
		if(proc_name == NULL){															\
			LOG_ERROR("VK_LoadDeviceAPI: Failed to load '%s'", #proc_name);				\
			ret = VK_FALSE;																\
		}

	// load core
	ret = VK_TRUE;
''')

	for name in VK_CORE[VK_DEVICE]:
		f.write(("\tVK_GET_PROC_ADDR(%s);\n" % name).encode("utf-8"));

	f.write(br'''

	if(ret == VK_FALSE){
		LOG_ERROR("VK_LoadDeviceAPI: Failed to load device core procs.");
		return VK_FALSE;
	}

	// load extensions
	for(i = 0; i < enabledExtensionCount; i++){
''')


	if_str = "if"
	first = True
	for ext, proc_list in VK_EXT_DEVICE_ITEMS:
		f.write((r'''
		%s(strcmp(ppEnabledExtensions[i], "%s") == 0){
			ret = VK_TRUE;
''' % (if_str,
		VK_EXT_NAME[ext]
		)).encode("utf-8"))

		for name in proc_list:
			f.write(("\t\t\tVK_GET_PROC_ADDR(%s);\n" % name).encode("utf-8"))

		f.write((r'''
			vk_info.%s = ret;
			if(ret == VK_FALSE)
				LOG_WARNING("VK_LoadDeviceAPI: Failed to load extension '%%s'", "%s");
		}
''' % (VK_EXT_NAME[ext][3:],
		VK_EXT_NAME[ext]
		)).encode("utf-8"))

		if first:
			if_str = "else if"
			first = False

	f.write(br'''
	}

	#undef VK_GET_PROC_ADDR
	//=================================================================

	l_state = 3;
	return VK_TRUE;
}

void VK_UnloadAPI()
{
	VK_CloseLib();
	l_state = 0;
}

''')




