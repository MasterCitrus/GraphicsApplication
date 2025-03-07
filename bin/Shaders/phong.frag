#version 410

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;

uniform vec3 cameraPosition;

uniform vec3 AmbientColour;
uniform vec3 LightColour;
uniform vec3 LightDirection;

uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float specularPower;

uniform sampler2D diffuseTex;

out vec4 FragColour;

void main()
{
	vec3 N = normalize(vNormal);
	vec3 L = normalize(LightDirection);

	float lambertTerm = max(0, min( 1, dot(N, -L)));

	vec3 V = normalize(cameraPosition - vPosition.xyz);
	vec3 R = reflect( L, N );

	float specularTerm = pow( max( 0, dot( R, V )), specularPower);

	vec3 textureColour = texture(diffuseTex, vTexCoords).rgb;

	vec3 diffuse = LightColour * Kd * lambertTerm * textureColour;
	vec3 ambient = AmbientColour * Ka * textureColour;
	vec3 specular = LightColour * Ks * specularTerm;

	FragColour = vec4(ambient + diffuse + specular, 1);
}