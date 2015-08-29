#pragma once

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <GL/glew.h>

#include <unordered_map>
#include <string>
#include <vector>

#include "Math/Vec3.h"
#include "Math/Vec2.h"
#include "Camera.h"
#include "GBuffer.h"
#include "Shader/NullShader.h"
#include "Shader/DeferredShader.h"

using std::unordered_map;
using std::string;
using std::vector;

using namespace clm;

class Lights {
private:
	void initMesh(unsigned int index, const aiMesh* mesh);
	void stencilPass();
	void pointLightPass();
	void finalPass();

	struct PointLightInternal {
		PointLight p;
		float radius;
	};

	struct MeshEntry {
		explicit MeshEntry();
		~MeshEntry();

		void init(const std::vector<Vec3>& vertices, const std::vector<unsigned int>& indices, DeferredShader* deferredShader);

		GLuint vertexBufferId;
		GLuint indexBufferId;
		GLuint vaoId;

		unsigned int numIndices;
	};

	Camera* camera;
	GBuffer* gBuffer;
	NullShader* nullShader;
	DeferredShader* deferredShader;

	vector<MeshEntry> entries;

	unordered_map<unsigned int, PointLightInternal> pointLights;
	unsigned int cPointLight;
public:
	explicit Lights(Camera* camera, GBuffer* gBuffer, NullShader* nullShader, DeferredShader* deferredShader);
	~Lights();

	bool loadMesh();

	unsigned int addPointLight(PointLight pointLight);
	void setPointLights(unsigned int pointLightId, PointLight pointLight);

	void render();
};
