#if defined(__APPLE__)
  #ifndef GL_SILENCE_DEPRECATION
    #define GL_SILENCE_DEPRECATION 1
  #endif
#endif

#include "Mesh.h"
#include <iostream>
#include "stb_image.h"
#include "MyCubeViewUI.h"


Mesh::Mesh(aiMesh* mesh, std::string diffuseTexture, MyCubeViewUI* ui)
{
	this->mesh = mesh;
	if (mesh != nullptr) {
		Init(diffuseTexture, ui);
	}
}

Mesh::~Mesh() {
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
	//glDeleteVertexArrays(1, &VAO);
}

void Mesh::Draw()
{
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, texture);
	glDrawElements(GL_TRIANGLES, testIndices.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void Mesh::Init(std::string diffuseTexture, MyCubeViewUI* ui)
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	for (int i = 0; i < mesh->mNumVertices; i++) {
		testVertices.push_back(mesh->mVertices[i].x);
		testVertices.push_back(mesh->mVertices[i].y);
		testVertices.push_back(mesh->mVertices[i].z);

		testVertices.push_back(mesh->mNormals[i].x);
		testVertices.push_back(mesh->mNormals[i].y);
		testVertices.push_back(mesh->mNormals[i].z);

		if (mesh->HasTextureCoords(0)) {
			testVertices.push_back(mesh->mTextureCoords[0][i].x);
			testVertices.push_back(mesh->mTextureCoords[0][i].y);
		}
		else {
			testVertices.push_back(0);
			testVertices.push_back(0);
		}
	}
	for (int i = 0; i < mesh->mNumFaces; i++) {
		for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
			testIndices.push_back(mesh->mFaces[i].mIndices[j]);
		}
	}

	std::cout << "texture name for this mesh is: " << mesh->mName.C_Str() << std::endl;
	ui->meshTree->add(mesh->mName.C_Str());

	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenTextures(1, &texture);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, testVertices.size() * sizeof(float), testVertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, testIndices.size() * sizeof(unsigned int), testIndices.data(), GL_STATIC_DRAW);

	glBindTexture(GL_TEXTURE_2D, texture);

	int width, height, nrChannels;
	unsigned char* data = stbi_load(diffuseTexture.c_str(), &width, &height, &nrChannels, 0);
	if (data != nullptr) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	stbi_image_free(data);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}
