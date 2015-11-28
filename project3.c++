// project2.c++: Starter for EECS 672 Project 2

#include "GLFWController.h"
#include "Block.h"
#include "Cylinder.h"
#include "Branches.h"
#include <time.h>
#include <stdlib.h>
#include "ControllerSubclass.h"
#include <iostream>
#include "Sphere.h"
using namespace std;

void set3DViewingInformation(double xyz[6])
{
	ModelView::setMCRegionOfInterest(xyz);


// Set values for eye-center-up to produce a reasonable off-axis
// view of your scene, then:

double xmid = 0.5 * (xyz[0] + xyz[1]);
double ymid = 0.5 * (xyz[2] + xyz[3]);
double zmid = 0.5 * (xyz[4] + xyz[5]);

cryph::AffPoint center(xmid, ymid, zmid);

double maxDelta = xyz[1] - xyz[0];
double delta 	= xyz[3] - xyz[2];

if(delta > maxDelta)
	maxDelta = delta;
delta = xyz[5] - xyz[4];
if(delta > maxDelta)
	maxDelta = delta;

float width = xyz[1] - xyz[0];
float height = xyz[3] - xyz[2];
float depth = xyz[5] - xyz[4];

float pd = sqrt(height * height + width * width);
float d = sqrt(pd * pd + depth * depth);

float r = d / 2;

double distEyeCenter = 2.0 * d;

cryph::AffPoint eye(xmid, ymid, zmid + distEyeCenter);



cryph::AffVector up = cryph::AffVector::yu;

// Notify the ModelView of our MC->EC viewing requests:
ModelView::setEyeCenterUp(eye, center, up);

double ecZpp, ecZmin, ecZmax;
double zpp, zmin, zmax;
zmin = - (distEyeCenter + r);
zmax = zmin + d;
zpp = zmax;

ecZpp 	= -(distEyeCenter - 0.5 * maxDelta);
ecZmin 	= ecZpp - maxDelta * 2.0;
ecZmax 	= ecZpp + 0.5 * maxDelta;

// Set values for ecZpp, ecZmin, ecZmax that make sense in the context of
// the EC system established above, then:

ModelView::setProjectionPlaneZ(zpp);
ModelView::setECZminZmax(zmin, zmax);
}

int main(int argc, char* argv[])
{
	ControllerSubclass c("Volcano, rain, lava and cloud under a moon(night scene)", MVC_USE_DEPTH_BIT);
	c.reportVersions(std::cout);

	// create your scene, adding things to the Controller....\

	//ground
	//vec3 tColor = { 124.0 / 255.0, 108.0 / 255.0, 109.0 / 255.0 };
	vec3 color = {78.0 / 255.0, 39.0 / 255.0, 40.0 / 255.0};

	cryph::AffPoint position(0.0, -33.0, 0.0);
	cryph::AffVector direction(0.0, 0.0, 0.0);
	vec3 length = {300, 68, 300};
	vec3 degrees = {10, 10, 10};
	c.addModel(new Block(position, direction, length, degrees, color));


	vec3 location = {0, 0, 0};
	vec3 color2 = {245.0 / 255.0, 146.0 / 255.0, 24.0 / 255.0};

	//create Moon
	vec3 m_loc = {-100, 200, 0};
	float m_rad = 20;
	vec3 m_color = {255.0 / 255.0, 255.0 / 255.0, 102.0 / 255.0};
	c.addModel(new Sphere(m_loc, m_rad, m_color));

	//VOCANO
	float radius = 20;
	float height = 10;
	for(int i = 0; i < 150; i++)
	{
		radius = radius - 0.1;
		height = height + 0.5;
		c.addModel(new Cylinder(location, radius, height, color));
	}


	vec3 location1 = {0, 0, 0};
	vec3 color7 = {255.0 / 255.0, 0.0 / 255.0, 2.0 / 255.0};
	vec3 color6 = {247.0 / 255.0, 49.0 / 255.0, 5.0 / 255.0};
	c.addModel(new Branches(location1, 4.5, 125, color6));
	c.addModel(new Cylinder(location1, 4.8, 90, color2));

	//rain
	srand(time(NULL));
	vec3 length1 = {0.5, 5, 0.5};
	vec3 position1 = {5, 0, 5};
	vec3 color3 = {3.0 / 255.0, 74.0 / 255.0, 236.0 / 255.0};
	for(int i = 0 ; i < 300; i++)
	{
		position1[0] = (rand() % 300) - 150;
		position1[2] = (rand() % 300) - 150;
		position1[1] = (rand() % 150);
		c.addModel(new Block(position1, direction, length1, degrees, color3));
	}

	//hot lava
	vec3 length2 = {5, 5, 5};
	vec3 position3 = {5, 0, 5};
	for(int i = 0 ; i < 1000; i++)
	{
		position3[0] = (rand() % 150) ;
		position3[2] = (rand() % 150) ;
		c.addModel(new Block(position3, direction, length2, degrees, color7));
	}


	//smokes
	vec3 color4 = {77.0 / 255.0, 78.0 / 255.0, 89.0 / 255.0};
	vec3 location2 = {0, 90, 0};
	for(int i = 0; i < 5; i++)
	{
		c.addModel(new Cylinder(location2, 10+i*5, 1, color4));
		location2[1] = location2[1] + 10;
	}

	//cloud
	vec3 c_color = {255.0 / 255.0, 255.0 / 255.0, 255.0 / 255.0};
	vec3 c_loc = {-200, 190, 0};
	float c_rad = 5;
	for(int i = 0; i <200; i++)
	{
		c_loc[0] = (rand() % 100) -100;
		c_loc[2] = (rand() % 60) - 20;
		c.addModel(new Sphere(c_loc, c_rad, c_color));
	}

	glClearColor(0.1, 0.1, 0.1, 0.1);

	double xyz[6];
	c.getOverallMCBoundingBox(xyz);
	set3DViewingInformation(xyz);

	c.run();

	return 0;
}
