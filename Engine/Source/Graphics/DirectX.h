#pragma once

#include "Source/Interfaces/RendererInterface.h"

#include <Windows.h>

struct D3D12_BLEND_DESC;
struct D3D12_RASTERIZER_DESC;
struct D3D12_DEPTH_STENCIL_DESC;
struct ID3D12GraphicsCommandList;
struct ID3D12CommandAllocator;
struct IDXGISwapChain3;
struct ID3D12CommandQueue;
struct ID3D12Resource;
struct ID3D12DescriptorHeap;
struct ID3D12RootSignature;
struct ID3D12PipelineState;
struct ID3D12Fence;

class DirectX : public IRenderer
{
public:

	virtual void Init() override;
	virtual void Update() override;
	virtual DefaultWindow* GetWindowInterface() override;
	virtual void SetWindowInterface(DefaultWindow* WindowInterface) override;

	void SetBlendState(D3D12_BLEND_DESC& BlendDesc);
	void SetRasterizerState(D3D12_RASTERIZER_DESC& RasterizerDesc);
	void SetDepthStencilState(D3D12_DEPTH_STENCIL_DESC& DepthStencilDesc);

protected:

	DefaultWindow* Window = nullptr;
	ID3D12CommandQueue* CommandQueue = nullptr;
	ID3D12CommandAllocator* CommandAllocator = nullptr;
	ID3D12GraphicsCommandList* CommandList = nullptr;
	IDXGISwapChain3* SwapChain = nullptr;
	ID3D12Resource* RenderTargets[2];
	ID3D12DescriptorHeap* RTVHeap = nullptr;
	UINT RTVIncrementSize = 0;
	ID3D12RootSignature* RootSignature = nullptr;
	ID3D12PipelineState* PipelineState = nullptr;
	ID3D12Fence* Fence = nullptr;
	UINT64 FenceValue = 0;
	HANDLE FenceEvent = nullptr;

private:

	unsigned int TriangleAngle = 0;
};