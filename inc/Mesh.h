#pragma once

#include <vector>
#include "assimp/Importer.hpp"
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#if defined(__APPLE__)
#  include <OpenGL/gl3.h> // defines OpenGL 3.0+ functions
#else
#  if defined(_WIN32)
#    define GLEW_STATIC 1
#  endif
#  include <GL/glew.h>
#endif
#include <FL/gl.h> // for gl_texture_reset()


#include <string>

class MyCubeViewUI;
class Mesh
{
public:
	GLuint EBO;
	GLuint VBO;
	GLuint VAO;
	GLuint texture;

	std::vector<float> testVertices;
	std::vector<unsigned int> testIndices;
	std::vector<float> testNormals;

	aiMesh* mesh;

	Mesh(aiMesh* mesh, std::string diffuseTexture, MyCubeViewUI* ui);

	void Draw();
	void Init(std::string diffuseTexture, MyCubeViewUI* ui);
};

