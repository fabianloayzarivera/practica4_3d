#pragma once
#include "common.h"
#include "Entity.h"
#include "mesh.h"

class Model : public Entity
{
private:
	std::shared_ptr<Mesh> mesh;
public:
	Model(const std::shared_ptr<Mesh>& mesh);
	virtual void draw() override;

};