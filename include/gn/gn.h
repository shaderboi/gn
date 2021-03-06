#ifndef GN_H_
#define GN_H_

#ifdef __cplusplus
#include <functional>
#endif

#define GN_OUT
#define GN_MAX_CHARS 256
#define GN_ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))
#define GN_TRUE 1
#define GN_FALSE 0
#define GN_FAILED(x) ((x) < GnSuccess)

#if defined(_WIN32)
#define GN_FPTR __stdcall
#elif defined(__linux__)
#define GN_FPTR
#else
#define GN_FPTR
#endif

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct GnInstance_t* GnInstance;
typedef struct GnAdapter_t* GnAdapter;
typedef struct GnDevice_t* GnDevice;
typedef struct GnQueue_t* GnQueue;
typedef struct GnFence_t* GnFence;
typedef struct GnBuffer_t* GnBuffer;
typedef struct GnTexture_t* GnTexture;
typedef struct GnResourceTableLayout_t* GnResourceTableLayout;
typedef struct GnPipelineLayout_t* GnPipelineLayout;
typedef struct GnPipeline_t* GnPipeline;
typedef struct GnResourceTablePool_t* GnResourceTablePool;
typedef struct GnResourceTable_t* GnResourceTable;
typedef struct GnCommandPool_t* GnCommandPool;
typedef struct GnCommandList_t* GnCommandList;

typedef uint32_t GnBool;
typedef uint64_t GnDeviceSize;

typedef enum
{
    GnSuccess,
    GnError_Unknown,
    GnError_Unimplemented,
    GnError_InitializationFailed,
    GnError_BackendNotAvailable,
    GnError_NoAdapterAvailable,
    GnError_UnsupportedFeature,
    GnError_InternalError,
    GnError_OutOfHostMemory,
} GnResult;

typedef enum
{
    GnBackend_Auto,
    GnBackend_D3D11,
    GnBackend_D3D12,
    GnBackend_Vulkan,
    GnBackend_Count,
} GnBackend;

typedef enum
{
    GnAllocationScope_Command = 0,
    GnAllocationScope_Object = 1,
    GnAllocationScope_Device = 2,
    GnAllocationScope_Instance = 3,
    GnAllocationScope_Count,
} GnAllocationScope;

typedef void* (*GnMallocFn)(void* userdata, size_t size, size_t alignment, GnAllocationScope scope);
typedef void* (*GnReallocFn)(void* userdata, void* original, size_t size, size_t alignment, GnAllocationScope scope);
typedef void (*GnFreeFn)(void* userdata, void* memory);
typedef void (*GnGetAdapterCallbackFn)(void* userdata, GnAdapter adapter);

typedef struct
{
    void*       userdata;
    GnMallocFn  malloc_fn;
    GnReallocFn realloc_fn;
    GnFreeFn    free_fn;
} GnAllocationCallbacks;

typedef struct
{
    GnBackend   backend;
    GnBool      enable_debugging;
    GnBool      enable_validation;
    GnBool      enable_backend_validation;
} GnInstanceDesc;

GnResult GnCreateInstance(const GnInstanceDesc* desc, const GnAllocationCallbacks* alloc_callbacks, GN_OUT GnInstance* instance);
void GnDestroyInstance(GnInstance instance);
GnAdapter GnGetDefaultAdapter(GnInstance instance);
uint32_t GnGetAdapterCount(GnInstance instance);
uint32_t GnGetAdapters(GnInstance instance, uint32_t num_adapters, GN_OUT GnAdapter* adapters);
uint32_t GnGetAdaptersWithCallback(GnInstance instance, void* userdata, GnGetAdapterCallbackFn callback_fn);
GnBackend GnGetBackend(GnInstance instance);

typedef enum
{
    GnAdapterType_Unknown,
    GnAdapterType_Discrete,
    GnAdapterType_Integrated,
    GnAdapterType_Software,
    GnAdapterType_Count,
} GnAdapterType;

typedef enum
{
    GnFeature_FullDrawIndexRange32Bit,
    GnFeature_TextureCubeArray,
    GnFeature_NativeMultiDrawIndirect,
    GnFeature_DrawIndirectFirstInstance,
    GnFeature_Count,
} GnFeature;

