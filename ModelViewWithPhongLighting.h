// ModelViewWithPhongLighting.h

#ifndef MODELVIEWWITHPHONGLIGHTING_H
#define MODELVIEWWITHPHONGLIGHTING_H

#ifdef __APPLE_CC__
#include "GLFW/glfw3.h"
#else
#include <GL/gl.h>
#endif

#include "ModelView.h"
#include "ShaderIF.h"
#include "Matrix4x4.h"

typedef float vec3[3];
typedef float vec4[4];

class ModelViewWithPhongLighting : public ModelView
{
public:
	ModelViewWithPhongLighting();
	virtual ~ModelViewWithPhongLighting();

	static void setShaderSources(const std::string& vShader, const std::string& fShader);

protected:
	//added new variables for project3
	static float lightPosition[];
	static float lightStrength[];
	static float ambientStrength[];
	static GLint uLoc_p_ecLightPos;
	static GLint uLoc_lightStrength;
	static int uLoc_actualNumLights;
	static GLint uLoc_globalAmbient;
	//--------------------------------
	static ShaderIF* shaderIF;
	static int numInstances;
	static GLuint shaderProgram;
	static GLint pvaLoc_mcPosition, pvaLoc_mcNormal;
	static GLint ppuLoc_mc_ec, ppuLoc_ec_lds;
	static GLint ppuLoc_lightStrength, ppuLoc_globalAmbient, ppuLoc_ka, ppuLoc_kd, ppuLoc_ks;
	static GLint ppuLoc_m;

	static void fetchGLSLVariableLocations();
	void handleCommand(unsigned char key, double ldsX, double ldsY);
	void establishLights();
private:
	static std::string vShaderSource, fShaderSource;
};

#endif
