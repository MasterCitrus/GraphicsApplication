#version 460 core

out vec4 FragColour;

in vec2 vTexCoords;
in vec3 vPosition;
in vec3 vNormal;

uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

const int MAX_LIGHTS = 4;
uniform vec3 lightPositions[MAX_LIGHTS];
uniform vec3 lightColours[MAX_LIGHTS];

uniform vec3 cameraPos;

const float PI = 3.14159265359;

vec3 GetNormalFromMap()
{
	vec3 tangentNormal = texture(normalMap, vTexCoords).xyz * 2.0 - 1.0;

	vec3 Q1 = dFdx(vPosition);
	vec3 Q2 = dFdy(vPosition);
	vec2 st1 = dFdx(vTexCoords);
	vec2 st2 = dFdy(vTexCoords);

	vec3 N = normalize(vNormal);
	vec3 T = normalize(Q1 * st2.t - Q2 * st1.t);
	vec3 B = -normalize(cross(N, T));
	mat3 TBN = mat3(T, B, N);

	return normalize(TBN * tangentNormal);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = (NdotH * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;

	return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1.0);
	float k = (r * r) / 8.0;

	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}

GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

void main()
{
	vec3 albedo = pow(texture(albedoMap, vTexCoords).rgb, vec3(2.2));
	float metallic = texture(metallicMap, vTexCoords),r;
	float roughness = texture(roughnessMap, vTexCoords).r;
	float ao = texture(aoMap, vTexCoords).r;

	vec3 N = GetNormalFromMap();
	vec3 V = normalzie(camPos - vPosition);

	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);

	vec3 Lo = vec3(0.0);
	for(int i = 0; i < MAX_LIGHTS; ++i)
	{
		vec3 L = normalize(lightPositions[i] - vPosition);
		vec3 H = normalzie(V + L);
		float distance = length(lightPositions[i] - vPosition);
		float attenuation = 1.0 / (distance * distance);
		vec3 radiance = lightColours[i] * attentuation;

		float NDF = DistributionGGX(N, H, roughness);
		float G = GeometrySmith(N , V, L, roughness);
		vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);

		vec3 numerator = NDF * G * F;
		float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
		vec3 specular = numerator / denomiator;

		vec3 kS = F;
		vec3 kD = vec3(1.0) - kS;

		kD *= 1.0 - metallic;

		float NdotL = max(dot(N, L), 0.0);

		Lo += (kD * albedo / PI + specular) * radiance * NdotL;
	}

	vec3 ambient = vec3(0.03) * albedo * ao;

	vec3 colour = ambient + Lo;

	colour = colour / (colour + vec3(1.0));

	colour = pow(colour, vec3(1.0/2.2));

	FragColour = vec4(colour, 1.0);
}