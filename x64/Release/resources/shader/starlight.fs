#version 330 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform vec4 shaderColor;
uniform float mixRatio;

uniform bool use2Light;

struct PointLight {
    vec3 position;

    vec3 lightColor;

    float constant;
    float linear;
    float quadratic;
};  
uniform PointLight pointLight;

void main()
{
    // ������
    float ambientStrength = 0.5;
    vec3 ambient = ambientStrength * lightColor;
  	
    // ������ 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // ���淴��
    float specularStrength = 0.7;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
        
    vec3 result = (ambient + diffuse + specular) * objectColor;
    vec3 r = mix(result, vec3(1.0f), mixRatio);
    
    if(!use2Light) {
        FragColor = vec4(r, 1.0f);
        return;
    }
    
    // 叠加上第二个光源
    vec3 lightDir2 = normalize(pointLight.position - FragPos);
    float diff2 = max(dot(norm, lightDir2), 0.0);
    
    // 光强衰减
    float distance = length(pointLight.position - FragPos);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));

    vec3 diffuse2 = diff2 * pointLight.lightColor * attenuation;
    vec3 reflectDir2 = reflect(-lightDir2, norm);
    float spec2 = pow(max(dot(viewDir, reflectDir2), 0.0), 32);
    vec3 specular2 = specularStrength * spec2 * pointLight.lightColor * attenuation;

    vec3 result2 = (ambient + diffuse2 + specular2) * objectColor;

    // vec3 r2 = mix(result2, vec3(1.0f), mixRatio);
    vec3 r2 = result2;
    FragColor = vec4(0.5f * r + 0.5f * r2, 1.0f);
} 