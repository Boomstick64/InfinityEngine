#ifdef PLATFORM_WINDOWS

#include "DirectX.h"

#include "d3d12.h"
#include "dxgi.h"
#include "dxgi1_4.h"
#include "d3dcompiler.h"

#include WINDOW_FILE

void DirectX::Init()
{
	if (Window == nullptr) return;

	TriangleAngle = 0;

	// The device is like a virtual representation of the GPU
	ID3D12Device* Device = nullptr;
	HRESULT Hr = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&Device));

	// Command queue decides which order the command lists should execute. In our case, we only have one command line
	CommandQueue = nullptr;
	D3D12_COMMAND_QUEUE_DESC QueueDesc = {};
	QueueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	QueueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	Hr = Device->CreateCommandQueue(&QueueDesc, IID_PPV_ARGS(&CommandQueue));

	// Command allocator is used to allocate memory on the GPU for commands
	CommandAllocator = nullptr;
	Hr = Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&CommandAllocator));
	Hr = CommandAllocator->Reset();

	// Command list is used to store a list of commands that we want to execute on the GPU
	CommandList = nullptr;
	Hr = Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CommandAllocator, nullptr, IID_PPV_ARGS(&CommandList));
	Hr = CommandList->Close();

	// Helper object to create a swap chain
	IDXGIFactory4* Factory = nullptr;
	Hr = CreateDXGIFactory1(IID_PPV_ARGS(&Factory));

	// Create swap chain
	DXGI_SWAP_CHAIN_DESC SwapChainDesc = {};
	SwapChainDesc.BufferCount = 2;
	SwapChainDesc.BufferDesc.Width = Window->GetWindowWidth();
	SwapChainDesc.BufferDesc.Height = Window->GetWindowHeight();
	SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	SwapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	SwapChainDesc.OutputWindow = Window->WindowHandle;
	SwapChainDesc.SampleDesc.Count = 1;
	SwapChainDesc.Windowed = TRUE;

	IDXGISwapChain* TempSwapChain = nullptr;
	Hr = Factory->CreateSwapChain(CommandQueue, &SwapChainDesc, &TempSwapChain);

	// Cast the swap chain to IDXGISwapChain3 to leverage latest features
	SwapChain = {};
	Hr = TempSwapChain->QueryInterface(IID_PPV_ARGS(&SwapChain));
	TempSwapChain->Release();
	TempSwapChain = nullptr;

	// Memory descriptor heap to store render target views(RTV). Descriptor describes how to interperate resource memory.
	RTVHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC RTVHeapDesc = {};
	RTVHeapDesc.NumDescriptors = 2;
	RTVHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	Hr = Device->CreateDescriptorHeap(&RTVHeapDesc, IID_PPV_ARGS(&RTVHeap));

	RTVIncrementSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	{
		D3D12_CPU_DESCRIPTOR_HANDLE RTVHandle(RTVHeap->GetCPUDescriptorHandleForHeapStart());
		for (UINT i = 0; i < 2; i++)
		{
			Hr = SwapChain->GetBuffer(i, IID_PPV_ARGS(&RenderTargets[i]));

			Device->CreateRenderTargetView(RenderTargets[i], nullptr, RTVHandle);
			RTVHandle.ptr += RTVIncrementSize;
		}
	}

	// Fence is used to synchronize the CPU and GPU, so they don't touch the same memory at the same time
	Fence = nullptr;
	FenceValue = 0;
	Hr = Device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&Fence));

	FenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	// Root signature is like have many object buffers and textures we want to use when drawing.
	// For our rotating triangle, we only need a single constant that is going to be our angle
	D3D12_ROOT_PARAMETER RootParameters[1] = {};
	RootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_32BIT_CONSTANTS;
	RootParameters[0].Constants.Num32BitValues = 1;
	RootParameters[0].Constants.ShaderRegister = 0;
	RootParameters[0].Constants.RegisterSpace = 0;
	RootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;

	RootSignature = nullptr;
	D3D12_ROOT_SIGNATURE_DESC RootSignatureDesc = {};
	RootSignatureDesc.NumParameters = _countof(RootParameters);
	RootSignatureDesc.pParameters = RootParameters;
	RootSignatureDesc.NumStaticSamplers = 0;
	RootSignatureDesc.pStaticSamplers = nullptr;
	RootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	ID3DBlob* SignatureBlob = nullptr;
	ID3DBlob* ErrorBlob = nullptr;
	Hr = D3D12SerializeRootSignature(&RootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &SignatureBlob, &ErrorBlob);
	Hr = Device->CreateRootSignature(0, SignatureBlob->GetBufferPointer(), SignatureBlob->GetBufferSize(), IID_PPV_ARGS(&RootSignature));

	if (SignatureBlob)
	{
		SignatureBlob->Release();
		SignatureBlob = nullptr;
	}

	if (ErrorBlob)
	{
		ErrorBlob->Release();
		ErrorBlob = nullptr;
	}

	

	// Compile shaders
	ID3DBlob* VertexShader = nullptr;
	ID3DBlob* PixelShader = nullptr;

	// This uses the working directory for the path, which is currently set to be inside intermediate/projectfiles/InfinityEngine
	Hr = D3DCompileFromFile(L"../../../Engine/Source/Graphics/Shaders/shader.hlsl", nullptr, nullptr, "VSMain", "vs_5_0", 0, 0, &VertexShader, nullptr);
	Hr = D3DCompileFromFile(L"../../../Engine/Source/Graphics/Shaders/shader.hlsl", nullptr, nullptr, "PSMain", "ps_5_0", 0, 0, &PixelShader, nullptr);

	// Pipeline state
	D3D12_GRAPHICS_PIPELINE_STATE_DESC PSODesc = {};
	PSODesc.pRootSignature = RootSignature;
	PSODesc.VS.pShaderBytecode = VertexShader->GetBufferPointer();
	PSODesc.VS.BytecodeLength = VertexShader->GetBufferSize();
	PSODesc.PS.pShaderBytecode = PixelShader->GetBufferPointer();
	PSODesc.PS.BytecodeLength = PixelShader->GetBufferSize();
	SetBlendState(PSODesc.BlendState);
	PSODesc.SampleMask = UINT_MAX;
	SetRasterizerState(PSODesc.RasterizerState);
	SetDepthStencilState(PSODesc.DepthStencilState);
	PSODesc.InputLayout.pInputElementDescs = nullptr;
	PSODesc.InputLayout.NumElements = 0;
	PSODesc.IBStripCutValue = D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
	PSODesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	PSODesc.NumRenderTargets = 1;
	PSODesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	PSODesc.SampleDesc.Count = 1;
	PSODesc.SampleDesc.Quality = 0;

	PipelineState = nullptr;
	Hr = Device->CreateGraphicsPipelineState(&PSODesc, IID_PPV_ARGS(&PipelineState));

	VertexShader->Release();
	VertexShader = nullptr;
	PixelShader->Release();
	PixelShader = nullptr;
}

