#version 430 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  

layout(std430, binding = 0) buffer SamplePoints {
    vec3 points[]; // SSBO 中存储的采样点数据
};
  
uniform vec3 viewPos; 

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

uniform Material material;

struct Light {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

float constant = 1.0f;
float linear = 0.09f;
float quadratic = 0.032f;

void main()
{
    vec3 color = vec3(0.0);

    // 遍历 SSBO 中的采样点，计算光照
    for (int i = 0; i < points.length(); ++i) {
        vec3 lightDir = normalize(points[i] - FragPos); // 光线方向
        float diff = max(dot(Normal, lightDir), 0.0);        // 漫反射系数

        vec3 viewDir = normalize(viewPos - FragPos); // 视线方向
        vec3 reflectDir = reflect(-lightDir, Normal); // 镜面反射方向
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess); // 镜面反射系数

        vec3 ambient = light.ambient * material.ambient; // 环境光
        vec3 diffuse = diff * light.diffuse * material.diffuse; // 漫反射
        vec3 specular = spec * light.specular * material.specular; // 镜面反射

        float distance = length(points[i] - FragPos);
        float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance)); // 衰减

        color += (ambient + diffuse + specular) * attenuation;
    }

    color = color / float(points.length()) * 2.5f; // 除以采样点数量，乘以 5.0 以增加亮度

    FragColor = vec4(color, 1.0);
} 