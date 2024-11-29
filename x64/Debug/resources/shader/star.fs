#version 330 core
out vec4 FragColor;

void main()
{
    vec4 white = vec4(1.0);
    vec4 red = vec4(1.0, 0.0, 0.0, 1.0);
    FragColor = mix(white, red, 0.8);
}