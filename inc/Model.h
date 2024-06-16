#pragma once
#include <vector>
#include "Mesh.h"

class Model {
public:
	Model() {}
	Model(std::string modelPath, MyCubeViewUI* uiStuff);
	~Model();
	
	void Draw();

	std::vector<Mesh> meshes;
};