void DirectX::Update()
{
	HRESULT Hr = CommandAllocator->Reset();
	Hr = CommandList->Reset(CommandAllocator, nullptr);

	UINT BackBufferIndex = SwapChain->GetCurrentBackBufferIndex();
	{
		D3D12_RESOURCE_BARRIER Barrier = {};
		Barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		Barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		Barrier.Transition.pResource = RenderTargets[BackBufferIndex];
		Barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		Barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		Barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		CommandList->ResourceBarrier(1, &Barrier);
	}

	D3D12_CPU_DESCRIPTOR_HANDLE RTVHandle = RTVHeap->GetCPUDescriptorHandleForHeapStart();
	RTVHandle.ptr += BackBufferIndex * RTVIncrementSize;

	// Clear the render target
	float ClearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	CommandList->ClearRenderTargetView(RTVHandle, ClearColor, 0, nullptr);

	// Set viewport and scissor
	D3D12_VIEWPORT Viewport = { 0.0f, 0.0f, static_cast<float>(Window->GetWindowWidth()), static_cast<float>(Window->GetWindowHeight()), 0.0f, 1.0f };
	D3D12_RECT ScissorRect = { 0, 0, LONG_MAX, LONG_MAX };
	CommandList->RSSetViewports(1, &Viewport);
	CommandList->RSSetScissorRects(1, &ScissorRect);

	CommandList->OMSetRenderTargets(1, &RTVHandle, FALSE, nullptr);
	CommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CommandList->SetGraphicsRootSignature(RootSignature);
	CommandList->SetPipelineState(PipelineState);

	CommandList->SetGraphicsRoot32BitConstant(0, TriangleAngle, 0);
	CommandList->DrawInstanced(3, 1, 0, 0);

	{
		D3D12_RESOURCE_BARRIER Barrier = {};
		Barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		Barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		Barrier.Transition.pResource = RenderTargets[BackBufferIndex];
		Barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		Barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		Barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		CommandList->ResourceBarrier(1, &Barrier);
	}

	Hr = CommandList->Close();

	ID3D12CommandList* CommandLists[] = { CommandList };
	CommandQueue->ExecuteCommandLists(1, CommandLists);

	Hr = SwapChain->Present(1, 0);

	// Wait on the CPU for the GPU frame to finish
	const UINT64 CurrentFenceValue = ++FenceValue;
	Hr = CommandQueue->Signal(Fence, CurrentFenceValue);

	if (Fence->GetCompletedValue() < CurrentFenceValue)
	{
		Hr = Fence->SetEventOnCompletion(CurrentFenceValue, FenceEvent);
		WaitForSingleObject(FenceEvent, INFINITE);
	}

	TriangleAngle++;
}

