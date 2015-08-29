#include "Lights.h"

Lights::MeshEntry::MeshEntry() :
	vertexBufferId(-1),
	indexBufferId(-1),
	vaoId(-1),
	numIndices(0)
{}

Lights::MeshEntry::~MeshEntry() {
	if (vaoId != (unsigned int)-1)
		glDeleteVertexArrays(1, &vaoId);

	if (vertexBufferId != (unsigned int)-1)
		glDeleteBuffers(1, &vertexBufferId);

	if (indexBufferId != (unsigned int)-1)
		glDeleteBuffers(1, &indexBufferId);
}

void Lights::MeshEntry::init(const std::vector<Vec3>& vertices, const std::vector<unsigned int>& indices, DeferredShader* deferredShader) {
	numIndices = indices.size();

	glGenBuffers(1, &vertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vec3) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &indexBufferId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * numIndices, &indices[0], GL_STATIC_DRAW);

	vaoId = deferredShader->createVertexArrayObject(vertexBufferId, sizeof(Vec3), 0);
}

Lights::Lights(Camera* camera, GBuffer* gBuffer, NullShader* nullShader, DeferredShader* deferredShader) :
	camera(camera),
	gBuffer(gBuffer),
	nullShader(nullShader),
	deferredShader(deferredShader),
	entries(),
	pointLights(),
	cPointLight(0)
{}

Lights::~Lights() {
}

bool Lights::loadMesh() {
	bool result = false;
	Assimp::Importer importer;

	//const aiScene* scene = importer.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs);
	const aiScene* scene = importer.ReadFile("Data/sphere.fbx", aiProcess_Triangulate | aiProcess_FlipUVs);

	if (scene) {
		entries.resize(scene->mNumMeshes);

		// Initialize the meshes in the scene one by one
		for (unsigned int i = 0; i < entries.size(); ++i) {
			const aiMesh* mesh = scene->mMeshes[i];
			initMesh(i, mesh);
			result = true;
		}
	}
	else {
		printf("Error parsing '%s': '%s'\n", "Data/sphere.fbx", importer.GetErrorString());
	}

	return result;
}

unsigned int Lights::addPointLight(PointLight pointLight) {
	pointLights[cPointLight].p = pointLight;

	float maxChannel = std::max(std::max(pointLight.color.r, pointLight.color.g), pointLight.color.b);
	pointLights[cPointLight].radius = (-pointLight.attLinear + std::sqrt(pointLight.attLinear * pointLight.attLinear -
																		 4 * pointLight.attExponential * (pointLight.attExponential - 256 * maxChannel * pointLight.diffuseIntensity))) /
									   2 * pointLight.attExponential;
	return cPointLight++;
}

void Lights::setPointLights(unsigned int pointLightId, PointLight pointLight) {
	if (pointLights.find(pointLightId) != pointLights.end()) {
		pointLights[pointLightId].p = pointLight;

		float maxChannel = std::max(std::max(pointLight.color.r, pointLight.color.g), pointLight.color.b);
		pointLights[pointLightId].radius = (-pointLight.attLinear + std::sqrt(pointLight.attLinear * pointLight.attLinear -
																			 4 * pointLight.attExponential * (pointLight.attExponential - 256 * maxChannel * pointLight.diffuseIntensity))) /
										   2 * pointLight.attExponential;
	}
}

void Lights::render() {
	glm::mat4 modelViewMatrix;
	glm::mat4 viewMatrix = camera->getViewMatrix();

	deferredShader->bind();
	deferredShader->setEyeWorldPos(camera->getPosition());
	deferredShader->setMatSpecularIntensity(1.0f);
	deferredShader->setMatSpecularPower(32);
	deferredShader->unbind();

	glEnable(GL_STENCIL_TEST);

	for (auto& it : pointLights) {
		modelViewMatrix = glm::translate(glm::mat4(), it.second.p.position);
		modelViewMatrix = glm::scale(modelViewMatrix, glm::vec3(it.second.radius, it.second.radius, it.second.radius));
		modelViewMatrix = viewMatrix * modelViewMatrix;

		nullShader->bind();
		nullShader->setModelViewMatrix(modelViewMatrix);
		stencilPass();
		nullShader->unbind();

		deferredShader->bind();
		deferredShader->setModelViewMatrix(modelViewMatrix);
		deferredShader->setPointLight(it.second.p);
		pointLightPass();
		deferredShader->unbind();
	}

	glDisable(GL_STENCIL_TEST);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Lights::initMesh(unsigned int index, const aiMesh* mesh) {
	vector<Vec3> vertices;
	vector<unsigned int> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; ++i) {
		const aiVector3D* pos = &(mesh->mVertices[i]);
		Vec3 v(pos->x, pos->y, pos->z);
		vertices.push_back(v);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; ++i) {
		const aiFace& face = mesh->mFaces[i];
		assert(face.mNumIndices == 3);
		indices.push_back(face.mIndices[0]);
		indices.push_back(face.mIndices[1]);
		indices.push_back(face.mIndices[2]);
	}

	entries[index].init(vertices, indices, deferredShader);
}

void Lights::stencilPass() {
	gBuffer->bindForStencilPass();

	glEnable(GL_DEPTH_TEST);
	glClear(GL_STENCIL_BUFFER_BIT);

	glStencilFunc(GL_ALWAYS, 0, 0);
	glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
	glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);

	for (unsigned int i = 0; i < entries.size() ; ++i) {
		glBindVertexArray(entries[i].vaoId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, entries[i].indexBufferId);
		glDrawElements(GL_TRIANGLES, entries[i].numIndices, GL_UNSIGNED_INT, 0);
	}

	glDisable(GL_DEPTH_TEST);
}

void Lights::pointLightPass() {
	gBuffer->bindForLightPass();

	glStencilFunc(GL_NOTEQUAL, 0, 0xFF);
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_ONE, GL_ONE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	for (unsigned int i = 0; i < entries.size() ; ++i) {
		glBindVertexArray(entries[i].vaoId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, entries[i].indexBufferId);
		glDrawElements(GL_TRIANGLES, entries[i].numIndices, GL_UNSIGNED_INT, 0);
	}

	glCullFace(GL_BACK);
	glDisable(GL_CULL_FACE);

	glDisable(GL_BLEND);
}
