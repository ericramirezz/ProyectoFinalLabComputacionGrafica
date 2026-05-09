#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Shader.h"
#include "Mesh.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
using namespace std;

#define NUM_HUESOS_POR_VERTICE 4

struct DatosHueso
{
	aiMatrix4x4 offset_matrix;
	aiMatrix4x4 final_world_transform;
};

struct PesosVertice
{
	GLuint ids[NUM_HUESOS_POR_VERTICE];
	float pesos[NUM_HUESOS_POR_VERTICE];

	PesosVertice()
	{
		memset(ids, 0, sizeof(ids));
		memset(pesos, 0, sizeof(pesos));
	}

	void agregarDatoHueso(GLuint idHueso, float peso)
	{
		for (GLuint i = 0; i < NUM_HUESOS_POR_VERTICE; i++)
		{
			if (pesos[i] == 0.0)
			{
				ids[i] = idHueso;
				pesos[i] = peso;
				return;
			}
		}
	}
};

class MallaAnimada {
public:
	// Datos de la malla
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> texturas;
	vector<PesosVertice> pesosHuesosPorVertice;
	GLuint VAO;

	// Constructor sin huesos
	MallaAnimada(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> texturas)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->texturas = texturas;

		this->configurarMalla();
	}

	// Constructor con huesos
	MallaAnimada(vector<Vertex> vertices, vector<GLuint> indices, vector<Texture> texturas, vector<PesosVertice> pesosHuesos)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->texturas = texturas;
		this->pesosHuesosPorVertice = pesosHuesos;

		this->configurarMalla();
	}

	// Dibuja la malla
	void Draw(Shader shader)
	{
		GLuint numDifuso = 1;
		GLuint numEspecular = 1;
		GLuint numNormal = 1;
		GLuint numAltura = 1;

		for (GLuint i = 0; i < this->texturas.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);

			string numero;
			string nombre = this->texturas[i].type;

			if (nombre == "texture_diffuse")
				numero = std::to_string(numDifuso++);
			else if (nombre == "texture_specular")
				numero = std::to_string(numEspecular++);
			else if (nombre == "texture_normal")
				numero = std::to_string(numNormal++);
			else if (nombre == "texture_height")
				numero = std::to_string(numAltura++);

			glUniform1i(glGetUniformLocation(shader.Program, (nombre + numero).c_str()), i);
			glBindTexture(GL_TEXTURE_2D, this->texturas[i].id);
		}

		// Dibujar malla
		glBindVertexArray(this->VAO);
		glDrawElements(GL_TRIANGLES, this->indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
	}

private:
	GLuint VBO, EBO, VBO_huesos;

	void configurarMalla()
	{
		glGenVertexArrays(1, &this->VAO);
		glGenBuffers(1, &this->VBO);
		glGenBuffers(1, &this->EBO);
		glGenBuffers(1, &this->VBO_huesos);

		glBindVertexArray(this->VAO);

		// Buffer de vertices
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
		glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

		// Buffer de huesos
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO_huesos);
		glBufferData(GL_ARRAY_BUFFER, this->pesosHuesosPorVertice.size() * sizeof(PesosVertice), &this->pesosHuesosPorVertice[0], GL_STATIC_DRAW);

		// Buffer de indices
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

		// Atributos de vertices
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO);

		// Posiciones
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		// Normales
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		// Coordenadas de textura
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
		// Tangentes
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
		// Bitangentes
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

		// Atributos de huesos
		glBindBuffer(GL_ARRAY_BUFFER, this->VBO_huesos);

		// IDs de huesos
		glEnableVertexAttribArray(5);
		glVertexAttribIPointer(5, 4, GL_INT, sizeof(PesosVertice), (void*)0);
		// Pesos de huesos
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(PesosVertice), (void*)offsetof(PesosVertice, pesos));

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
		glBindVertexArray(0);
	}
};
