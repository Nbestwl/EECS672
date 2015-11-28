// Block.c++

#include <iostream>
#include <math.h>

#include "Block.h"
#include "ShaderIF.h"
#include <iostream>
typedef float vec3[3];
// index lists for the three faces that can't be drawn with glDrawArrays
GLuint Block::indexList[3][4] = {
	{ 6, 7, 0, 1 }, // xmin face
	{ 6, 0, 4, 2 }, // ymin face
	{ 1, 7, 3, 5 }  // ymax face
};

Block::Block(cryph::AffPoint center, cryph::AffVector direction_, vec3 length, vec3 degrees, vec3 mycolor):
	displayBlockEdges(false), displayBlockFill(true), direction(direction_)
{
	degrees[0] = angle1;
	degrees[1] = angle2;
	degrees[2] = angle3;
	colors[0] = mycolor[0];
	colors[1] = mycolor[1];
	colors[3] = mycolor[2];
	lx = length[0];
	ly = length[1];
	lz = length[2];

	cryph::Matrix4x4 matrix = cryph::Matrix4x4::translation(direction);
	cryph::Matrix4x4 xrt = cryph::Matrix4x4::xRotationDegrees(angle1);
	cryph::Matrix4x4 yrt = cryph::Matrix4x4::yRotationDegrees(angle2);
	cryph::Matrix4x4 zrt = cryph::Matrix4x4::zRotationDegrees(angle3);
	cryph::Matrix4x4 Final = matrix * xrt * yrt * zrt;
	xmin = center.x - lx/2; xmax = center.x + lx/2;
	ymin = center.y - ly/2; ymax = center.y + ly/2;
	zmin = center.z - lz/2; zmax = center.z + lz/2;

	defineBlock();
}

Block::~Block()
{
	glDeleteBuffers(3, ebo);
	glDeleteBuffers(1, vbo);
	glDeleteVertexArrays(1, vao);
}

void Block::defineBlock()
{
	vec3 vtx[] = { // The 8 unique vertices (Note the order)
		{xmin ,ymin, zmax}, {xmin, ymax, zmax},
		{xmax, ymin, zmax}, {xmax, ymax, zmax},
		{xmax, ymin, zmin}, {xmax, ymax, zmin},
		{xmin, ymin, zmin}, {xmin, ymax, zmin}
	};
for(int i = 0; i < 8; i++)
{
	cryph::AffPoint temp = Final * cryph::AffPoint(vtx[i]);
	temp.aCoords(vtx[i]);
}

	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(1, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 8*sizeof(vec3), vtx, GL_STATIC_DRAW);
	glVertexAttribPointer(pvaLoc_mcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pvaLoc_mcPosition);

	glGenBuffers(3, ebo);
	for (int i=0 ; i<3 ; i++)
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4*sizeof(GLuint), indexList[i], GL_STATIC_DRAW);
	}

	glDisableVertexAttribArray(pvaLoc_mcNormal);
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Block::getMCBoundingBox(double* xyzLimits) const
{
	xyzLimits[0] = xmin;
	xyzLimits[1] = xmax;
	xyzLimits[2] = ymin;
	xyzLimits[3] = ymax;
	xyzLimits[4] = zmin;
	xyzLimits[5] = zmax;
}

// void Block::handleCommand(unsigned char key, double ldsX, double ldsY)
// {
// 	if (key == 'b')
// 		displayBlockFill = !displayBlockFill;
// 	else if (key == 'B')
// 		displayBlockEdges = !displayBlockEdges;
// 	else
// 		this->ModelView::handleCommand(key, ldsX, ldsY);
// }

void Block::renderBlock(float* color)
{
	vec3 bColor;
	bColor[0] = colors[0];
	bColor[1] = colors[1];
	bColor[2] = colors[2];
	if(color == NULL)
	{
		color = bColor;
	}
	vec3 ka, kd;
	establishLights();

	glUniform3fv(ppuLoc_ka, 1, color);
  glUniform3fv(ppuLoc_kd, 1, color);
  glUniform3fv(ppuLoc_ks, 1, color);
  glUniform1f (ppuLoc_m, 10);

	glBindVertexArray(vao[0]);
	// The three faces that can be drawn with glDrawArrays
	glVertexAttrib3f(pvaLoc_mcNormal, 0.0, 0.0, 1.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glVertexAttrib3f(pvaLoc_mcNormal, 1.0, 0.0, 0.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 2, 4);
	glVertexAttrib3f(pvaLoc_mcNormal, 0.0, 0.0, -1.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

	// The three faces that are drawn with glDrawElements
	glVertexAttrib3f(pvaLoc_mcNormal, -1.0, 0.0, 0.0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, NULL);
	glVertexAttrib3f(pvaLoc_mcNormal, 0.0, -1.0, 0.0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, NULL);
	glVertexAttrib3f(pvaLoc_mcNormal, 0.0, 1.0, 0.0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[2]);
	glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, NULL);
}

void Block::render()
{
	GLint pgm;
	glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
	glUseProgram(shaderProgram);

	cryph::Matrix4x4 mc_ec, ec_lds;
	getMatrices(mc_ec, ec_lds);
	float mat[16];
	glUniformMatrix4fv(ppuLoc_mc_ec, 1, false, mc_ec.extractColMajor(mat));
	glUniformMatrix4fv(ppuLoc_ec_lds, 1, false, ec_lds.extractColMajor(mat));

	float black[] = { 0.0, 0.0, 0.0 };

	if (displayBlockFill)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		renderBlock(NULL);
	}
	if (displayBlockEdges)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		renderBlock(black);
	}

	glUseProgram(pgm);
}
