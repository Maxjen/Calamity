#pragma once

#include <memory>

#include <glm/glm.hpp>
//#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include "Mesh.h"
#include "Texture.h"
#include "../Calamity/Shader/CharacterMeshShader.h"

using std::unique_ptr;

class CharacterMeshComponent {
private:
	struct MeshVertex {
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoord;
		unsigned int boneIds[4];
		float boneWeights[4];
	};

	struct MeshEntry {
		explicit MeshEntry();
		~MeshEntry();

		GLuint vertexBufferId;
		GLuint indexBufferId;
		GLuint vaoId;

		unsigned int numIndices;

		vector<glm::vec3> vertexPositions;
		map<string, vector<VertexWeight>> weightsByBoneName;
	};

	vector<MeshEntry> entries;
	vector<Texture> textures;
	map<string, Bone3D*>* bonesByName;
	CharacterMeshShader* characterMeshShader;
public:
	explicit CharacterMeshComponent();
	bool init(const string& fileName, map<string, Bone3D*>* bonesByName, CharacterMeshShader* characterMeshShader);
	void render();

	void getBoneAabb(const string& boneName, float weightThreshold, glm::vec3& aabbMin, glm::vec3& aabbMax);
};
