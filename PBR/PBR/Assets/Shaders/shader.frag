#version 460 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 WorldPos;
in vec3 Normal;

uniform bool textured;

// material parameters
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

uniform vec3 a;
uniform float m;
uniform float r;
uniform float ambientOcclusion;
uniform float lightIntensity;

// lights
uniform vec3 lightPositions;
uniform vec3 lightColors;

uniform vec3 camPos;

const float PI = 3.14159265359;

vec3 getNormalFromMap()
{
    vec3 tangentNormal = texture(normalMap, TexCoords).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = Normal;
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

vec3 Radiance(vec3 lightDir)
{
    float distance = length(lightDir);
    float attenuation = 1.0 / (distance * distance);
    vec3 radiance = lightColors * attenuation * lightIntensity;

    return radiance;
}

float Distribution(vec3 halfwayVec, float roughness, vec3 normal)
{
    float a = pow(roughness, 2);
    float a2 = pow(a, 2);
    float NdotH = max(dot(normal, halfwayVec), 0.0);
    float NdotH2 = pow(NdotH, 2);

    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * pow(denom, 2);

    return num / denom;
}

float GSchlickGGX(float NdotCamDir, float roughness)
{
    float r = (roughness + 1.0);
    float k = pow(r, 2) / 8.0;

    float num   = NdotCamDir;
    float denom = NdotCamDir * (1.0 - k) + k;

    return num / denom;
}

float Geometry(vec3 camDir, vec3 lightDir, float roughness, vec3 normal)
{
    float NdotCamDir = max(dot(normal, camDir), 0.0);
    float NdotLightDir = max(dot(normal, lightDir), 0.0);
    float ggx2 = GSchlickGGX(NdotCamDir, roughness);
    float ggx1 = GSchlickGGX(NdotLightDir, roughness);

    return ggx1 * ggx2;
}

vec3 Fresnel(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

vec3 BRDF(float D, float G, vec3 F, vec3 camDir, vec3 lightDir, vec3 albedo, float metallic, vec3 normal)
{
    vec3 numerator    = D * G * F; 
    float denominator = 4.0 * max(dot(normal, camDir), 0.0) * max(dot(normal, lightDir), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;
        
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;	  

    return (kD * albedo / PI + specular);
}

void main()
{
    vec3 albedo;
    vec3 normal;
    float metallic;
    float roughness;
    float ao;

    if (textured)
    {
        albedo = pow(texture(albedoMap, TexCoords).rgb, vec3(2.2));
        metallic = texture(metallicMap, TexCoords).r;
        roughness = texture(roughnessMap, TexCoords).r;
        ao = texture(aoMap, TexCoords).r;
        normal = Normal;
    }
    else
    {
        albedo = a;
        normal = Normal;
        metallic = m;
        roughness = r;
        ao = ambientOcclusion;
    }
    
    vec3 camDir = normalize(camPos - WorldPos);

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);
    for(int i = 0; i < 1; ++i) 
    {
        vec3 lightDir = normalize(lightPositions - WorldPos);
        vec3 halfwayVec = normalize(camDir + lightDir);
        
        vec3 radiance = Radiance(lightDir);

        float D   = Distribution(halfwayVec, roughness, normal);   
        float G   = Geometry(camDir, lightDir, roughness, normal);      
        vec3 F    = Fresnel(max(dot(halfwayVec, camDir), 0.0), F0);
           
        vec3 brdf = BRDF(D, G, F, camDir, lightDir, albedo, metallic, normal);

        float cosTheta = max(dot(normal, lightDir), 0.0);        

        Lo += brdf * radiance * cosTheta;
    }   
    
    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));
    
    FragColor = vec4(color, 1.0);
}