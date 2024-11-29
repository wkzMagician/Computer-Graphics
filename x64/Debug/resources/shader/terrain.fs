#version 330

layout(location = 0) out vec4 FragColor;

in vec4 Color;
in float Height;

in vec2 Tex;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D gTextureHeight0;
uniform sampler2D gTextureHeight1;
uniform sampler2D gTextureHeight2;
uniform sampler2D gTextureHeight3;

uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
// uniform vec3 objectColor;

float gHeight0 = 0.2;
float gHeight1 = 0.35;
float gHeight2 = 0.5;
float gHeight3 = 0.75;

vec4 CalcTexColor()
{
    vec4 TexColor;

    if (Height < gHeight0) {
       TexColor = texture(gTextureHeight0, Tex);
    } else if (Height < gHeight1) {
       vec4 Color0 = texture(gTextureHeight0, Tex);
       vec4 Color1 = texture(gTextureHeight1, Tex);
       float Delta = gHeight1 - gHeight0;
       float Factor = (Height - gHeight0) / Delta;
       TexColor = mix(Color0, Color1, Factor);
    } else if (Height < gHeight2) {
       vec4 Color0 = texture(gTextureHeight1, Tex);
       vec4 Color1 = texture(gTextureHeight2, Tex);
       float Delta = gHeight2 - gHeight1;
       float Factor = (Height - gHeight1) / Delta;
       TexColor = mix(Color0, Color1, Factor);
    } else if (Height < gHeight3) {
       vec4 Color0 = texture(gTextureHeight2, Tex);
       vec4 Color1 = texture(gTextureHeight3, Tex);
       float Delta = gHeight3 - gHeight2;
       float Factor = (Height - gHeight2) / Delta;
       TexColor = mix(Color0, Color1, Factor);
    } else {
       TexColor = texture(gTextureHeight3, Tex);
    }

    return TexColor;
}


void main()
{
    vec4 TexColor = CalcTexColor();

    vec4 tmpColor = Color * TexColor;



   //  // ������
   //  float ambientStrength = 0.5;
   //  vec3 ambient = ambientStrength * lightColor;
  	
   //  // ������ 
   //  vec3 norm = normalize(Normal);
   //  vec3 lightDir = normalize(lightPos - FragPos);
   //  float diff = max(dot(norm, lightDir), 0.0);
   //  vec3 diffuse = diff * lightColor;
    
   //  // ���淴��
   //  float specularStrength = 0.7;
   //  vec3 viewDir = normalize(viewPos - FragPos);
   //  vec3 reflectDir = reflect(-lightDir, norm);  
   //  float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   //  vec3 specular = specularStrength * spec * lightColor;  
        
   //  vec3 result = (ambient + diffuse + specular) * vec3(tmpColor);
   //  FragColor = vec4(result, 1.0);

   FragColor = tmpColor;
}
