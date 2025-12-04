#version 330 core

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};

struct Light {
    vec3 direction;
    vec3 position;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    // attenuation constants
    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;

in vec3 FragPos;
in vec2 TexCoords;
in vec3 Normal;

uniform Light light;
uniform Material material;
uniform float time;
uniform bool useEmission;

void main()
{
    // light source direction
    // vec3 lightDir = normalize(-light.direction);
    vec3 lightDir = normalize(light.position - FragPos);
    float distance = length(light.position - FragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

    // ambient
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
    // ambient *= attenuation;
     // diffuse 
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
    diffuse *= attenuation;
    
    // emmission
    vec3 emission = vec3(0.0);
    if (useEmission && texture(material.specular, TexCoords).r == 0.0) {
        emission = texture(material.emission, TexCoords).rgb;

        /*some extra fun stuff with "time uniform" */
        emission = texture(material.emission, TexCoords + vec2(0.0,time)).rgb;   /*moving */
        emission = emission * (sin(time) * 0.5 + 0.5) * 2.0; 
    }

    // specular
    vec3 viewDir = normalize(-FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));
    specular *= attenuation;
    
    vec3 result = (ambient + diffuse + specular + emission);
    FragColor = vec4(result, 1.0);
}