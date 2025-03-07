#version 410

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;
in vec3 vTangent;
in vec3 vBiTangent;

uniform vec3 CameraPosition;

uniform vec3 AmbientColour;
uniform vec3 LightColour;
uniform vec3 LightDirection;

const int MAX_LIGHTS = 4;
uniform int numLights;
uniform vec3 PointLightColour[MAX_LIGHTS];
uniform vec3 PointLightPosition[MAX_LIGHTS];

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float specularPower;

uniform sampler2D diffuseTex;
uniform sampler2D specularTex;
uniform sampler2D normalTex;

out vec4 FragColour;

vec3 GetDiffuse(vec3 direction, vec3 colour, vec3 normal)
{
	return colour * max( 0, dot( normal, -direction));
}

vec3 GetSpecular(vec3 direction, vec3 colour, vec3 normal, vec3 view)
{
	vec3 R = reflect( direction, normal);
	float specularTerm = pow( max( 0, dot( R, normal )), specularPower);
	return specularTerm * colour;
}

void main()
{
	vec3 N = normalize(vNormal);
	vec3 L = normalize(LightDirection);
	vec3 T = normalize(vTangent);
	vec3 B = normalize(vBiTangent);

	vec3 texDiffuse = texture(diffuseTex, vTexCoords).rgb;
	vec3 texSpecular = texture(specularTex, vTexCoords).rgb;
	vec3 texNormal = texture(normalTex, vTexCoords).rgb;

	mat3 TBN = mat3(T, B, N);

	N = TBN * (texNormal * 2 - 1);

	vec3 diffuseTotal = GetDiffuse(L, LightColour, N);

	vec3 V = normalize(CameraPosition - vPosition.xyz);
	
	vec3 specularTotal = GetSpecular(L, LightColour, N, V);

	for(int i = 0; i < numLights; i++)
	{
		vec3 direction = vPosition.xyz - PointLightPosition[i];
		float distance = length(direction);
		direction = direction / distance;

		vec3 colour = PointLightColour[i]/(distance * distance);

		diffuseTotal += GetDiffuse(direction, colour, N);
		specularTotal += GetSpecular(direction, colour, N, V);
	}
	
	
	vec3 ambient = AmbientColour * Ka * texDiffuse;
	vec3 diffuse = Kd * texDiffuse * diffuseTotal;
	vec3 specular = Ks * texSpecular * specularTotal;

	FragColour = vec4(ambient + diffuse + specular, 1);
}