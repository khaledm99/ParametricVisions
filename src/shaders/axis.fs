#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec3 FragPos;

uniform vec3 viewPos;
void main()
{

    FragColor = vec4(ourColor, 1.0);
};
