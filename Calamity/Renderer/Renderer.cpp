#include "Renderer.h"

namespace clm {

SpriteLayer::~SpriteLayer() {
	glDeleteVertexArrays(1, &vaoId);
	glDeleteBuffers(1, &vertexBufferId);
	glDeleteBuffers(1, &indexBufferId);
}

void SpriteLayer::uploadBuffers() {
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, vertexBufferData.size() * sizeof(SpriteVertexSimple), &(vertexBufferData[0]), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferData.size() * sizeof(int), &(indexBufferData[0]), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

TextureTriangleLayer::~TextureTriangleLayer() {
	glDeleteVertexArrays(1, &vaoId);
	glDeleteBuffers(1, &vertexBufferId);
	glDeleteBuffers(1, &indexBufferId);
}

void TextureTriangleLayer::uploadBuffers() {
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, vertexBufferData.size() * sizeof(TextureVertexSimple), &(vertexBufferData[0]), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferData.size() * sizeof(int), &(indexBufferData[0]), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

FontLayer::~FontLayer() {
	glDeleteVertexArrays(1, &vaoId);
	glDeleteBuffers(1, &vertexBufferId);
	glDeleteBuffers(1, &indexBufferId);
}

void FontLayer::uploadBuffers() {
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, vertexBufferData.size() * sizeof(SpriteVertexSimple), &(vertexBufferData[0]), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferData.size() * sizeof(int), &(indexBufferData[0]), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

LineLayer::~LineLayer() {
	glDeleteVertexArrays(1, &vaoId);
	glDeleteBuffers(1, &vertexBufferId);
	glDeleteBuffers(1, &indexBufferId);
}

void LineLayer::uploadBuffers() {
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, vertexBufferData.size() * sizeof(ColorVertex), &(vertexBufferData[0]), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferData.size() * sizeof(int), &(indexBufferData[0]), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

TriangleLayer::~TriangleLayer() {
	glDeleteVertexArrays(1, &vaoId);
	glDeleteBuffers(1, &vertexBufferId);
	glDeleteBuffers(1, &indexBufferId);
}

void TriangleLayer::uploadBuffers() {
	glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, vertexBufferData.size() * sizeof(ColorVertex), &(vertexBufferData[0]), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferData.size() * sizeof(int), &(indexBufferData[0]), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

Renderer::Renderer(TextureManager* textureManager, FontManager* fontManager, SpriteShader* spriteShader, TextureShader* textureShader, FontShader* fontShader, ColorShader* colorShader) :
	textureManager(textureManager),
	spriteShader(spriteShader),
	textureShader(textureShader),
	fontManager(fontManager),
	fontShader(fontShader),
	colorShader(colorShader),
	spriteVertices(),
	cSpriteVertex(0),
	textureVertices(),
	cTextureVertex(0),
	colorVertices(),
	cColorVertex(0)
{}

void Renderer::setSpriteShaderViewPos(float viewPosX, float viewPosY) {
	glm::mat4 modelViewMatrix = glm::translate(glm::mat4(), glm::vec3(-viewPosX, -viewPosY, 0.0f));
	spriteShader->bind();
	spriteShader->setModelViewMatrix(modelViewMatrix);
	spriteShader->unbind();
}

void Renderer::setTextureShaderViewPos(float viewPosX, float viewPosY) {
	glm::mat4 modelViewMatrix = glm::translate(glm::mat4(), glm::vec3(-viewPosX, -viewPosY, 0.0f));
	textureShader->bind();
	textureShader->setModelViewMatrix(modelViewMatrix);
	textureShader->unbind();
}

void Renderer::setColorShaderViewPos(float viewPosX, float viewPosY) {
	glm::mat4 modelViewMatrix = glm::translate(glm::mat4(), glm::vec3(-viewPosX, -viewPosY, 0.0f));
	colorShader->bind();
	colorShader->setModelViewMatrix(modelViewMatrix);
	colorShader->unbind();
}

TextureManager* Renderer::getTextureManager() {
	return textureManager;
}

FontManager* Renderer::getFontManager() {
	return fontManager;
}

void Renderer::enableBlending() {
	glEnable(GL_BLEND);
}

void Renderer::setBlendFunc(GLenum sFactor, GLenum dFactor) {
	glBlendFunc(sFactor, dFactor);
}

void Renderer::disableBlending() {
	glDisable(GL_BLEND);
}

unsigned int Renderer::addSpriteVertex(float x, float y, float u, float v, char r, char g, char b, char a, int textureId) {
	SpriteVertex& vertex = spriteVertices[cSpriteVertex];
	vertex.x = x;
	vertex.y = y;
	vertex.u = u;
	vertex.v = v;
	vertex.r = r;
	vertex.g = g;
	vertex.b = b;
	vertex.a = a;
	vertex.textureId = textureId;
	return cSpriteVertex++;
}

void Renderer::removeSpriteVertex(unsigned int v) {
	spriteVertices.erase(v);
}

void Renderer::setSpriteVertexPosition(unsigned int v, float x, float y) {
	spriteVertices[v].x = x;
	spriteVertices[v].y = y;
}

Vec2 Renderer::getSpriteVertexPosition(unsigned int v) {
	return Vec2(spriteVertices[v].x, spriteVertices[v].y);
}

void Renderer::initSpriteLayer(SpriteLayer* spriteLayer) {
	glGenBuffers(1, &(spriteLayer->vertexBufferId));
	glGenBuffers(1, &(spriteLayer->indexBufferId));
	spriteLayer->vaoId = spriteShader->createVertexArrayObject(spriteLayer->vertexBufferId, sizeof(SpriteVertexSimple), 0, (GLvoid*)(2 * sizeof(float)), (GLvoid*)(4 * sizeof(float)));
}

void Renderer::addSpriteTriangleToLayer(SpriteLayer* spriteLayer, unsigned int v1, unsigned int v2, unsigned int v3) {
	//SpriteLayer& layer = spriteLayers[layerId];

	unordered_map<unsigned int, int>::iterator it;

	SpriteVertexSimple v;

	it = spriteLayer->indices.find(v1);
	if (it == spriteLayer->indices.end()) {
		extractSpriteVertexSimple(v, v1);
		spriteLayer->vertexBufferData.push_back(v);
		spriteLayer->indices[v1] = spriteLayer->vertexBufferData.size() - 1;
	}

	it = spriteLayer->indices.find(v2);
	if (it == spriteLayer->indices.end()) {
		extractSpriteVertexSimple(v, v2);
		spriteLayer->vertexBufferData.push_back(v);
		spriteLayer->indices[v2] = spriteLayer->vertexBufferData.size() - 1;
	}

	it = spriteLayer->indices.find(v3);
	if (it == spriteLayer->indices.end()) {
		extractSpriteVertexSimple(v, v3);
		spriteLayer->vertexBufferData.push_back(v);
		spriteLayer->indices[v3] = spriteLayer->vertexBufferData.size() - 1;
	}

	spriteLayer->indexBufferData.push_back(spriteLayer->indices[v1]);
	spriteLayer->indexBufferData.push_back(spriteLayer->indices[v2]);
	spriteLayer->indexBufferData.push_back(spriteLayer->indices[v3]);

	if (!spriteLayer->verticesWithAtlas.empty()) {
		//VerticesWithAtlas& last = spriteLayer->verticesWithAtlas[layer.verticesWithAtlas.size() - 1];
		VerticesWithAtlas& last = spriteLayer->verticesWithAtlas.back();
		if (last.atlasId == textureManager->getAtlasId(spriteVertices[v1].textureId)) {
			last.numVertices += 3;
			return;
		}
	}
	VerticesWithAtlas newV;
	newV.atlasId = textureManager->getAtlasId(spriteVertices[v1].textureId);
	newV.numVertices = 3;
	spriteLayer->verticesWithAtlas.push_back(newV);
}

void Renderer::drawSpriteLayer(SpriteLayer* spriteLayer) {
	spriteShader->bind();
	glBindVertexArray(spriteLayer->vaoId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, spriteLayer->indexBufferId);
	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
	/*textureManager->bind(GL_TEXTURE0);
	glDrawElements(GL_TRIANGLES, layer.indexBufferData.size(), GL_UNSIGNED_INT, 0);*/
	int offset = 0;
	for (auto& it : spriteLayer->verticesWithAtlas) {
		textureManager->bind(GL_TEXTURE0, it.atlasId);
		glDrawElements(GL_TRIANGLES, it.numVertices, GL_UNSIGNED_INT, (GLvoid*)(offset * sizeof(int)));
		offset += it.numVertices;
	}

	textureManager->unbind();
	//glDisable(GL_BLEND);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	spriteShader->unbind();
}

void Renderer::extractSpriteVertexSimple(SpriteVertexSimple& result, unsigned int v) {
	const SpriteVertex& s = spriteVertices[v];

	TextureBounds bounds = textureManager->getUvBounds(s.textureId);
	result.x = s.x;
	result.y = s.y;
	result.u = bounds.minU + s.u * (bounds.maxU - bounds.minU);
	result.v = bounds.minV + s.v * (bounds.maxV - bounds.minV);
	result.r = s.r;
	result.g = s.g;
	result.b = s.b;
	result.a = s.a;
}

unsigned int Renderer::addTextureVertex(float x, float y, float u, float v, char r, char g, char b, char a, int textureId) {
	TextureVertex& vertex = textureVertices[cTextureVertex];
	vertex.x = x;
	vertex.y = y;
	vertex.u = u;
	vertex.v = v;
	vertex.r = r;
	vertex.g = g;
	vertex.b = b;
	vertex.a = a;
	vertex.textureId = textureId;

	TextureBounds bounds = textureManager->getUvBounds(textureId);
	vertex.texOriginU = bounds.minU;
	vertex.texOriginV = bounds.maxV;
	vertex.texWidth = bounds.maxU - bounds.minU;
	vertex.texHeight = bounds.maxV - bounds.minV;

	return cTextureVertex++;
}

void Renderer::removeTextureVertex(unsigned int v) {
	textureVertices.erase(v);
}

void Renderer::setTextureVertexPosition(unsigned int v, float x, float y) {
	textureVertices[v].x = x;
	textureVertices[v].y = y;
}

void Renderer::setTextureVertexUV(unsigned int v, float texU, float texV) {
	textureVertices[v].u = texU;
	textureVertices[v].v = texV;
}

void Renderer::setTextureVertexColor(unsigned int v, char r, char g, char b, char a) {
	textureVertices[v].r = r;
	textureVertices[v].g = g;
	textureVertices[v].b = b;
	textureVertices[v].a = a;
}

void Renderer::initTextureTriangleLayer(TextureTriangleLayer* textureTriangleLayer) {
	glGenBuffers(1, &(textureTriangleLayer->vertexBufferId));
	glGenBuffers(1, &(textureTriangleLayer->indexBufferId));
	textureTriangleLayer->vaoId =
	textureShader->createVertexArrayObject(textureTriangleLayer->vertexBufferId, sizeof(TextureVertexSimple), 0, (GLvoid*)(2 * sizeof(float)), (GLvoid*)(4 * sizeof(float)), (GLvoid*)(4 * sizeof(float) + 4 * sizeof(char)), (GLvoid*)(6 * sizeof(float) + 4 * sizeof(char)));
}

void Renderer::addTextureTriangleToLayer(TextureTriangleLayer* textureTriangleLayer, unsigned int v1, unsigned int v2, unsigned int v3) {
	//SpriteLayer& layer = spriteLayers[layerId];

	unordered_map<unsigned int, int>::iterator it;

	TextureVertexSimple v;

	it = textureTriangleLayer->indices.find(v1);
	if (it == textureTriangleLayer->indices.end()) {
		extractTextureVertexSimple(v, v1);
		textureTriangleLayer->vertexBufferData.push_back(v);
		textureTriangleLayer->indices[v1] = textureTriangleLayer->vertexBufferData.size() - 1;
	}

	it = textureTriangleLayer->indices.find(v2);
	if (it == textureTriangleLayer->indices.end()) {
		extractTextureVertexSimple(v, v2);
		textureTriangleLayer->vertexBufferData.push_back(v);
		textureTriangleLayer->indices[v2] = textureTriangleLayer->vertexBufferData.size() - 1;
	}

	it = textureTriangleLayer->indices.find(v3);
	if (it == textureTriangleLayer->indices.end()) {
		extractTextureVertexSimple(v, v3);
		textureTriangleLayer->vertexBufferData.push_back(v);
		textureTriangleLayer->indices[v3] = textureTriangleLayer->vertexBufferData.size() - 1;
	}

	textureTriangleLayer->indexBufferData.push_back(textureTriangleLayer->indices[v1]);
	textureTriangleLayer->indexBufferData.push_back(textureTriangleLayer->indices[v2]);
	textureTriangleLayer->indexBufferData.push_back(textureTriangleLayer->indices[v3]);

	if (!textureTriangleLayer->verticesWithAtlas.empty()) {
		//VerticesWithAtlas& last = spriteLayer->verticesWithAtlas[layer.verticesWithAtlas.size() - 1];
		VerticesWithAtlas& last = textureTriangleLayer->verticesWithAtlas.back();
		if (last.atlasId == textureManager->getAtlasId(textureVertices[v1].textureId)) {
			last.numVertices += 3;
			return;
		}
	}
	VerticesWithAtlas newV;
	newV.atlasId = textureManager->getAtlasId(spriteVertices[v1].textureId);
	newV.numVertices = 3;
	textureTriangleLayer->verticesWithAtlas.push_back(newV);
}

void Renderer::drawTextureTriangleLayer(TextureTriangleLayer* textureTriangleLayer) {
	textureShader->bind();
	glBindVertexArray(textureTriangleLayer->vaoId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, textureTriangleLayer->indexBufferId);
	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
	/*textureManager->bind(GL_TEXTURE0);
	glDrawElements(GL_TRIANGLES, layer.indexBufferData.size(), GL_UNSIGNED_INT, 0);*/
	int offset = 0;
	for (auto& it : textureTriangleLayer->verticesWithAtlas) {
		textureManager->bind(GL_TEXTURE0, it.atlasId);
		glDrawElements(GL_TRIANGLES, it.numVertices, GL_UNSIGNED_INT, (GLvoid*)(offset * sizeof(int)));
		offset += it.numVertices;
	}

	textureManager->unbind();
	//glDisable(GL_BLEND);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	textureShader->unbind();
}

void Renderer::extractTextureVertexSimple(TextureVertexSimple& result, unsigned int v) {
	const TextureVertex& t = textureVertices[v];

	result.x = t.x;
	result.y = t.y;
	result.u = t.u;
	result.v = t.v;
	result.r = t.r;
	result.g = t.g;
	result.b = t.b;
	result.a = t.a;
	result.texOriginU = t.texOriginU;
	result.texOriginV = t.texOriginV;
	result.texWidth = t.texWidth;
	result.texHeight = t.texHeight;
}

void Renderer::writeTextureAtlasesToPng() {
	textureManager->writeToPng();
}

void Renderer::initFontLayer(FontLayer* fontLayer) {
	fontLayer->cIndex = 0;
	glGenBuffers(1, &(fontLayer->vertexBufferId));
	glGenBuffers(1, &(fontLayer->indexBufferId));
	fontLayer->vaoId = fontShader->createVertexArrayObject(fontLayer->vertexBufferId, sizeof(SpriteVertexSimple), 0, (GLvoid*)(2 * sizeof(float)), (GLvoid*)(4 * sizeof(float)));
}

void Renderer::addLetterToLayer(FontLayer* fontLayer, wchar_t letter, float x, float y, char r, char g, char b, char a, unsigned int fontId) {
	Glyph glyph = fontManager->getGlyph(fontId, letter);

	SpriteVertexSimple v0, v1, v2, v3;
	v0.x = x + glyph.offsetX;
	v0.y = y + glyph.offsetY;
	v0.u = glyph.minU;
	v0.v = glyph.minV;
	v1.x = v0.x;
	v1.y = v0.y - glyph.height;
	v1.u = glyph.minU;
	v1.v = glyph.maxV;
	v2.x = v0.x + glyph.width;
	v2.y = v1.y;
	v2.u = glyph.maxU;
	v2.v = glyph.maxV;
	v3.x = v2.x;
	v3.y = v0.y;
	v3.u = glyph.maxU;
	v3.v = glyph.minV;

	v0.r = v1.r = v2.r = v3.r = r;
	v0.g = v1.g = v2.g = v3.g = g;
	v0.b = v1.b = v2.b = v3.b = b;
	v0.a = v1.a = v2.a = v3.a = a;

	fontLayer->indexBufferData.push_back(fontLayer->cIndex);
	fontLayer->indexBufferData.push_back(fontLayer->cIndex + 1);
	fontLayer->indexBufferData.push_back(fontLayer->cIndex + 2);
	fontLayer->indexBufferData.push_back(fontLayer->cIndex);
	fontLayer->indexBufferData.push_back(fontLayer->cIndex + 2);
	fontLayer->indexBufferData.push_back(fontLayer->cIndex + 3);

	fontLayer->vertexBufferData.push_back(v0);
	fontLayer->vertexBufferData.push_back(v1);
	fontLayer->vertexBufferData.push_back(v2);
	fontLayer->vertexBufferData.push_back(v3);

	fontLayer->cIndex += 4;

	if (!fontLayer->lettersWithFont.empty()) {
		VerticesWithAtlas& last = fontLayer->lettersWithFont.back();
		if (last.atlasId == fontId) {
			last.numVertices += 1;
			return;
		}
	}
	VerticesWithAtlas newV;
	newV.atlasId = fontId;
	newV.numVertices = 1;
	fontLayer->lettersWithFont.push_back(newV);


	/*SpriteVertex& vertex = spriteVertices[cSpriteVertex];
	vertex.x = x;
	vertex.y = y;
	vertex.u = u;
	vertex.v = v;
	vertex.r = r;
	vertex.g = g;
	vertex.b = b;
	vertex.a = a;
	vertex.textureId = textureId;
	return cSpriteVertex++;*/
}

void Renderer::drawFontLayer(FontLayer* fontLayer) {
	fontShader->bind();
	glBindVertexArray(fontLayer->vaoId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, fontLayer->indexBufferId);
	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
	int offset = 0;
	for (auto& it : fontLayer->lettersWithFont) {
		//printf("%d\n", it.atlasId);
		fontManager->bindFontAtlas(GL_TEXTURE0, it.atlasId);
		glDrawElements(GL_TRIANGLES, it.numVertices * 6, GL_UNSIGNED_INT, (GLvoid*)(offset * sizeof(int)));
		offset += it.numVertices * 6;
	}

	fontManager->unbind();
	//glDisable(GL_BLEND);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	fontShader->unbind();
}

unsigned int Renderer::addColorVertex(float x, float y, char r, char g, char b, char a) {
	ColorVertex& vertex = colorVertices[cColorVertex];
	vertex.x = x;
	vertex.y = y;
	vertex.r = r;
	vertex.g = g;
	vertex.b = b;
	vertex.a = a;
	return cColorVertex++;
}

void Renderer::removeColorVertex(unsigned int v) {
	colorVertices.erase(v);
}

void Renderer::setColorVertexPosition(unsigned int v, float x, float y) {
	colorVertices[v].x = x;
	colorVertices[v].y = y;
}

void Renderer::setColorVertexColor(unsigned int v, char r, char g, char b, char a) {
	colorVertices[v].r = r;
	colorVertices[v].g = g;
	colorVertices[v].b = b;
	colorVertices[v].a = a;
}

void Renderer::initLineLayer(LineLayer* lineLayer) {
	glGenBuffers(1, &(lineLayer->vertexBufferId));
	glGenBuffers(1, &(lineLayer->indexBufferId));
	lineLayer->vaoId = colorShader->createVertexArrayObject(lineLayer->vertexBufferId, sizeof(ColorVertex), 0, (GLvoid*)(2 * sizeof(float)));
}

void Renderer::addLineToLayer(LineLayer* lineLayer, unsigned int v1, unsigned int v2) {
	//SpriteLayer& layer = spriteLayers[layerId];

	unordered_map<unsigned int, int>::iterator it;

	//ColorVertex v;

	it = lineLayer->indices.find(v1);
	if (it == lineLayer->indices.end()) {
		lineLayer->vertexBufferData.push_back(colorVertices[v1]);
		lineLayer->indices[v1] = lineLayer->vertexBufferData.size() - 1;
	}

	it = lineLayer->indices.find(v2);
	if (it == lineLayer->indices.end()) {
		lineLayer->vertexBufferData.push_back(colorVertices[v2]);
		lineLayer->indices[v2] = lineLayer->vertexBufferData.size() - 1;
	}

	lineLayer->indexBufferData.push_back(lineLayer->indices[v1]);
	lineLayer->indexBufferData.push_back(lineLayer->indices[v2]);
}

void Renderer::drawLineLayer(LineLayer* lineLayer) {
	colorShader->bind();
	glBindVertexArray(lineLayer->vaoId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineLayer->indexBufferId);
	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
	glDrawElements(GL_LINES, lineLayer->indexBufferData.size(), GL_UNSIGNED_INT, 0);
	//glDisable(GL_BLEND);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	colorShader->unbind();
}

void Renderer::initTriangleLayer(TriangleLayer* triangleLayer) {
	glGenBuffers(1, &(triangleLayer->vertexBufferId));
	glGenBuffers(1, &(triangleLayer->indexBufferId));
	triangleLayer->vaoId = colorShader->createVertexArrayObject(triangleLayer->vertexBufferId, sizeof(ColorVertex), 0, (GLvoid*)(2 * sizeof(float)));
}

void Renderer::addTriangleToLayer(TriangleLayer* triangleLayer, unsigned int v1, unsigned int v2, unsigned int v3) {
	//SpriteLayer& layer = spriteLayers[layerId];

	unordered_map<unsigned int, int>::iterator it;

	//ColorVertex v;

	it = triangleLayer->indices.find(v1);
	if (it == triangleLayer->indices.end()) {
		triangleLayer->vertexBufferData.push_back(colorVertices[v1]);
		triangleLayer->indices[v1] = triangleLayer->vertexBufferData.size() - 1;
	}

	it = triangleLayer->indices.find(v2);
	if (it == triangleLayer->indices.end()) {
		triangleLayer->vertexBufferData.push_back(colorVertices[v2]);
		triangleLayer->indices[v2] = triangleLayer->vertexBufferData.size() - 1;
	}

	it = triangleLayer->indices.find(v3);
	if (it == triangleLayer->indices.end()) {
		triangleLayer->vertexBufferData.push_back(colorVertices[v3]);
		triangleLayer->indices[v3] = triangleLayer->vertexBufferData.size() - 1;
	}

	triangleLayer->indexBufferData.push_back(triangleLayer->indices[v1]);
	triangleLayer->indexBufferData.push_back(triangleLayer->indices[v2]);
	triangleLayer->indexBufferData.push_back(triangleLayer->indices[v3]);
}

void Renderer::drawTriangleLayer(TriangleLayer* triangleLayer) {
	colorShader->bind();
	glBindVertexArray(triangleLayer->vaoId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleLayer->indexBufferId);
	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);*/
	glDrawElements(GL_TRIANGLES, triangleLayer->indexBufferData.size(), GL_UNSIGNED_INT, 0);
	//glDisable(GL_BLEND);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	colorShader->unbind();
}

}
