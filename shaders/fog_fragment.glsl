#version 330 core
in vec3 fragPos;

out vec4 FragColor;

uniform vec3 cameraPos;
uniform vec3 fogColor;
uniform float fogDensity;
uniform float fogHeight;
uniform float scatteringCoefficient;

void main()
{
    float distanceToCamera = length(cameraPos - fragPos);
    float fogFactor = clamp((fragPos.y - fogHeight) * fogDensity, 0.0, 1.0);
    float fogIntensity = exp(-distanceToCamera * scatteringCoefficient * fogFactor);

    vec4 sceneColor = vec4(1.0);
    //FragColor = mix(vec4(fogColor, 1.0), sceneColor, fogIntensity);
	FragColor = vec4(1.0, 0.0, 0.0, 1.0);  // Output a solid red color
}