typedef enum
{
    GnFormat_Unknown,

    // [Texture Format]
    // 8-bpc texture formats
    GnFormat_R8Unorm,
    GnFormat_R8Snorm,
    GnFormat_R8Uint,
    GnFormat_R8Sint,
    GnFormat_RG8Unorm,
    GnFormat_RG8Snorm,
    GnFormat_RG8Uint,
    GnFormat_RG8Sint,
    GnFormat_RGBA8Srgb,
    GnFormat_RGBA8Unorm,
    GnFormat_RGBA8Snorm,
    GnFormat_RGBA8Uint,
    GnFormat_RGBA8Sint,
    GnFormat_BGRA8Unorm,
    GnFormat_BGRA8Srgb,

    // 16-bpc texture formats
    GnFormat_R16Uint,
    GnFormat_R16Sint,
    GnFormat_R16Float,
    GnFormat_RG16Uint,
    GnFormat_RG16Sint,
    GnFormat_RG16Float,
    GnFormat_RGBA16Uint,
    GnFormat_RGBA16Sint,
    GnFormat_RGBA16Float,

    // 32-bpc texture formats
    GnFormat_R32Uint,
    GnFormat_R32Sint,
    GnFormat_R32Float,
    GnFormat_RG32Uint,
    GnFormat_RG32Sint,
    GnFormat_RG32Float,
    GnFormat_RGB32Uint,
    GnFormat_RGB32Sint,
    GnFormat_RGB32Float,
    GnFormat_RGBA32Uint,
    GnFormat_RGBA32Sint,
    GnFormat_RGBA32Float,
    GnFormat_Count,

    // Vertex Formats
    // 8-bpc vertex formats
    GnFormat_Unorm8,
    GnFormat_Unorm8x2,
    GnFormat_Unorm8x4,
    GnFormat_Snorm8,
    GnFormat_Snorm8x2,
    GnFormat_Snorm8x4,
    GnFormat_Uint8,
    GnFormat_Uint8x2,
    GnFormat_Uint8x4,
    GnFormat_Sint8,
    GnFormat_Sint8x2,
    GnFormat_Sint8x4,

    // 16-bpc vertex formats
    GnFormat_Float16    = GnFormat_R16Float,
    GnFormat_Float16x2  = GnFormat_RG16Float,
    GnFormat_Float16x4  = GnFormat_RGBA16Float,
    GnFormat_Uint16     = GnFormat_R16Uint,
    GnFormat_Uint16x2   = GnFormat_RG16Uint,
    GnFormat_Uint16x4   = GnFormat_RGBA16Uint,
    GnFormat_Sint16     = GnFormat_R16Sint,
    GnFormat_Sint16x2   = GnFormat_RG16Sint,
    GnFormat_Sint16x4   = GnFormat_RGBA16Sint,

    // 32-bpc vertex formats
    GnFormat_Float32    = GnFormat_R32Float,
    GnFormat_Float32x2  = GnFormat_RG32Float,
    GnFormat_Float32x3  = GnFormat_RGB32Float,
    GnFormat_Float32x4  = GnFormat_RGBA32Float,
    GnFormat_Uint32     = GnFormat_R32Uint,
    GnFormat_Uint32x2   = GnFormat_RG32Uint,
    GnFormat_Uint32x3   = GnFormat_RGB32Uint,
    GnFormat_Uint32x4   = GnFormat_RGBA32Uint,
    GnFormat_Sint32     = GnFormat_R32Sint,
    GnFormat_Sint32x2   = GnFormat_RG32Sint,
    GnFormat_Sint32x3   = GnFormat_RGB32Sint,
    GnFormat_Sint32x4   = GnFormat_RGBA32Sint,
} GnFormat;

