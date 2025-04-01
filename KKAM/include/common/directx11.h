#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl/client.h>

// Simpler DX11 names

using Device11 = ID3D11Device;
using Context11 = ID3D11DeviceContext;
using SwapChain11 = IDXGISwapChain;
using RenderTargetView11 = ID3D11RenderTargetView;
using Buffer11 = ID3D11Buffer;
using InputLayout11 = ID3D11InputLayout;
using VertexShader11 = ID3D11VertexShader;

using Microsoft::WRL::ComPtr;
