// ModelViewWithPhongLighting.c++

#include <iostream>
#include <math.h>

#include "AffPoint.h"
#include "AffVector.h"
#include "Matrix4x4.h"

#include "ModelViewWithPhongLighting.h"
#include "ShaderIF.h"

ShaderIF* ModelViewWithPhongLighting::shaderIF = NULL;
int ModelViewWithPhongLighting::numInstances = 0;
GLuint ModelViewWithPhongLighting::shaderProgram = 0;
GLint ModelViewWithPhongLighting::pvaLoc_mcPosition = -2;
GLint ModelViewWithPhongLighting::pvaLoc_mcNormal = -2;
GLint ModelViewWithPhongLighting::ppuLoc_kd = -2;
GLint ModelViewWithPhongLighting::ppuLoc_mc_ec = -2;
GLint ModelViewWithPhongLighting::ppuLoc_ec_lds = -2;
GLint ModelViewWithPhongLighting::ppuLoc_lightStrength = -2;
GLint ModelViewWithPhongLighting::ppuLoc_globalAmbient = -2;
GLint ModelViewWithPhongLighting::ppuLoc_ka = -2;
GLint ModelViewWithPhongLighting::ppuLoc_ks = -2;
GLint ModelViewWithPhongLighting::ppuLoc_m = 0;
GLint ModelViewWithPhongLighting::uLoc_p_ecLightPos = -2;
GLint ModelViewWithPhongLighting::uLoc_lightStrength = -2;
int ModelViewWithPhongLighting::uLoc_actualNumLights = 3;
GLint ModelViewWithPhongLighting::uLoc_globalAmbient = -2;

float ModelViewWithPhongLighting::lightPosition[] = {
   -100.0, 100.0, 0.0, 0.0, // source 0: directional
   100.0, -100.0, 1.0, 0.0,  // source 1: directional
   -100, 200, 0.0, 1.0 // source 2: an actual location in the scene
   };
float ModelViewWithPhongLighting::lightStrength[] = {
    1.0, 1.0, 1.0, // source 0: full strength white
   0.4, 0.4, 0.4, // source 1: 40% white
    0.8, 0.8, 0.8  // source 2: 80% strength yellow
   };
float ModelViewWithPhongLighting::ambientStrength[] = { 0.15, 0.15, 0.15 }; // assumed ambient light

std::string ModelViewWithPhongLighting::vShaderSource = "simple3d.vsh";
std::string ModelViewWithPhongLighting::fShaderSource = "simple3d.fsh";

ModelViewWithPhongLighting::ModelViewWithPhongLighting()
{
	if (shaderProgram == 0)
	{
		shaderIF = new ShaderIF(vShaderSource, fShaderSource);
		shaderProgram = shaderIF->getShaderPgmID();
		fetchGLSLVariableLocations();
	}
	numInstances++;
}

ModelViewWithPhongLighting::~ModelViewWithPhongLighting()
{
	if (--numInstances == 0)
	{
		shaderIF->destroy();
		delete shaderIF;
		shaderIF = NULL;
		shaderProgram = 0;
	}
}

void ModelViewWithPhongLighting::fetchGLSLVariableLocations()
{
	if (shaderProgram > 0)
	{
		pvaLoc_mcPosition = pvAttribLocation(shaderProgram, "mcPosition");
		pvaLoc_mcNormal = pvAttribLocation(shaderProgram, "mcNormal");
		ppuLoc_mc_ec = ppUniformLocation(shaderProgram, "mc_ec");
		ppuLoc_ec_lds = ppUniformLocation(shaderProgram, "ec_lds");
		ppuLoc_ka = ppUniformLocation(shaderProgram, "ka");
		ppuLoc_kd = ppUniformLocation(shaderProgram, "kd");
		ppuLoc_ks = ppUniformLocation(shaderProgram, "ks");
		ppuLoc_m = ppUniformLocation(shaderProgram, "m");
    uLoc_p_ecLightPos = ppUniformLocation(shaderProgram, "p_ecLightPos");
    uLoc_lightStrength = ppUniformLocation(shaderProgram, "lightStrength");
    uLoc_actualNumLights = ppUniformLocation(shaderProgram, "actualNumLights");
    uLoc_globalAmbient = ppUniformLocation(shaderProgram, "globalAmbient");
	}
}

void ModelViewWithPhongLighting::setShaderSources(const std::string& vShaderSrc, const std::string& fShaderSrc)
{
	vShaderSource = vShaderSrc;
	fShaderSource = fShaderSrc;
}

void ModelViewWithPhongLighting::handleCommand(unsigned char theChar, double ldsX, double ldsY)
{
	const unsigned char O = 79;
	const unsigned char P = 80;
	const unsigned char Q = 81;
	if(theChar == 'p' || theChar == 'P')
		ModelView::setProjection(PERSPECTIVE);
	if(theChar == 'q' || theChar == 'Q')
		ModelView::setProjection(OBLIQUE);
	if(theChar == 'o' || theChar == 'O')
		ModelView::setProjection(ORTHOGONAL);
}

void ModelViewWithPhongLighting::establishLights()
{
    // If light sources defined in MC, transform them to EC:

    // Now send the EC geometric description along with the non-geometric data:
    int numLights = 3;
    glUniform4fv(uLoc_p_ecLightPos, numLights, lightPosition);
    glUniform3fv(uLoc_lightStrength, numLights, lightStrength);
    glUniform1i(uLoc_actualNumLights, numLights);
    glUniform3fv(uLoc_globalAmbient, 1, ambientStrength);
}
