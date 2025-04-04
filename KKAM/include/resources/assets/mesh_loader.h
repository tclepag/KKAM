#pragma once

#include <map>
#include "common/common.h"
#include "common/directx11.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <nlohmann/json.hpp>

#include <fstream>
#include <sstream>

#include "resources/dx11/dx11texture.h"

namespace resources {
	namespace assets {
		using json = nlohmann::json;
		struct MeshData {
			Vector<Vector3> vertices;
			Vector<Vector3> normals;
			Vector<Vector2> texCoords;
			Vector<uint32_t> indices;

			std::map<size_t, DX11Texture*> textures;
		};

		class MeshLoader {
		public:
			static MeshData load(const CString& path); // Loads a .mesh file and returns the mesh data associated with it
			static bool save(const CString& path, const MeshData& meshData); // Saves the mesh data to a .mesh file to a directory
		private:
			// This class is not meant to be a creatable class
			MeshLoader() = default;
			~MeshLoader() = default;

			static std::string getDirectory(const std::string& filePath) {
				std::filesystem::path path(filePath);
				return path.parent_path().string();
			}

			static std::string combinePath(const std::string& directory, const std::string& relativePath) {
				std::filesystem::path dir(directory);
				std::filesystem::path relPath(relativePath);
				return (dir / relPath).lexically_normal().string();
			}
		};
	}
}