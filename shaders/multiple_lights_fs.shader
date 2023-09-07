#version 300 es

precision highp float;

struct TexMaterial
{
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

struct Material
{
    vec3 diffuse;
    vec3 specular;
    float shininess;
};


struct DirLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


struct PointLight
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


struct SpotLight
{
    vec3 position;
    vec3 direction;
    float cutoff;
    float outer_cutoff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};



in vec3 vFragPos;
in vec3 vNormal;

out vec4 vFragColor;

uniform vec3 vFogColor;
uniform Material sMaterial;
uniform vec3 vViewPos;
uniform DirLight sDirLight;
uniform SpotLight sSpotLight;
uniform PointLight sPlayerLight;
uniform PointLight sKeyLight;
uniform PointLight sInPortalLight;
uniform PointLight sOutPortalLight;
uniform bool bKeyOn;
uniform bool bInPortalOn;
uniform bool bOutPortalOn;
uniform float fBrightness;
uniform float fPlayerLightScalar;


float RadialFalloff(vec3 vPos, vec3 vFragPos);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 vViewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 vFragPos, vec3 vViewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 vFragPos, vec3 vViewDir);
float fog_factor(float d);

void main()
{
    /****************************/
    /*        Properties        */
    /****************************/
    vec3 vNorm = normalize(vNormal);
    vec3 vViewDir = normalize(vViewPos - vFragPos);


    /***********************************************/
    /*        Phase 1: Directional Lighting        */
    /***********************************************/
    vec3 result = CalcDirLight(sDirLight, vNorm, vViewDir);


    /*****************************************/
    /*        Phase 2: Point Lighting        */
    /*****************************************/
    result += CalcPointLight(sPlayerLight, vNorm, vFragPos, vViewDir) * fPlayerLightScalar;
    result += bKeyOn ? CalcPointLight(sKeyLight, vNorm, vFragPos, vViewDir) : vec3(0.0);
    result += bInPortalOn ? CalcPointLight(sInPortalLight, vNorm, vFragPos, vViewDir) : vec3(0.0);
    result += bOutPortalOn ? CalcPointLight(sOutPortalLight, vNorm, vFragPos, vViewDir) : vec3(0.0);


    /******************************/
    /*        Phase 3: Fog        */
    /******************************/
    vec4 camera_eye = vec4(vViewPos, 1.0);
    float d = distance(camera_eye, vec4(vFragPos, 1.0));
    float alpha = fog_factor(d);


    /******************************************/
    /*        Phase 4: Combine Results        */
    /******************************************/
    result = mix(result, vFogColor, alpha);

    float falloffMap = RadialFalloff(sPlayerLight.position, vFragPos) * fPlayerLightScalar;
    falloffMap += bKeyOn ? RadialFalloff(sKeyLight.position, vFragPos) : 0.0;
    falloffMap += bInPortalOn ? RadialFalloff(sInPortalLight.position, vFragPos) : 0.0;
    falloffMap += bOutPortalOn ? RadialFalloff(sOutPortalLight.position, vFragPos) : 0.0;

    result *= falloffMap;
    
    result *= 1.2;

    vFragColor = vec4(result, 1.0);
}



float RadialFalloff(vec3 vPos, vec3 vFragPos)
{
    return 0.8 / distance(vec4(vPos, 1.0), vec4(vFragPos, 1.0));
}



vec3 CalcDirLight(DirLight light, vec3 normal, vec3 vViewDir)
{
    vec3 light_dir = normalize(-light.direction);

    /*********************************/
    /*        Diffuse Shading        */
    /*********************************/
    float diff = max(dot(normal, light_dir), 0.0);

    /**********************************/
    /*        Specular Shading        */
    /**********************************/
    vec3 reflect_dir = reflect(-light_dir, normal);

    float spec = pow(max(dot(vViewDir, reflect_dir), 0.0), sMaterial.shininess);

    /********************************/
    /*        Combine Result        */
    /********************************/
    vec3 ambient  = light.ambient * vec3(sMaterial.diffuse);
    vec3 diffuse  = light.diffuse * diff * vec3(sMaterial.diffuse);
    vec3 specular = light.specular * spec * vec3(sMaterial.specular);

    return ambient + diffuse + specular;
}


// Calculates the color when using a point light
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 vFragPos, vec3 vViewDir)
{
    vec3 light_dir = normalize(light.position - vFragPos);

    /*********************************/
    /*        Diffuse Shading        */
    /*********************************/
    float diff = max(dot(normal, light_dir), 0.0);

    /**********************************/
    /*        Specular Shading        */
    /**********************************/
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(vViewDir, reflect_dir), 0.0), sMaterial.shininess);

    /*************************************/
    /*        Attenuation Shading        */
    /*************************************/
    float distance    = length(light.position - vFragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    /********************************/
    /*        Combine Result        */
    /********************************/
    vec3 ambient  = light.ambient * sMaterial.diffuse;
    vec3 diffuse  = light.diffuse * diff * sMaterial.diffuse;
    vec3 specular = light.specular * spec * sMaterial.specular;

    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}


vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 vFragPos, vec3 vViewDir)
{
    vec3 light_dir = normalize(light.position - vFragPos);

    /*********************************/
    /*        Diffuse Shading        */
    /*********************************/
    float diff = max(dot(normal, light_dir), 0.0);

    /**********************************/
    /*        Specular Shading        */
    /**********************************/
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(vViewDir, reflect_dir), 0.0), sMaterial.shininess);

    /*************************************/
    /*        Attenuation Shading        */
    /*************************************/
    float distance    = length(light.position - vFragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

    /*************************************/
    /*        Spotlight Intensity        */
    /*************************************/
    float theta = dot(light_dir, normalize(-light.direction));
    float epsilon = light.cutoff - light.outer_cutoff;
    float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0);

    /********************************/
    /*        Combine Result        */
    /********************************/
    vec3 ambient  = light.ambient * vec3(sMaterial.diffuse);
    vec3 diffuse  = light.diffuse * diff * vec3(sMaterial.diffuse);
    vec3 specular = light.specular * spec * vec3(sMaterial.specular);

    ambient  *= attenuation * intensity;
    diffuse  *= attenuation * intensity;
    specular *= attenuation * intensity;

    return ambient + diffuse + specular;
}



float fog_factor(float d)
{
    const float fog_max = 100.0;
    const float fog_min = 10.0;

    if (d >= fog_max)
        return 1.;

    if (d <= fog_min)
        return 0.;

    return 1.0 - (fog_max - d) / (fog_max - fog_min);
}