typedef enum
{
    GnTextureFormatFeature_CopySrc                  = 1 << 0,
    GnTextureFormatFeature_CopyDst                  = 1 << 1,
    GnTextureFormatFeature_BlitSrc                  = 1 << 2,
    GnTextureFormatFeature_BlitDst                  = 1 << 3,
    GnTextureFormatFeature_Sampled                  = 1 << 4,
    GnTextureFormatFeature_LinearFilterable         = 1 << 5,
    GnTextureFormatFeature_StorageRead              = 1 << 6,
    GnTextureFormatFeature_StorageWrite             = 1 << 7,
    GnTextureFormatFeature_ColorAttachment          = 1 << 8,
    GnTextureFormatFeature_ColorAttachmentBlending  = 1 << 9,
    GnTextureFormatFeature_DepthStencilAttachment   = 1 << 10,
} GnTextureFormatFeature;
typedef uint32_t GnTextureFormatFeatureFlags;

typedef enum
{
    GnQueueType_Direct,
    GnQueueType_Compute,
    GnQueueType_Copy,
    GnQueueType_Count,
} GnQueueType;

typedef struct
{
    char            name[GN_MAX_CHARS];
    uint32_t        vendor_id;
    GnAdapterType   type;
} GnAdapterProperties;

typedef struct
{
    uint32_t max_texture_size_1d;
    uint32_t max_texture_size_2d;
    uint32_t max_texture_size_3d;
    uint32_t max_texture_size_cube;
    uint32_t max_texture_array_layers;
    uint32_t max_uniform_buffer_range;
    uint32_t max_storage_buffer_range;
    uint32_t max_shader_constant_size;
    uint32_t max_bound_pipeline_layout_slots;
    uint32_t max_per_stage_sampler_resources;
    uint32_t max_per_stage_uniform_buffer_resources;
    uint32_t max_per_stage_storage_buffer_resources;
    uint32_t max_per_stage_read_only_storage_buffer_resources;
    uint32_t max_per_stage_sampled_texture_resources;
    uint32_t max_per_stage_storage_texture_resources;
    uint32_t max_per_stage_resources;
    uint32_t max_resource_table_samplers;
    uint32_t max_resource_table_uniform_buffers;
    uint32_t max_resource_table_storage_buffers;
    uint32_t max_resource_table_read_only_storage_buffer_resources;
    uint32_t max_resource_table_sampled_textures;
    uint32_t max_resource_table_storage_textures;
} GnAdapterLimits;

typedef struct
{
    uint32_t    id;
    GnQueueType type;
    GnBool      timestamp_query_supported;
} GnQueueProperties;

typedef void (*GnGetAdapterFeatureCallbackFn)(void* userdata, GnFeature feature);
typedef void (*GnGetAdapterQueuePropertiesCallbackFn)(void* userdata, const GnQueueProperties* queue_properties);

void GnGetAdapterProperties(GnAdapter adapter, GN_OUT GnAdapterProperties* properties);
void GnGetAdapterLimits(GnAdapter adapter, GN_OUT GnAdapterLimits* limits);
uint32_t GnGetAdapterFeatureCount(GnAdapter adapter);
uint32_t GnGetAdapterFeatures(GnAdapter adapter, uint32_t num_features, GnFeature* features);
uint32_t GnGetAdapterFeaturesWithCallback(GnAdapter adapter, void* userdata, GnGetAdapterFeatureCallbackFn callback_fn);
GnBool GnIsAdapterFeaturePresent(GnAdapter adapter, GnFeature feature);
GnTextureFormatFeatureFlags GnGetTextureFormatFeatureSupport(GnAdapter adapter, GnFormat format);
GnBool GnIsVertexFormatSupported(GnAdapter adapter, GnFormat format);
uint32_t GnGetAdapterQueueCount(GnAdapter adapter);
uint32_t GnGetAdapterQueueProperties(GnAdapter adapter, uint32_t num_queues, GnQueueProperties* queue_properties);
uint32_t GnGetAdapterQueuePropertiesWithCallback(GnAdapter adapter, void* userdata, GnGetAdapterQueuePropertiesCallbackFn callback_fn);

typedef struct
{
    uint32_t num_enabled_queues;
    const uint32_t* enabled_queue_ids;
    uint32_t num_enabled_features;
    const GnFeature* enabled_features;
} GnDeviceDesc;

GnResult GnCreateDevice(GnAdapter adapter, const GnDeviceDesc* desc, const GnAllocationCallbacks* alloc_callbacks, GN_OUT GnDevice* device);
void GnDestroyDevice(GnDevice device);

