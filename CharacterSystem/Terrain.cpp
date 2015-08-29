#include "Terrain.h"

Terrain::Terrain(Camera* camera, MeshShader* meshShader, btSoftRigidDynamicsWorld* dynamicsWorld) :
	camera(camera),
	meshShader(meshShader),
	vertexBufferId(0),
	indexBufferId(0),
	numIndices(0),
	vaoId(0),
	texture(),
	dynamicsWorld(dynamicsWorld),
	data(),
	terrainShape(),
	rigidBody(),
	bLoaded(false)
{}

Terrain::~Terrain() {
	if (vaoId != (unsigned int)-1)
		glDeleteVertexArrays(1, &vaoId);

	if (vertexBufferId != (unsigned int)-1)
		glDeleteBuffers(1, &vertexBufferId);

	if (indexBufferId != (unsigned int)-1)
		glDeleteBuffers(1, &indexBufferId);

	if (bLoaded) {
		dynamicsWorld->removeRigidBody(rigidBody.get());
		delete rigidBody->getMotionState();
	}
}

void Terrain::loadTerrain() {
	if (!bLoaded) {
		bLoaded = true;

		int width = 65;
		int length = 65;
		data.resize(width * length);
		for (int y = 0; y < length; ++y) {
			for (int x = 0; x < width; ++x)
				data[y * length + x] = sin(((float)y) / 3);
		}


		vector<Vertex> vertices;
		vector<unsigned int> indices;

		for (int y = 0; y < length; ++y) {
			for (int x = 0; x < width; ++x) {
				glm::vec3 tx, ty;

				if (x == 0) {
					tx = glm::vec3(1, data[y * length + x + 1] - data[y * length + x], 0);
				}
				else if (x == width - 1) {
					tx = glm::vec3(1, data[y * length + x] - data[y * length + x - 1], 0);
				}
				else {
					glm::vec3 txL, txR;
					txL = glm::vec3(1, data[y * length + x] - data[y * length + x - 1], 0);
					txR = glm::vec3(1, data[y * length + x + 1] - data[y * length + x], 0);
					txL = glm::normalize(txL);
					txR = glm::normalize(txR);
					tx = txL + txR;
				}
				tx = glm::normalize(tx);

				if (y == 0) {
					ty = glm::vec3(0, data[(y + 1) * length + x] - data[y * length + x], 1);
				}
				else if (y == length - 1) {
					ty = glm::vec3(0, data[y * length + x] - data[(y - 1) * length + x], 1);
				}
				else {
					glm::vec3 tyB, tyT;
					tyB = glm::vec3(0, data[y * length + x] - data[(y - 1) * length + x], 1);
					tyT = glm::vec3(0, data[(y + 1) * length + x] - data[y * length + x], 1);
					tyB = glm::normalize(tyB);
					tyT = glm::normalize(tyT);
					ty = tyB + tyT;
				}
				ty = glm::normalize(ty);
				glm::vec3 n = glm::cross(ty, tx);
				n = glm::normalize(n);

				Vertex v;
				v.pos.x = -(width / 2) + x;
				v.pos.y = data[y * length + x];
				v.pos.z = -(length / 2) + y;
				v.normal = n;
				v.texCoord = glm::vec2(((float)x) / width, ((float)y) / length);

				vertices.push_back(v);
			}
		}

		for (int y = 0; y < length - 1; ++y) {
			for (int x = 0; x < width - 1; ++x) {
				indices.push_back(y * length + x);
				indices.push_back((y + 1) * length + x);
				indices.push_back((y + 1) * length + x + 1);

				indices.push_back(y * length + x);
				indices.push_back((y + 1) * length + x + 1);
				indices.push_back(y * length + x + 1);
			}
		}

		numIndices = indices.size();

		glGenBuffers(1, &vertexBufferId);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

		glGenBuffers(1, &indexBufferId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, &indices[0], GL_STATIC_DRAW);

		vaoId = meshShader->createVertexArrayObject(vertexBufferId, sizeof(Vertex), 0, (GLvoid*)(3 * sizeof(float)), (GLvoid*)(6 * sizeof(float)));

		texture.load("Data/test.png");



		terrainShape.reset(new btHeightfieldTerrainShape(width, length, &data[0], 1.0f, -10.0f, 10.0f, 1, PHY_FLOAT, false));
		//terrainShape->setLocalScaling(btVector3(5.0f, 1.0f, 5.0f));
		btVector3 localInertia(0, 0, 0);
		terrainShape->calculateLocalInertia(0, localInertia);
		btTransform transform;
		transform.setIdentity();
		transform.setOrigin(btVector3(0, 0, 0));
		btDefaultMotionState* motionState = new btDefaultMotionState(transform);
		btRigidBody::btRigidBodyConstructionInfo constructionInfo(0, motionState, terrainShape.get(), localInertia);
		rigidBody.reset(new btRigidBody(constructionInfo));
		dynamicsWorld->addRigidBody(rigidBody.get(), COL_WORLD, COL_ALL);
	}
}

void Terrain::render() {
	if (bLoaded) {
		/*glm::mat4 modelMatrix = glm::translate(glm::mat4(), position);
		modelMatrix = glm::scale(modelMatrix, scale);*/

		glm::mat4 modelMatrix = glm::mat4();

		glm::mat4 modelViewMatrix = camera->getViewMatrix() * modelMatrix;

		glm::mat4 modelInverseTranspose = glm::inverse(modelMatrix);
		modelInverseTranspose = glm::transpose(modelInverseTranspose);

		meshShader->bind();
		meshShader->setModelViewMatrix(modelViewMatrix);
		meshShader->setModelMatrix(modelMatrix);
		meshShader->setModelInverseTranspose(modelInverseTranspose);

		glBindVertexArray(vaoId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);

		texture.bind(GL_TEXTURE0);
		glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
		texture.unbind();

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		meshShader->unbind();
	}
}
