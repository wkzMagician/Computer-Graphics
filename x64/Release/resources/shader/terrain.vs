/*
    Copyright 2022 Etay Meiri

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#version 330

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 InTex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float gMinHeight;
uniform float gMaxHeight;

out vec4 Color;
out float Height;

out vec2 Tex;
out vec3 FragPos;
out vec3 Normal;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    
    gl_Position = projection * view * vec4(FragPos, 1.0);

    Tex = InTex;


    float DeltaHeight = gMaxHeight - gMinHeight;
    float HeightRatio = (aPos.y - gMinHeight) / DeltaHeight;
    float c = HeightRatio * 0.8 + 0.2;
    Color = vec4(c, c, c, 1.0);

    Height = HeightRatio;
}