typedef struct
{
    uint32_t                num_wait_fences;
    const GnFence*          wait_fences;
    uint32_t                num_command_lists;
    const GnCommandList*    command_lists;
    uint32_t                num_signal_fences;
    const GnFence*          signal_fences;
} GnSubmission;

GnResult GnCreateQueue(GnDevice device, uint32_t queue_index, const GnAllocationCallbacks* alloc_callbacks, GN_OUT GnQueue* queue);
void GnDestroyQueue(GnQueue queue);
GnResult GnQueueSubmit(GnQueue queue, uint32_t num_submissions, GnSubmission* submissions, GnFence signal_host_fence);
GnResult GnQueueSubmitAndWait(GnQueue queue, uint32_t num_submissions, GnSubmission* submissions);
GnResult GnQueuePresent(GnQueue queue);
void GnWaitQueue(GnQueue queue);

typedef enum
{
    GnFence_DeviceToDeviceSync,
    GnFence_DeviceToHostSync,
} GnFenceType;

GnResult GnCreateFence(GnDevice device, GnFenceType type, bool signaled, const GnAllocationCallbacks* alloc_callbacks, GN_OUT GnFence* fence);
void GnDestroyFence(GnFence fence);
GnResult GnGetFenceStatus(GnFence fence);
GnResult GnWaitFence(GnFence fence, uint64_t timeout);
void GnResetFence(GnFence fence);
GnFenceType GnGetFenceType(GnFence fence);

typedef enum
{
    GnBufferUsage_CopySrc   = 1 << 0,
    GnBufferUsage_CopyDst   = 1 << 1,
    GnBufferUsage_Uniform   = 1 << 2,
    GnBufferUsage_Index     = 1 << 3,
    GnBufferUsage_Vertex    = 1 << 4,
    GnBufferUsage_Storage   = 1 << 5,
    GnBufferUsage_Indirect  = 1 << 6,
} GnBufferUsage;
typedef uint32_t GnBufferUsageFlags;

typedef struct
{
    GnDeviceSize        size;
    GnBufferUsageFlags  usage;
} GnBufferDesc;

GnResult GnCreateBuffer(GnDevice device, const GnBufferDesc* desc, GN_OUT GnBuffer* buffer);
void GnDestroyBuffer(GnBuffer buffer);
void GnGetBufferDesc(GnBuffer buffer, GN_OUT GnBufferDesc* texture_desc);

typedef enum
{
    GnTextureType_1D,
    GnTextureType_2D,
    GnTextureType_3D,
} GnTextureType;

typedef enum
{
    GnTextureUsage_CopySrc = 1 << 0,
    GnTextureUsage_CopyDst = 1 << 1,
    GnTextureUsage_BlitSrc = 1 << 2,
    GnTextureUsage_BlitDst = 1 << 3,
    GnTextureUsage_Sampled = 1 << 4,
    GnTextureUsage_Storage = 1 << 5,
    GnTextureUsage_ColorAttachment = 1 << 6,
    GnTextureUsage_DepthStencilAttachment = 1 << 7,
} GnTextureUsage;
typedef uint32_t GnTextureUsageFlags;

typedef struct
{
    GnTextureUsageFlags usage;
    GnTextureType       type;
    GnFormat            format;
    uint32_t            width;
    uint32_t            height;
    uint32_t            depth;
    uint32_t            mip_levels;
    uint32_t            array_size;
    uint32_t            num_samples;
} GnTextureDesc;

GnResult GnCreateTexture(GnDevice device, const GnTextureDesc* desc, GN_OUT GnTexture* texture);
void GnDestroyTexture(GnTexture texture);
void GnGetTextureDesc(GnTexture texture, GN_OUT GnTextureDesc* texture_desc);

typedef enum
{
    GnCommandPoolUsage_Transient                = 1 << 0,
    GnCommandPoolUsage_AllowResetCommandLists   = 1 << 1
} GnCommandPoolUsage;
typedef uint32_t GnCommandPoolUsageFlags;

