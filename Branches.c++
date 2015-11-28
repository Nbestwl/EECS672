// Branches.c++

#include <iostream>
#include <math.h>

#include "Branches.h"

Branches::Branches(vec3 location, float radius, float height, vec3 color) : displayCylEdges(false), displayCylFill(true), radius_(radius), height_(height)
{
	location_[0] = location[0];
	location_[1] = location[1];
	location_[2] = location[2];
	color_[0] = color[0];
	color_[1] = color[1];
	color_[2] = color[2];

	limits[0] = location_[0] + -1.0 * radius_;
  limits[1] = location_[0] + 1.0 * radius_;
  limits[2] = location_[1] + -1.0 * (height_ / 2.0);
  limits[3] = location_[1] + 1.0 * (height_ / 2.0);
  limits[4] = location_[2] + -1.0 * radius_;
  limits[5] = location_[2] + 1.0 * radius_;

	defineBranches();
}

Branches::~Branches()
{
	glDeleteBuffers(2, vbo);
	glDeleteVertexArrays(1, vao);
}

const int N_POINTS_AROUND_SLICE = 18; // number points around a cross-section

void Branches::defineBranches()
{
	typedef float vec3[3];
	int nPoints = 2 * (N_POINTS_AROUND_SLICE + 1);
	vec3* coords = new vec3[nPoints];
	vec3* normals = new vec3[nPoints];
	vec3* coords_1 = new vec3[N_POINTS_AROUND_SLICE];
	vec3* coords_2 = new vec3[N_POINTS_AROUND_SLICE];
	double theta = 0.0;
	double dTheta = 2.0*M_PI/N_POINTS_AROUND_SLICE;
	double vb = location_[2];
  double zb = location_[0];
  double z1 = location_[1] - height_/2.0;
  double z2 = location_[1] + height_/2.0;
  double r = radius_;

  for( int i = 0; i <= N_POINTS_AROUND_SLICE; ++i )
    {
      int j = 2 * i;
      double dx = cos( theta );
      double dz = sin( theta );

      normals[j][0] = dx; normals[j][1] = 0.0; normals[j][2] = dz;
      coords[j][0] = zb + r*dx; coords[j][1] = z1; coords[j][2] = vb + r*dz;
      normals[j+1][0] = dx; normals[j+1][1] = 0.0; normals[j+1][2] = dz;
      coords[j+1][0] = zb + r*dx; coords[j+1][1] = z2; coords[j+1][2] = vb + r*dz;
      theta += dTheta;
    }
	//theta = 0.0;
	//for(int i = 0; i < N_POINTS_AROUND_SLICE; ++i)
	//{
	//	double dx = cos(theta);
	//	double dz = sin(theta);
	//	coords_1[i][0] = zb + r*dx; coords_1[i][1] = z1; coords_1[i][2] = vb + r*dz;
	//	theta += dTheta;
	//}
	//theta = 0.0;
	//for(int j = 0; j < N_POINTS_AROUND_SLICE; ++j)
	//{
	//	double dx = cos(theta);
	//	double dz = sin(theta);
	//	coords_2[j][0] = zb + r*dx; coords_2[j][1] = z2; coords_2[j][2] = vb + r*dz;
	//	theta += dTheta;
	//}

	// ************************************************************************
	// EXERCISE: Create/fill VAOs and VBOs here.
	//           Also use glVertexAttribPointer and glEnableVertexAttribArray
	// ************************************************************************
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);

	glGenBuffers(2, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, nPoints*sizeof(vec3), coords, GL_STATIC_DRAW);
	glVertexAttribPointer(pvaLoc_mcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(pvaLoc_mcPosition);


	glBindBuffer( GL_ARRAY_BUFFER, vbo[1] );
	glBufferData( GL_ARRAY_BUFFER, sizeof( vec3 ) * nPoints, normals, GL_STATIC_DRAW );
	glVertexAttribPointer( pvaLoc_mcNormal, 3, GL_FLOAT, GL_TRUE, 0, 0 );
	glEnableVertexAttribArray( pvaLoc_mcNormal );

	//glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	//glBufferData(GL_ARRAY_BUFFER, N_POINTS_AROUND_SLICE*sizeof(vec3), coords_1, GL_STATIC_DRAW);
	//glVertexAttribPointer(pvaLoc_mcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(pvaLoc_mcPosition);

	//glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	//glBufferData(GL_ARRAY_BUFFER, N_POINTS_AROUND_SLICE*sizeof(vec3), coords_2, GL_STATIC_DRAW);
	//glVertexAttribPointer(pvaLoc_mcPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
	//glEnableVertexAttribArray(pvaLoc_mcPosition);

	delete [] coords;
	delete [] normals;
	//delete [] coords_1;
	//delete [] coords_2;
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Branches::getMCBoundingBox(double* xyzLimits) const
{
	memcpy( xyzLimits, limits, 6 * sizeof( double ) );
}

void Branches::handleCommand(unsigned char key, double ldsX, double ldsY)
{
	if (key == 'c')
		displayCylFill = !displayCylFill;
	else if (key == 'C')
		displayCylEdges = !displayCylEdges;
	else
		this->ModelView::handleCommand(key, ldsX, ldsY);
}

void Branches::renderBranches(float* color)
{
	typedef float vec3[3];
	vec3 cylColor;
	cylColor[0] = color_[0];
	cylColor[1] = color_[1];
	cylColor[2] = color_[2];
	if (color == NULL)
		color = cylColor;
	glUniform3fv(ppuLoc_kd, 1, color);
	glBindVertexArray(vao[0]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 2*(N_POINTS_AROUND_SLICE+1));
}

void Branches::render()
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

	if (displayCylFill)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		renderBranches(NULL);
	}
	if (displayCylEdges)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		renderBranches(black);
	}

	glUseProgram(pgm);
}
