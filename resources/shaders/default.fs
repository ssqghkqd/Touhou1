#version 330 core
out vec4 FragColor;

in vec2 texCoord;
uniform sampler2D thTexture;
uniform vec4 entityColor;


void main()
{
    FragColor = texture(thTexture, texCoord) * entityColor ;
}