typedef enum
{
    GnCommandListUsage_Primary              = 0,
    GnCommandListUsage_BundleDraw           = 1 << 0,
    GnCommandListUsage_BundleDrawIndexed    = 1 << 1,
    GnCommandListUsage_BundleDispatch       = 1 << 2,
} GnCommandListUsage;
typedef uint32_t GnCommandListUsageFlags;

typedef struct
{
    uint32_t                queue_group_index;
    GnCommandPoolUsageFlags usage;
    uint32_t                max_allocated_cmd_list;
} GnCommandPoolDesc;

typedef struct
{
    GnCommandPool               command_pool;
    GnCommandListUsageFlags     usage;
    uint32_t                    num_cmd_lists;
} GnCommandListDesc;

GnResult GnCreateCommandPool(GnDevice device, const GnCommandPoolDesc* desc, GN_OUT GnCommandPool* command_pool);
void GnDestroyCommandPool(GnCommandPool command_pool);
void GnTrimCommandPool(GnCommandPool command_pool);

typedef enum
{
    GnCommandListBegin_OneTimeSubmit = 1 << 0,
    GnCommandListBegin_RenderPassContinue = 1 << 1,
    GnCommandListBegin_SimultaneousUse = 1 << 2,
} GnCommandListBegin;
typedef uint32_t GnCommandListBeginFlags;

typedef struct
{
    
} GnCommandListInheritance;

typedef struct
{
    GnCommandListBeginFlags         flags;
    const GnCommandListInheritance* inheritance;
} GnCommandListBeginDesc;

GnResult GnCreateCommandList(GnDevice device, GnCommandPool command_pool, uint32_t num_cmd_lists, GN_OUT GnCommandList* command_lists);
void GnDestroyCommandList(GnCommandPool command_pool, uint32_t num_cmd_lists, const GnCommandList* command_lists);
GnResult GnBeginCommandList(GnCommandList command_list, const GnCommandListBeginDesc* desc);
GnResult GnEndCommandList(GnCommandList command_list);
GnBool GnIsRecordingCommandList(GnCommandList command_list);

typedef enum
{
    GnResourceAccess_IndirectCommandRead            = 1 << 0,
    GnResourceAccess_IndexRead                      = 1 << 1,
    GnResourceAccess_VertexAttributeRead            = 1 << 2,
    GnResourceAccess_UniformRead                    = 1 << 3,
    GnResourceAccess_InputAttachmentRead            = 1 << 4,
    GnResourceAccess_ShaderRead                     = 1 << 5,
    GnResourceAccess_ShaderWrite                    = 1 << 6,
    GnResourceAccess_ColorAttachmentRead            = 1 << 7,
    GnResourceAccess_ColorAttachmentWrite           = 1 << 8,
    GnResourceAccess_DepthStencilAttachmentRead     = 1 << 9,
    GnResourceAccess_DepthStencilAttachmentWrite    = 1 << 10,
    GnResourceAccess_CopyRead                       = 1 << 11,
    GnResourceAccess_CopyWrite                      = 1 << 12,
    GnResourceAccess_HostRead                       = 1 << 13,
    GnResourceAccess_HostWrite                      = 1 << 14,
    GnResourceAccess_AllRead                        = 1 << 15,
    GnResourceAccess_AllWrite                       = 1 << 16,
} GnResourceAccess;

typedef enum
{
    GnTextureLayout_Undefined               = 0,
    GnTextureLayout_General                 = 1,
    GnTextureLayout_ColorAttachment         = 2,
    GnTextureLayout_DepthStencilAttachment  = 3,
    GnTextureLayout_DepthStencilReadOnly    = 4,
    GnTextureLayout_ShaderReadOnly          = 5,
    GnTextureLayout_Storage                 = 6,
    GnTextureLayout_CopySrc                 = 7,
    GnTextureLayout_CopyDst                 = 8,
    GnTextureLayout_ClearDst                = 9,
    GnTextureLayout_Present                 = 10,
} GnTextureLayout;

typedef struct
{
    float x;
    float y;
    float width;
    float height;
    float min_depth;
    float max_depth;
} GnViewport;

typedef struct
{
    uint32_t x;
    uint32_t y;
    uint32_t width;
    uint32_t height;
} GnScissorRect;

