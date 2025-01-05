#version 430 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  

layout(std430, binding = 0) buffer SamplePoints {
    vec3 points[]; // SSBO 中存储的采样点数据
};
  
uniform vec3 viewPos; 

struct Material {
    vec3 albedo;
    float metallic;
    float roughness;
    float ao;
};

uniform Material material;

uniform vec3 lightColor;

const float PI = 3.14159265359;

// 函数声明
vec3 fresnelSchlick(float cosTheta, vec3 F0);
float distributionGGX(vec3 N, vec3 H, float roughness);
float geometrySchlickGGX(float NdotV, float roughness);
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness);

void main()
{
    vec3 N = normalize(Normal); // 归一化法线
    vec3 V = normalize(viewPos - FragPos); // 视线方向

    vec3 F0 = vec3(0.04); // 默认反射率
    F0 = mix(F0, material.albedo, material.metallic); // 计算反射率
   
    vec3 Lo = vec3(0.0); // 输出颜色
    for(int i = 0; i < points.length(); i++)
    {
        vec3 L = normalize(points[i] - FragPos); // 光线方向
        vec3 H = normalize(V + L); // 半角向量

        float distance = length(points[i] - FragPos);
        float attenuation = 1.0 / (distance * distance); // 线性空间：1/d^2
        vec3 radiance = lightColor * attenuation; // 辐照度
    
        float NDF = distributionGGX(N, H, material.roughness); // 法线分布函数
        float G = geometrySmith(N, V, L, material.roughness); // 几何遮蔽函数
        vec3 F = fresnelSchlick(max(dot(N, V), 0.0), F0); // 计算菲涅尔项

        vec3 kS = F; // 镜面反射的贡献
        vec3 kD = vec3(1.0) - kS; // 漫反射的贡献
        kD *= (1.0 - material.metallic); // 漫反射的贡献

        vec3 nominator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 防止除零
        vec3 specular = nominator / denominator;

        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * material.albedo / PI + specular) * radiance * NdotL; // 出射光线的辐射率
    }

    vec3 ambient = vec3(0.03) * material.albedo * material.ao; // 环境光
    vec3 color = ambient + Lo; // 最终颜色

    // HDR 色调映射
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));

    FragColor = vec4(color, 1.0);
}

// Fresnel 方程
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

// GGX 法线分布函数
float distributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return num / denom;
}

// 几何遮蔽函数（Schlick-GGX 单向）
float geometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return num / denom;
}

// 几何遮蔽函数（Smith 模型）
float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = geometrySchlickGGX(NdotV, roughness);
    float ggx1 = geometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}