// Block.h

#ifndef BLOCK_H
#define BLOCK_H
#include <GL/gl.h>
#include "ModelViewWithPhongLighting.h"

class Block : public ModelViewWithPhongLighting
{
public:
	Block(cryph::AffPoint center, cryph::AffVector direction, vec3 length, vec3 degrees, vec3 mycolor);// lengths in 3 directions
	virtual ~Block();

	// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
	void getMCBoundingBox(double* xyzLimits) const;
	//void handleCommand(unsigned char key, double ldsX, double ldsY);
	void render();
private:
	GLuint vao[1];
	GLuint vbo[1];
	GLuint ebo[3];

	float angle1, angle2, angle3;
	float xmin, xmax, ymin, ymax, zmin, zmax, lx, ly, lz, cx, cy, cz;
	vec3 colors, length;
	cryph::Matrix4x4 Final;
	cryph::AffPoint center;
	cryph::AffVector direction;
	static GLuint indexList[3][4];

	bool displayBlockEdges, displayBlockFill;

	void defineBlock();
	void renderBlock(float* color);
};

#endif