void GnCmdSetGraphicsPipeline(GnCommandList command_list, GnPipeline graphics_pipeline);
void GnCmdSetGraphicsPipelineLayout(GnCommandList command_list, GnPipelineLayout layout);
void GnCmdSetGraphicsResourceTable(GnCommandList command_list, uint32_t slot, GnResourceTable resource_table);
void GnCmdSetGraphicsUniformBuffer(GnCommandList command_list, uint32_t slot, GnBuffer uniform_buffer, GnDeviceSize offset);
void GnCmdSetGraphicsStorageBuffer(GnCommandList command_list, uint32_t slot, GnBuffer storage_buffer, GnDeviceSize offset);
void GnCmdSetGraphicsShaderConstants(GnCommandList command_list, uint32_t first_slot, uint32_t size, const void* data, uint32_t offset);
void GnCmdSetIndexBuffer(GnCommandList command_list, GnBuffer index_buffer, GnDeviceSize offset);
void GnCmdSetVertexBuffer(GnCommandList command_list, uint32_t slot, GnBuffer vertex_buffer, GnDeviceSize offset);
void GnCmdSetVertexBuffers(GnCommandList command_list, uint32_t first_slot, uint32_t num_vertex_buffers, const GnBuffer* vertex_buffer, const GnDeviceSize* offsets);
void GnCmdSetViewport(GnCommandList command_list, uint32_t slot, float x, float y, float width, float height, float min_depth, float max_depth);
void GnCmdSetViewport2(GnCommandList command_list, uint32_t slot, const GnViewport* viewport);
void GnCmdSetViewports(GnCommandList command_list, uint32_t first_slot, uint32_t num_viewports, const GnViewport* viewports);
void GnCmdSetScissor(GnCommandList command_list, uint32_t slot, uint32_t x, uint32_t y, uint32_t width, uint32_t height);
void GnCmdSetScissor2(GnCommandList command_list, uint32_t slot, const GnScissorRect* scissor);
void GnCmdSetScissors(GnCommandList command_list, uint32_t first_slot, uint32_t num_scissors, const GnScissorRect* scissors);
void GnCmdSetBlendConstants(GnCommandList command_list, const float blend_constants[4]);
void GnCmdSetBlendConstants2(GnCommandList command_list, float r, float g, float b, float a);
void GnCmdSetStencilRef(GnCommandList command_list, uint32_t stencil_ref);
void GnCmdBeginRenderPass(GnCommandList command_list);
void GnCmdDraw(GnCommandList command_list, uint32_t num_vertices, uint32_t first_vertex);
void GnCmdDrawInstanced(GnCommandList command_list, uint32_t num_vertices, uint32_t num_instances, uint32_t first_vertex, uint32_t first_instance);
void GnCmdDrawIndirect(GnCommandList command_list, GnBuffer indirect_buffer, GnDeviceSize offset, uint32_t num_indirect_commands);
void GnCmdDrawIndexed(GnCommandList command_list, uint32_t num_indices, uint32_t first_index, int32_t vertex_offset);
void GnCmdDrawIndexedInstanced(GnCommandList command_list, uint32_t num_indices, uint32_t first_index, uint32_t num_instances, int32_t vertex_offset, uint32_t first_instance);
void GnCmdDrawIndexedIndirect(GnCommandList command_list, GnBuffer indirect_buffer, GnDeviceSize offset, uint32_t num_indirect_commands);
void GnCmdEndRenderPass(GnCommandList command_list);
void GnCmdSetComputePipeline(GnCommandList command_list, GnPipeline graphics_pipeline);
void GnCmdSetComputePipelineLayout(GnCommandList command_list, GnPipelineLayout layout);
void GnCmdSetComputeResourceTable(GnCommandList command_list, uint32_t slot, GnResourceTable resource_table);
void GnCmdSetComputeUniformBuffer(GnCommandList command_list, uint32_t slot, GnBuffer uniform_buffer, GnDeviceSize offset);
void GnCmdSetComputeStorageBuffer(GnCommandList command_list, uint32_t slot, GnBuffer storage_buffer, GnDeviceSize offset);
void GnCmdSetComputeShaderConstants(GnCommandList command_list, uint32_t first_slot, uint32_t size, const void* data, uint32_t offset);
void GnCmdDispatch(GnCommandList command_list, uint32_t num_thread_group_x, uint32_t num_thread_group_y, uint32_t num_thread_group_z);
void GnCmdDispatchIndirect(GnCommandList command_list, GnBuffer indirect_buffer, GnDeviceSize offset);
void GnCmdCopyBuffer(GnCommandList command_list, GnBuffer src_buffer, GnDeviceSize src_offset, GnBuffer dst_buffer, GnDeviceSize dst_offset, GnDeviceSize size);
void GnCmdCopyTexture(GnCommandList command_list, GnTexture src_texture, GnTexture dst_texture);
void GnCmdCopyBufferToTexture(GnCommandList command_list, GnBuffer src_buffer, GnTexture dst_texture);
void GnCmdCopyTextureToBuffer(GnCommandList command_list, GnTexture src_texture, GnBuffer dst_buffer);
void GnCmdBlitTexture(GnCommandList command_list, GnTexture src_texture, GnTexture dst_texture);
void GnCmdBarrier(GnCommandList command_list);
void GnCmdExecuteBundles(GnCommandList command_list, uint32_t num_bundles, const GnCommandList* bundles);

