#pragma once

#include "resources/texture.h"
#include "common/directx11.h"

namespace resources {
	class DX11Texture : public Texture {
	public:
		DX11Texture();
		~DX11Texture();
		void loadFromFile() override;
		void bind() override;
		void unbind() override;
		void setContext(Context11* context) {
			this->m_context = context;
		}
		void releaseResources();
	private:
		Context11* m_context;
		ID3D11Texture2D* m_texture;
		ID3D11ShaderResourceView* m_textureView;
		ID3D11SamplerState* m_samplerState;
	};
}