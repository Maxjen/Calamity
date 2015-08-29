#pragma once

#include <memory>
#include <vector>
#include <cmath>

#include "btBulletDynamicsCommon.h"
#include "BulletSoftBody/btSoftRigidDynamicsWorld.h"
#include "BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h"

#include "Camera.h"
#include "../Calamity/Shader/MeshShader.h"
#include "Texture.h"
#include "GameObject3D.h"

using std::unique_ptr;
using std::vector;

using namespace clm;

class Terrain {
private:
	struct Vertex {
		/*explicit Vertex(glm::vec3 pos, glm::vec3 normal, glm::vec2 texCoord) :
			pos(pos),
			normal(normal),
			texCoord(texCoord)
		{}*/

		glm::vec3 pos;
		glm::vec3 normal;
		glm::vec2 texCoord;
	};

	Camera* camera;
	MeshShader* meshShader;

	GLuint vertexBufferId;
	GLuint indexBufferId;
	unsigned int numIndices;
	GLuint vaoId;
	Texture texture;

	btSoftRigidDynamicsWorld* dynamicsWorld;

	vector<float> data;
	unique_ptr<btHeightfieldTerrainShape> terrainShape;
	unique_ptr<btRigidBody> rigidBody;

	bool bLoaded;
public:
	explicit Terrain(Camera* camera, MeshShader* meshShader, btSoftRigidDynamicsWorld* dynamicsWorld);
	~Terrain();

	void loadTerrain();
	void render();
};
