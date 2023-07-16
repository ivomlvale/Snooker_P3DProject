#version 440 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

uniform sampler2D ourTexture;

//verificar se estamos a mexer na mesa ou nas bolas
uniform bool is_table;

//verificar se a fonte esta ou nao ativada
uniform bool ambient_active;
uniform bool directional_active;
uniform bool point_active;
uniform bool spot_active;

//posiçao da camera
uniform vec3 viewPos;

//posiçao da luz
uniform vec3 lightPos;

//cor da luz
uniform vec3 light_color;

//atenuacoes
uniform float constant;
uniform float linear;
uniform float quadratic;

//angulos de recorte
uniform float spotCutoff;
uniform float spotOuterCutoff;

//inicializar funcoes
void CalcAmbientLight();
void CalcDirectionalLight();
void CalcPointLight();
void CalcSpotlight();

void main()
{
    //Se for a mesa, atribui a cor verde
	if(is_table == true){
		FragColor =  vec4(vec3(0.0f, 1.0f, 0.0f ) * 0.2 , 1.0f);
	}
    //Se nao for calcula as luzes
	else{
        //Verificar se a fonte de luz ambiente está ativa para a calcular
		if(ambient_active == true)
		{
			CalcAmbientLight();
		}
        //Verificar se a fonte de luz direcional está ativa para a calcular
		if(directional_active == true)
		{
			CalcDirectionalLight();
		}
        //Verificar se a luz pontual está ativa para a calcular
		if(point_active == true)
		{
			CalcPointLight();
		}
        //Verificar se a fonte de luz conica está ativa para a calcular
		if(spot_active == true)
		{
			CalcSpotlight();
		}
        //se nenhuma estiver ativada, deixamos a textura sem nenhum tipo de fonte de luz
		if(!ambient_active && !directional_active && !point_active && !spot_active )
		{
			FragColor = texture(ourTexture, TexCoord);
		}

	}
}

//Fonte de luz Mbiente
void CalcAmbientLight(){
	FragColor = vec4(light_color * material.ambient , 1.0f) * texture(ourTexture, TexCoord);
}

//Fonte de luz Direcional
void CalcDirectionalLight(){
	vec3 lightDir = normalize(lightPos - FragPos);
    vec3 normalN = normalize(Normal);

	//difusa
	float diff = max(dot(normalN, lightDir), 0.0);

	//especular
	vec3 reflectDir = reflect(-lightDir, normalN);
	vec3 viewDir = normalize(viewPos - FragPos);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	//resultados
	vec3 ambient  = material.ambient * light_color; 
    vec3 diffuse  = material.diffuse  * diff * light_color;
    vec3 specular = material.specular * spec * light_color;
    
	vec3 result = (ambient + diffuse + specular);

	FragColor = vec4(result, 1.0) * texture(ourTexture, TexCoord);
}

//Fonte de luz Pontual
void CalcPointLight(){
	vec3 lightDir = normalize(lightPos - FragPos);
    vec3 normalN = normalize(Normal);

    //difusa
	float diff = max(dot(normalN, lightDir), 0.0);

    //especular
	vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normalN);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    //atenuaçoes
    float distance = length(lightPos - FragPos);
    float attenuation = 1.0 / (constant + linear * distance  + quadratic * (distance * distance));    

    //resultados 
    vec3 ambient  = material.ambient  * light_color;
    vec3 diffuse  = material.diffuse  * diff * light_color;
    vec3 specular = material.specular * spec * light_color;

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    vec3 result = (ambient + diffuse + specular);
	
	
	
	FragColor = vec4(result, 1.0) * texture(ourTexture, TexCoord);
}

//Fonte de luz Cónica
void CalcSpotlight(){
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 normalN = normalize(Normal);

    //difusa
    float diff = max(dot(normalN, lightDir), 0.0);

    //especular
    vec3 reflectDir = reflect(-lightDir, normalN);
    vec3 viewDir = normalize(viewPos - FragPos);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    //calcular angulo de recorte
    float theta = dot(lightDir, normalize(vec3(0.9f, 0.2f, 0.4f)));


    float attenuation;
    float distance;

    //calculamos a luz tendo em conta o angulo de recorte
    if(theta > spotCutoff) 
    {
        //atenuaçao
        distance = length(lightPos - FragPos);
        attenuation = 1.0 / (constant + linear * distance  + quadratic * (distance * distance));

        //resultados
        vec3 ambient  = material.ambient  * light_color;
        vec3 diffuse  = material.diffuse  * diff * light_color;
        vec3 specular = material.specular * spec * light_color;
        ambient  *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;

        vec3 result = (ambient + diffuse + specular);

        FragColor = vec4(result, 1.0) * texture(ourTexture, TexCoord);
    }
    
    //Suavizar o angulo de recorte
    else if(theta < spotCutoff && theta > spotOuterCutoff)
	{
		float epsilon = spotCutoff - spotOuterCutoff;
		float intensity = clamp((theta - spotOuterCutoff) / epsilon, 0.0, 1.0);    
		diff *= intensity;
		spec *= intensity;

         //atenuaçao
        distance = length(lightPos - FragPos);
        attenuation = 1.0 / (constant + linear * distance  + quadratic * (distance * distance));

        //resultados
        vec3 ambient  = material.ambient  * light_color;
        vec3 diffuse  = material.diffuse  * diff * light_color;
        vec3 specular = material.specular * spec * light_color;
        ambient  *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;

        vec3 result = (ambient + diffuse + specular);

        FragColor = vec4(result, 1.0) * texture(ourTexture, TexCoord);
	}
    
    //Se estiver fora da spotlight, dámos uma percentagem da luz ambiente para nao ficar tudo escuro 
    else
    {
         FragColor = vec4(vec3(0.2f, 0.2f, 0.2f ) * material.ambient , 0.2f) * texture(ourTexture, TexCoord);
    }
}
