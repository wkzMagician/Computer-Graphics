#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform sampler2D gTextureHeight0;

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
     FragColor = vec4(result, 1.0);

     // 应用纹理
     // vec4 texColor = texture(gTextureHeight0, TexCoords);
     // FragColor = FragColor * texColor;
}