// Branches.h

#ifndef BRANCHES_H
#define BRANCHES_H

#include <GL/gl.h>
#include <cstring>
#include "ModelViewWithPhongLighting.h"

class Branches : public ModelViewWithPhongLighting
{
public:
	Branches(vec3 location, float radius, float height, vec3 color);
	virtual ~Branches();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimits) const;
	void handleCommand(unsigned char key, double ldsX, double ldsY);
	void render();
private:
	GLuint vao[1];
	GLuint vbo[2]; // 0: coordinates; 1: normal vectors

	bool displayCylEdges, displayCylFill;

	void defineBranches();
	void renderBranches(float* color);
 	double limits[6];
	vec3 color_;
	float radius_;
	float height_;
	vec3 location_;
};

#endif
