#version 410 core

uniform mat4 mc_ec =   // (dynamic rotations) * (ViewOrientation(E,C,up))
	mat4(1.0, 0.0, 0.0, 0.0, // initialize to identity matrix
	     0.0, 1.0, 0.0, 0.0, // ==> no dynamic rotations -AND- MC = EC
	     0.0, 0.0, 1.0, 0.0,
	     0.0, 0.0, 0.0, 1.0);
uniform mat4 ec_lds = // (W-V map) * (projection matrix)
	mat4(1.0, 0.0, 0.0, 0.0, // initialize to (almost) identity matrix
	     0.0, 1.0, 0.0, 0.0, // ==> ORTHOGONAL projection -AND- EC = LDS
	     0.0, 0.0, -1.0, 0.0,
	     0.0, 0.0, 0.0, 1.0);

//initialize all the variables
const int MAX_NUM_LIGHTS = 3;
uniform int actualNumLights = 3;
uniform float m = 10;
uniform vec3 ka = vec3(0.0, 0.0, 0.0);
uniform vec3 kd = vec3(0.0, 0.0, 0.0);
uniform vec3 ks = vec3(0.0, 0.0, 0.0);
uniform vec3 lightStrength[MAX_NUM_LIGHTS];
uniform vec4 p_ecLightPos[MAX_NUM_LIGHTS];
uniform vec3 globalAmbient = vec3(0.0, 0.0, 0.0);

layout (location = 0) in vec3 mcPosition;
in vec3 mcNormal; // incoming normal vector in model coordinates
out vec3 colorToFS;

float atten(vec3 i, vec3 Q)
{
	vec3 d = i - Q;
	float length = sqrt(d.x*d.x + d.y*d.y + d.z*d.z);
	return 1/(length+1);
}

vec3 evaluateLightingModel(in vec3 ec_Q, in vec3 ec_nHat)
{
	vec3 accumDiffusion = vec3(0.0, 0.0, 0.0);
	vec3 accumSpec = vec3(0.0, 0.0, 0.0);
	vec3 ambientPart = ka * globalAmbient;
	// Create a unit vector towards the viewer (method depends on type of projection!)
  // NOTE: GLSL matrices are indexed as M[col][row]!!!
	vec3 vHat;		//unit vector to the eye
	if(ec_lds[2][3] == 0)			//ORTHOGONAL
		vHat = vec3(0.0, 0.0, 1.0);
	else
		vHat = -normalize(ec_Q);

	if(dot(ec_nHat, vHat) < 0)
	{
		ec_nHat = -1.0 * ec_nHat;
	}

  for (int i=0 ; i<actualNumLights ; i++)
  {
			// If light sources defined in MC, transform them to EC:
			vec4 lightposition;
			if(i == 0 || i == 1)
			{
				lightposition = mc_ec * p_ecLightPos[i];
			}
			if(i == 2)
			{
				lightposition = p_ecLightPos[i];
			}

			vec3 liHat = lightposition.xyz;		//unit vector to the light source i
			if(lightposition.w == 1.0)
				liHat = normalize(liHat - ec_Q);
			else
				liHat = normalize(liHat);
			//check if the light source is behind the object
			float d = atten(lightposition.xyz, ec_Q);
			float attenuation = 1/(1 + 0.001 * d);
			if(dot(liHat, ec_nHat) > 0)
			{
				accumDiffusion += kd * dot(liHat, ec_nHat) * attenuation * lightStrength[i];
			}
			vec3 riHat = normalize(reflect(-liHat, ec_nHat));
			if(dot(riHat, vHat) > 0)
			{
				accumSpec += ks * pow(dot(riHat, vHat), m) * attenuation * lightStrength[i];
			}
  }
	return (ambientPart + accumDiffusion + accumSpec);
}

void main ()
{
	// convert current vertex and its associated normal to eye coordinates
	vec4 p_ecPosition = mc_ec * vec4(mcPosition, 1.0);
	mat3 normalMatrix = transpose( inverse( mat3x3(mc_ec) ) );
	vec3 ec_nHat = normalize(normalMatrix * mcNormal);
	colorToFS = evaluateLightingModel(p_ecPosition.xyz, ec_nHat);
	// apply the projection matrix to compute the projective space representation
	// of the 3D logical device space coordinates of the input vertex:
	gl_Position = ec_lds * p_ecPosition;
}
