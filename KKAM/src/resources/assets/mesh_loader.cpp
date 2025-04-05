#include "resources/assets/mesh_loader.h"
#include "utils/console.h"

namespace resources {
	namespace assets {
		MeshData MeshLoader::load(const CString& path) {
			MeshData mesh;
			std::ifstream file(path.c_str());
			if (!file.is_open()) {
				utils::Console::error(CString("Failed to open file: ") + CString(path).c_str());
				return mesh;
			}

			json data;
			try {
				file >> data;
			}
			catch (const nlohmann::json::parse_error& e) {
				utils::Console::error(CString("Failed to parse JSON: ") + CString(e.what()).c_str());
				return mesh;
			}

			try {
				if (data.contains("vertices")) {
					for (const auto& vertex : data["vertices"]) {
						mesh.vertices.push_back(Vector3(vertex[0], vertex[1], vertex[2]));
					}
				}
				else {
					utils::Console::error("JSON does not contain 'vertices' key.");
				}

				if (data.contains("normals")) {
					for (const auto& normal : data["normals"]) {
						mesh.normals.push_back(Vector3(normal[0], normal[1], normal[2]));
					}
				}
				else {
					utils::Console::error("JSON does not contain 'normals' key.");
				}

				if (data.contains("uv")) {
					for (const auto& uv : data["uv"]) {
						mesh.texCoords.push_back(Vector2(uv[0], uv[1]));
					}
				}
				else {
					utils::Console::error("JSON does not contain 'uv' key.");
				}

				if (data.contains("indices")) {
					for (const auto& index : data["indices"]) {
						mesh.indices.push_back(index);
					}
				}
				else {
					utils::Console::error("JSON does not contain 'indices' key.");
				}

				if (data.contains("textures")) {
					int i = 0;
					for (const auto& texture : data["textures"]) {
						std::string file_path = texture.get<std::string>();
						utils::Console::log(CString("Loading texture: ") + CString(combinePath(getDirectory(path), file_path)).c_str());
						auto* dx11Texture = new DX11Texture();
						dx11Texture->setFilePath(CString(combinePath(getDirectory(path), file_path)));
						dx11Texture->setSlot(i);
						mesh.textures[i] = dx11Texture;
						i++;
					}
				}
				else {
					utils::Console::error("JSON does not contain 'textures' key.");
				}
			}
			catch (const std::exception& e) {
				utils::Console::error(CString("Error processing JSON data: ") + CString(e.what()).c_str());
			}

			return mesh;
		}
	}
}
