#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "SOIL2/SOIL2.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "meshAnim.h"
#include "Shader.h"

using namespace std;

GLint CargarTexturaAnimacion(const char* ruta, string directorio);

class ModeloAnimado
{
public:
	ModeloAnimado(GLchar* ruta)
	{
		this->numHuesos = 0;
		this->escena = this->importador.ReadFile(ruta, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
		if (!this->escena || this->escena->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !this->escena->mRootNode) { cout << "ERROR::ASSIMP:: " << this->importador.GetErrorString() << endl; return; }
		this->transformacionGlobalInversa = this->escena->mRootNode->mTransformation;
		this->transformacionGlobalInversa.Inverse();
		this->directorio = string(ruta);
		this->directorio = this->directorio.substr(0, this->directorio.find_last_of('/'));
		this->procesarNodo(this->escena->mRootNode, this->escena);
		printf("Modelo animado cargado: %s\n", ruta);
		printf("  Mallas: %d, Huesos: %d, Animaciones: %d\n", (int)this->mallas.size(), (int)this->numHuesos, (int)this->escena->mNumAnimations);
	}

	void Draw(Shader shader, GLfloat tiempoEnSegundos)
	{
		vector<aiMatrix4x4> transformaciones;
		this->calcularTransformacionHuesos(tiempoEnSegundos, transformaciones);
		for (GLuint i = 0; i < transformaciones.size(); i++)
		{
			string nombre = "bones[" + to_string(i) + "]";
			glUniformMatrix4fv(glGetUniformLocation(shader.Program, nombre.c_str()), 1, GL_TRUE, (const GLfloat*)&transformaciones[i]);
		}
		for (GLuint i = 0; i < this->mallas.size(); i++) { this->mallas[i].Draw(shader); }
	}

private:
	vector<MallaAnimada> mallas;
	string directorio;
	vector<Texture> texturasCargadas;
	map<string, GLuint> mapeoHuesos;
	GLuint numHuesos;
	vector<DatosHueso> matricesHuesos;
	aiMatrix4x4 transformacionGlobalInversa;
	Assimp::Importer importador;
	const aiScene* escena;

	void procesarNodo(aiNode* nodo, const aiScene* escena) { for (GLuint i = 0; i < nodo->mNumMeshes; i++) { aiMesh* malla = escena->mMeshes[nodo->mMeshes[i]]; this->mallas.push_back(this->procesarMalla(malla, escena)); } for (GLuint i = 0; i < nodo->mNumChildren; i++) { this->procesarNodo(nodo->mChildren[i], escena); } }

	MallaAnimada procesarMalla(aiMesh* malla, const aiScene* escena)
	{
		vector<Vertex> vertices; vector<GLuint> indices; vector<Texture> texturas; vector<PesosVertice> pesosHuesosPorVertice;
		vertices.resize(malla->mNumVertices); pesosHuesosPorVertice.resize(malla->mNumVertices);
		for (GLuint i = 0; i < malla->mNumVertices; i++) { Vertex vertice; glm::vec3 vec; vec.x = malla->mVertices[i].x; vec.y = malla->mVertices[i].y; vec.z = malla->mVertices[i].z; vertice.Position = vec; if (malla->mNormals) { vec.x = malla->mNormals[i].x; vec.y = malla->mNormals[i].y; vec.z = malla->mNormals[i].z; vertice.Normal = vec; } if (malla->mTextureCoords[0]) { glm::vec2 coordTex; coordTex.x = malla->mTextureCoords[0][i].x; coordTex.y = malla->mTextureCoords[0][i].y; vertice.TexCoords = coordTex; } else { vertice.TexCoords = glm::vec2(0.0f, 0.0f); } if (malla->mTangents) { vec.x = malla->mTangents[i].x; vec.y = malla->mTangents[i].y; vec.z = malla->mTangents[i].z; vertice.Tangent = vec; } if (malla->mBitangents) { vec.x = malla->mBitangents[i].x; vec.y = malla->mBitangents[i].y; vec.z = malla->mBitangents[i].z; vertice.Bitangent = vec; } vertices[i] = vertice; }
		for (GLuint i = 0; i < malla->mNumFaces; i++) { aiFace cara = malla->mFaces[i]; for (GLuint j = 0; j < cara.mNumIndices; j++) { indices.push_back(cara.mIndices[j]); } }
		if (malla->mMaterialIndex >= 0) { aiMaterial* material = escena->mMaterials[malla->mMaterialIndex]; vector<Texture> mapasDifusos = this->cargarTexturasDelMaterial(material, aiTextureType_DIFFUSE, "texture_diffuse"); texturas.insert(texturas.end(), mapasDifusos.begin(), mapasDifusos.end()); vector<Texture> mapasEspeculares = this->cargarTexturasDelMaterial(material, aiTextureType_SPECULAR, "texture_specular"); texturas.insert(texturas.end(), mapasEspeculares.begin(), mapasEspeculares.end()); }
		for (GLuint i = 0; i < malla->mNumBones; i++) { GLuint indiceHueso = 0; string nombreHueso(malla->mBones[i]->mName.data); if (this->mapeoHuesos.find(nombreHueso) == this->mapeoHuesos.end()) { indiceHueso = this->numHuesos; this->numHuesos++; DatosHueso datosH; this->matricesHuesos.push_back(datosH); this->matricesHuesos[indiceHueso].offset_matrix = malla->mBones[i]->mOffsetMatrix; this->mapeoHuesos[nombreHueso] = indiceHueso; } else { indiceHueso = this->mapeoHuesos[nombreHueso]; } for (GLuint j = 0; j < malla->mBones[i]->mNumWeights; j++) { GLuint idVertice = malla->mBones[i]->mWeights[j].mVertexId; float peso = malla->mBones[i]->mWeights[j].mWeight; pesosHuesosPorVertice[idVertice].agregarDatoHueso(indiceHueso, peso); } }
		return MallaAnimada(vertices, indices, texturas, pesosHuesosPorVertice);
	}

	vector<Texture> cargarTexturasDelMaterial(aiMaterial* mat, aiTextureType tipo, string nombreTipo) { vector<Texture> texturas; for (GLuint i = 0; i < mat->GetTextureCount(tipo); i++) { aiString str; mat->GetTexture(tipo, i, &str); GLboolean saltar = false; for (GLuint j = 0; j < this->texturasCargadas.size(); j++) { if (this->texturasCargadas[j].path == str) { texturas.push_back(this->texturasCargadas[j]); saltar = true; break; } } if (!saltar) { Texture textura; textura.id = CargarTexturaAnimacion(str.C_Str(), this->directorio); textura.type = nombreTipo; textura.path = str; texturas.push_back(textura); this->texturasCargadas.push_back(textura); } } return texturas; }

	void calcularTransformacionHuesos(double tiempoEnSegundos, vector<aiMatrix4x4>& transformaciones) { aiMatrix4x4 matrizIdentidad; if (this->escena->mNumAnimations == 0) { transformaciones.resize(this->numHuesos); for (GLuint i = 0; i < this->numHuesos; i++) transformaciones[i] = aiMatrix4x4(); return; } double ticksPorSegundo = this->escena->mAnimations[0]->mTicksPerSecond != 0 ? this->escena->mAnimations[0]->mTicksPerSecond : 25.0f; double tiempoEnTicks = tiempoEnSegundos * ticksPorSegundo; float tiempoAnimacion = fmod(tiempoEnTicks, this->escena->mAnimations[0]->mDuration); this->recorrerJerarquiaNodos(tiempoAnimacion, this->escena->mRootNode, matrizIdentidad); transformaciones.resize(this->numHuesos); for (GLuint i = 0; i < this->numHuesos; i++) { transformaciones[i] = this->matricesHuesos[i].final_world_transform; } }

	void recorrerJerarquiaNodos(float tiempoAnimacion, const aiNode* nodo, const aiMatrix4x4& transformacionPadre)
	{
		string nombreNodo(nodo->mName.data);
		const aiAnimation* animacion = this->escena->mAnimations[0];
		aiMatrix4x4 transformacionNodo = nodo->mTransformation;

		const aiNodeAnim* canal = this->buscarCanalAnimacion(animacion, nombreNodo);

		if (canal)
		{
			aiVector3D escala;
			this->interpolarEscala(escala, tiempoAnimacion, canal);
			aiMatrix4x4 matEscala;
			aiMatrix4x4::Scaling(escala, matEscala);

			aiQuaternion rotacion;
			this->interpolarRotacion(rotacion, tiempoAnimacion, canal);
			aiMatrix4x4 matRotacion = aiMatrix4x4(rotacion.GetMatrix());

			aiVector3D posicion;
			this->interpolarPosicion(posicion, tiempoAnimacion, canal);

			if (nombreNodo.find("Hips") != string::npos || nombreNodo.find("hips") != string::npos)
			{
				posicion.x = 0.0f;
				posicion.z = 0.0f;
			}

			aiMatrix4x4 matPosicion;
			aiMatrix4x4::Translation(posicion, matPosicion);
			transformacionNodo = matPosicion * matRotacion * matEscala;
		}

		aiMatrix4x4 transformacionGlobal = transformacionPadre * transformacionNodo;

		if (this->mapeoHuesos.find(nombreNodo) != this->mapeoHuesos.end())
		{
			GLuint indiceHueso = this->mapeoHuesos[nombreNodo];
			this->matricesHuesos[indiceHueso].final_world_transform =
				this->transformacionGlobalInversa * transformacionGlobal * this->matricesHuesos[indiceHueso].offset_matrix;
		}

		for (GLuint i = 0; i < nodo->mNumChildren; i++)
		{
			this->recorrerJerarquiaNodos(tiempoAnimacion, nodo->mChildren[i], transformacionGlobal);
		}
	}

	const aiNodeAnim* buscarCanalAnimacion(const aiAnimation* animacion, const string& nombreNodo) { for (GLuint i = 0; i < animacion->mNumChannels; i++) { const aiNodeAnim* canal = animacion->mChannels[i]; if (string(canal->mNodeName.data) == nombreNodo) return canal; } return nullptr; }

	void interpolarPosicion(aiVector3D& resultado, float tiempoAnimacion, const aiNodeAnim* canal) { if (canal->mNumPositionKeys == 1) { resultado = canal->mPositionKeys[0].mValue; return; } GLuint indice = this->buscarIndicePosicion(tiempoAnimacion, canal); GLuint siguiente = indice + 1; float deltaTiempo = (float)(canal->mPositionKeys[siguiente].mTime - canal->mPositionKeys[indice].mTime); float factor = (tiempoAnimacion - (float)canal->mPositionKeys[indice].mTime) / deltaTiempo; const aiVector3D& inicio = canal->mPositionKeys[indice].mValue; const aiVector3D& fin = canal->mPositionKeys[siguiente].mValue; resultado = inicio + factor * (fin - inicio); }

	void interpolarRotacion(aiQuaternion& resultado, float tiempoAnimacion, const aiNodeAnim* canal) { if (canal->mNumRotationKeys == 1) { resultado = canal->mRotationKeys[0].mValue; return; } GLuint indice = this->buscarIndiceRotacion(tiempoAnimacion, canal); GLuint siguiente = indice + 1; float deltaTiempo = (float)(canal->mRotationKeys[siguiente].mTime - canal->mRotationKeys[indice].mTime); float factor = (tiempoAnimacion - (float)canal->mRotationKeys[indice].mTime) / deltaTiempo; const aiQuaternion& inicio = canal->mRotationKeys[indice].mValue; const aiQuaternion& fin = canal->mRotationKeys[siguiente].mValue; aiQuaternion::Interpolate(resultado, inicio, fin, factor); resultado = resultado.Normalize(); }

	void interpolarEscala(aiVector3D& resultado, float tiempoAnimacion, const aiNodeAnim* canal) { if (canal->mNumScalingKeys == 1) { resultado = canal->mScalingKeys[0].mValue; return; } GLuint indice = this->buscarIndiceEscala(tiempoAnimacion, canal); GLuint siguiente = indice + 1; float deltaTiempo = (float)(canal->mScalingKeys[siguiente].mTime - canal->mScalingKeys[indice].mTime); float factor = (tiempoAnimacion - (float)canal->mScalingKeys[indice].mTime) / deltaTiempo; const aiVector3D& inicio = canal->mScalingKeys[indice].mValue; const aiVector3D& fin = canal->mScalingKeys[siguiente].mValue; resultado = inicio + factor * (fin - inicio); }

	GLuint buscarIndicePosicion(float tiempoAnimacion, const aiNodeAnim* canal) { for (GLuint i = 0; i < canal->mNumPositionKeys - 1; i++) { if (tiempoAnimacion < (float)canal->mPositionKeys[i + 1].mTime) return i; } return 0; }
	GLuint buscarIndiceRotacion(float tiempoAnimacion, const aiNodeAnim* canal) { for (GLuint i = 0; i < canal->mNumRotationKeys - 1; i++) { if (tiempoAnimacion < (float)canal->mRotationKeys[i + 1].mTime) return i; } return 0; }
	GLuint buscarIndiceEscala(float tiempoAnimacion, const aiNodeAnim* canal) { for (GLuint i = 0; i < canal->mNumScalingKeys - 1; i++) { if (tiempoAnimacion < (float)canal->mScalingKeys[i + 1].mTime) return i; } return 0; }
};

GLint CargarTexturaAnimacion(const char* ruta, string directorio) { string nombreArchivo = string(ruta); nombreArchivo = directorio + '/' + nombreArchivo; GLuint idTextura; glGenTextures(1, &idTextura); int ancho, alto; unsigned char* imagen = SOIL_load_image(nombreArchivo.c_str(), &ancho, &alto, 0, SOIL_LOAD_RGBA); printf("Textura Anim: %s -> %s (w:%d h:%d)\n", nombreArchivo.c_str(), imagen ? "OK" : "FAILED", ancho, alto); if (!imagen) { printf("  SOIL Error: %s\n", SOIL_last_result()); return idTextura; } glBindTexture(GL_TEXTURE_2D, idTextura); glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ancho, alto, 0, GL_RGBA, GL_UNSIGNED_BYTE, imagen); glGenerateMipmap(GL_TEXTURE_2D); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); glBindTexture(GL_TEXTURE_2D, 0); SOIL_free_image_data(imagen); return idTextura; }