DefaultWindow* DirectX::GetWindowInterface()
{
	return Window;
}

void DirectX::SetWindowInterface(DefaultWindow* WindowInterface)
{
	Window = WindowInterface;
}

void DirectX::SetBlendState(D3D12_BLEND_DESC& BlendDesc)
{
	BlendDesc = {};

	BlendDesc.AlphaToCoverageEnable = FALSE;
	BlendDesc.IndependentBlendEnable = FALSE;

	D3D12_RENDER_TARGET_BLEND_DESC DefaultRenderTargetBlendDesc = {};
	DefaultRenderTargetBlendDesc.BlendEnable = FALSE;
	DefaultRenderTargetBlendDesc.LogicOpEnable = FALSE;
	DefaultRenderTargetBlendDesc.SrcBlend = D3D12_BLEND_ONE;
	DefaultRenderTargetBlendDesc.DestBlend = D3D12_BLEND_ZERO;
	DefaultRenderTargetBlendDesc.BlendOp = D3D12_BLEND_OP_ADD;
	DefaultRenderTargetBlendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;
	DefaultRenderTargetBlendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;
	DefaultRenderTargetBlendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;
	DefaultRenderTargetBlendDesc.LogicOp = D3D12_LOGIC_OP_NOOP;
	DefaultRenderTargetBlendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	for (UINT i = 0; i < D3D12_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
	{
		BlendDesc.RenderTarget[i] = DefaultRenderTargetBlendDesc;
	}
}

void DirectX::SetRasterizerState(D3D12_RASTERIZER_DESC& RasterizerDesc)
{
	RasterizerDesc = {};

	RasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	RasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
	RasterizerDesc.FrontCounterClockwise = FALSE;
	RasterizerDesc.DepthBias = D3D12_DEFAULT_DEPTH_BIAS;
	RasterizerDesc.DepthBiasClamp = D3D12_DEFAULT_DEPTH_BIAS_CLAMP;
	RasterizerDesc.SlopeScaledDepthBias = D3D12_DEFAULT_SLOPE_SCALED_DEPTH_BIAS;
	RasterizerDesc.DepthClipEnable = TRUE;
	RasterizerDesc.MultisampleEnable = FALSE;
	RasterizerDesc.AntialiasedLineEnable = FALSE;
	RasterizerDesc.ForcedSampleCount = 0;
	RasterizerDesc.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_OFF;
}

void DirectX::SetDepthStencilState(D3D12_DEPTH_STENCIL_DESC& DepthStencilDesc)
{
	DepthStencilDesc = {};

	DepthStencilDesc.DepthEnable = FALSE;
	DepthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	DepthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_GREATER;

	DepthStencilDesc.StencilEnable = FALSE;
	DepthStencilDesc.StencilReadMask = D3D12_DEFAULT_STENCIL_READ_MASK;
	DepthStencilDesc.StencilWriteMask = D3D12_DEFAULT_STENCIL_WRITE_MASK;

	DepthStencilDesc.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	DepthStencilDesc.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;

	DepthStencilDesc.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilPassOp = D3D12_STENCIL_OP_KEEP;
	DepthStencilDesc.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
}

#endif // PLATFORM_WINDOWS