// [HELPERS]

typedef struct
{
    // These variables must not be used by the application directly!
    GnInstance  _instance;
    uint32_t    _current_adapter;
} GnAdapterQuery;

typedef GnBool (*GnAdapterQueryLimitConstraintsFn)(const GnAdapterLimits* limits);

void GnInitAdapterQuery(GnInstance instance, GN_OUT GnAdapterQuery* adapter_query);
void GnQueryAdapterWithType(GnAdapterQuery* query, GnAdapterType type);
void GnQueryAdapterWithVendorID(GnAdapterQuery* query, uint32_t vendor_id);
void GnQueryAdapterWithFeature(GnAdapterQuery* query, GnFeature feature);
void GnQueryAdapterWithFeatures(GnAdapterQuery* query, uint32_t num_features, GnFeature* features);
void GnQueryAdapterWithLimitConstraints(GnAdapterQuery* query, GnAdapterQueryLimitConstraintsFn limit_constraints_fn);
void GnFetchAdapters(const GnAdapterQuery* query, void* userdata, GnGetAdapterCallbackFn callback_fn);
GnBool GnFetchNextAdapter(GnAdapterQuery* query, GN_OUT GnAdapter* adapter);
GnAdapter GnFirstAdapter(const GnAdapterQuery* query);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
// C++ std::function wrapper for GnGetAdaptersWithCallback
static uint32_t GnGetAdaptersWithCallback(GnInstance instance, std::function<void(GnAdapter)> callback_fn)
{
    auto wrapper_fn = [](void* userdata, GnAdapter adapter) {
        const auto& fn = *static_cast<std::function<void(GnAdapter)>*>(userdata);
        fn(adapter);
    };

    return GnGetAdaptersWithCallback(instance, static_cast<void*>(&callback_fn), wrapper_fn);
}

// C++ std::function wrapper for GnGetAdapterFeaturesWithCallback
static uint32_t GnGetAdapterFeaturesWithCallback(GnAdapter adapter, std::function<void(GnFeature)> callback_fn)
{
    auto wrapper_fn = [](void* userdata, GnFeature feature) {
        const auto& fn = *static_cast<std::function<void(GnFeature)>*>(userdata);
        fn(feature);
    };

    return GnGetAdapterFeaturesWithCallback(adapter, static_cast<void*>(&callback_fn), wrapper_fn);
}

static uint32_t GnGetAdapterQueuePropertiesWithCallback(GnAdapter adapter, std::function<void(const GnQueueProperties&)> callback_fn)
{
    auto wrapper_fn = [](void* userdata, const GnQueueProperties* queue_properties) {
        const auto& fn = *static_cast<std::function<void(const GnQueueProperties&)>*>(userdata);
        fn(*queue_properties);
    };

    return GnGetAdapterQueuePropertiesWithCallback(adapter, static_cast<void*>(&callback_fn), wrapper_fn);
}
#endif

#endif // GN_H_