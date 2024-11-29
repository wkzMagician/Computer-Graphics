#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform bool useTex;
uniform vec4 Ka;    // 环境光反射系数
uniform vec4 Kd;    // 漫反射系数
uniform vec4 Ks;    // 镜面反射系数
uniform float Ns;   // 高光系数

uniform vec3 lightPos;
uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;
uniform vec3 viewPos;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_opacity1;

uniform bool isLight;

void main()
{  
   vec4 texColor = texture(texture_diffuse1, TexCoords);
   vec3 color = texColor.rgb;

   vec3 norm = normalize(Normal);
   vec3 lightDir = normalize(lightPos - FragPos);
   float diff = max(dot(norm, lightDir), 0.0);
 
   vec3 viewDir = normalize(viewPos - FragPos);
   vec3 reflectDir = reflect(-lightDir, norm);  
   float spec = pow(max(dot(viewDir, reflectDir), 0.0), Ns);
   float tex_spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
   
    vec3 ambient = vec3(1.0);
    vec3 diffuse = vec3(1.0); 
    vec3 specular = vec3(1.0);
    vec3 result = vec3(1.0);

    bool isRedBall = Kd == vec4(0.517331, 0.007343, 0.000000, 1.0);
    bool isGold = Kd == vec4(0.496920 ,0.471582, 0.114475, 1.0);

   if(useTex)
   {
        ambient = lightAmbient * color;
        diffuse = diff * color;
        specular = tex_spec * Ks.rgb;
        result = ambient + diffuse + specular;

        float alpha = texture2D(texture_diffuse1, TexCoords).a;

        if(alpha < 0.2f)
            discard;
        gl_FragColor = vec4(result, 1.0);
   }
   else
   {
        ambient = lightAmbient * Kd.rgb; // 环境光
        diffuse = diff * Kd.rgb; // 漫反射光
        specular = spec * Ks.rgb; // 镜面反色红光
        result = ambient + diffuse + specular;
        if((isRedBall || isGold) && isLight)
        {
          //   ambient = mix(vec3(1.0), ambient, 0.8);  
          ambient = mix(vec3(1.0, 140.0 / 255.0, 0.0), ambient, 0.8);
          ambient =  mix(vec3(1.0), ambient, 0.9);
            result = ambient * 2.0 + diffuse + specular * 2.0;
        }
        gl_FragColor = vec4(result, 1.0f);
   }
    
}