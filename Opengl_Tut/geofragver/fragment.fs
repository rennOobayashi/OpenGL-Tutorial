#version 330 core
out vec4 FragColor;

in vec3 world_pos;
in vec3 normal;
in vec2 texcoords;

uniform samplerCube irradiance_map;
uniform samplerCube prefilter_map;
uniform sampler2D brdf_lut;

uniform vec3 cam_pos;

uniform sampler2D albedo_map;
uniform sampler2D normal_map;
uniform sampler2D metallic_map;
uniform sampler2D roughness_map;
uniform sampler2D ao_map;

uniform vec3 light_positions[4];
uniform vec3 light_colors[4];

const float pi = 3.14159265359;

vec3 fresnel_schlick(float costheta, vec3 fo);
vec3 fresnel_schlick_roughness(float costheta, vec3 fo, float roughness);
float distributionggx(vec3 n, vec3 h, float roughness);
float geometry_schlickggx(float ndotv, float roughness);
float gemoetry_smith(vec3 n, vec3 v, vec3 l, float roughness);

void main()
{
    vec3 albedo = pow(texture(albedo_map, texcoords).rgb, vec3(2.2));
    float metallic = texture(metallic_map, texcoords).r;
    float roughness = texture(roughness_map, texcoords).r;
    float ao = texture(ao_map, texcoords).r;

    vec3 n = normal;
    vec3 v = normalize(cam_pos - world_pos);
    vec3 r = reflect(-v, n);

    vec3 fo = vec3(0.04);
    fo = mix(fo, albedo, metallic);
    
    vec3 lo = vec3(0.0);

    for (int i = 0; i < 4; i++) {
        vec3 l = normalize(light_positions[i] - world_pos);
        vec3 h = normalize(v + l);

        float distance = length(light_positions[i] - world_pos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = light_colors[i] * attenuation;

        vec3 f = fresnel_schlick(max(dot(h, v), 0.0), fo);
        float ndf = distributionggx(n, h, roughness);
        float g = gemoetry_smith(n, v, l, roughness);

        vec3 numerator = ndf * g * f;
        float denominator = 4.0 * max(dot(n, v), 0.0) * max(dot(n, l), 0.0) + 0.0001; //0으로 끝나지 않게 0.0001을 더함
        vec3 specular = numerator / denominator;

        //반사되는 빛에너지
        vec3 ks = f;
        //굴절되는 빛, 금속 표면은 빛을 굴절시지키 않아(확산 반사X) 표면이 금속이면 kd 무효화하여 적용
        vec3 kd = vec3(1.0) - ks;
        kd *= 1.0 - metallic;

        float ndotl = max(dot(n, l), 0.0);
        lo += (kd * albedo / pi + specular) * radiance * ndotl;
    }

    vec3 f = fresnel_schlick_roughness(max(dot(n, v), 0.0), fo, roughness);

    vec3 ks = f;
    vec3 kd = 1.0 - ks;
    kd *= 1.0 - metallic;

    vec3 irradiance = texture(irradiance_map, n).rgb;
    vec3 diffuse = irradiance * albedo;

    const float max_reflection_lod = 4.0;
    vec3 prefiltered_color = textureLod(prefilter_map, r, roughness * max_reflection_lod).rgb;
    vec2 env_brfd = texture(brdf_lut, vec2(max(dot(n, v), 0.0), roughness)).rg;
    vec3 specular = prefiltered_color * (f * env_brfd.x + env_brfd.y);

    vec3 ambient = (kd * diffuse + specular) * ao;
    vec3 color = ambient + lo;

    //감마 보정
    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);

    FragColor.rgb = normal;
    FragColor.a = 1.0;
}

//얼마나 표면의 빛을 잘 반사하고 굴절시키는지 계산(프리넬 방정식)
vec3 fresnel_schlick(float costheta, vec3 fo) {
    return fo + (1.0 - fo) * pow(clamp(1.0 - costheta, 0.0, 1.0), 5.0);
}

vec3 fresnel_schlick_roughness(float costheta, vec3 fo, float roughness) {
    return fo + (max(vec3(1.0 - roughness), fo) - fo) * pow(clamp(1.0 - costheta, 0.0, 1.0), 5.0);
}

float distributionggx(vec3 n, vec3 h, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float ndoth = max(dot(n, h), 0.0);
    float ndoth2 = ndoth * ndoth;

    float num = a2;
    float denom = (ndoth2 * (a2 - 1.0) + 1.0);
    denom = pi * denom * denom;

    return num / denom;
}

float geometry_schlickggx(float ndotv, float roughness) {
    float r = roughness + 1.0;
    float k = (r * r) / 8.0;

    float num = ndotv;
    float denom = ndotv * (1.0 - k) + k;

    return num / denom;
}

float gemoetry_smith(vec3 n, vec3 v, vec3 l, float roughness) {
    float ndotv = max(dot(n, v), 0.0);
    float ndotl = max(dot(n, l), 0.0);
    float ggx1 = geometry_schlickggx(ndotl, roughness);
    float ggx2 = geometry_schlickggx(ndotv, roughness);

    return ggx1 * ggx2;
}