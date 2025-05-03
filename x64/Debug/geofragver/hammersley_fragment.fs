#version 330 core
out vec4 FragColor;
in vec3 local_pos;

const float pi = 3.14159265359;

uniform samplerCube environment_map;
uniform float roughness;

float van_der_corput(uint n, uint base);
float distribution_ggx(float n_dot_h, float roughness);
vec2 hammersley(uint i, uint n);
vec3 importance_sample_ggx(vec2 xi, vec3 n, float roughness);

void main() {
    const uint sample_cnt = 4096u;
    vec3 n = normalize(local_pos);
    vec3 r = n;
    vec3 v = r;
    float total_weight = 0.0;
    vec3 prefilterd_color = vec3(0.0);

    for (uint i = 0u; i < sample_cnt; i++) {
        vec2 xi = hammersley(i, sample_cnt);
        vec3 h = importance_sample_ggx(xi, n, roughness);
        vec3 l = normalize(2.0 * dot(v, h) * h - v);

        float n_dot_l = max(dot(n, l), 0.0);

        if (n_dot_l > 0.0) {
            float n_dot_h = max(dot(n, h), 0.0);
            float h_dot_v = max(dot(h, v), 0.0);

            float d = distribution_ggx(n_dot_h, roughness);
            float pdf = (d * n_dot_h / (4.0 * h_dot_v)) + 0.0001;

            float resolution = 512.0;
            float sa_texel = 4.0 * pi / (6.0 * resolution * resolution);
            float sa_sample = 1.0 / (float(sample_cnt) * pdf + 0.0001);

            float mip_level = roughness == 0.0 ? 0.0 : 0.5 * log2(sa_sample / sa_texel);

            prefilterd_color += textureLod(environment_map, l, mip_level).rgb * n_dot_l;
            total_weight += n_dot_l;
        }
    }

    prefilterd_color = prefilterd_color / total_weight;

    FragColor = vec4(prefilterd_color, 1.0);
}

float van_der_corput(uint n, uint base) {
    float inv_base = 1.0 / float(base);
    float denom = 1.0;
    float result = 0.0;

    for (uint i = 0u; i < 32u; i++) {
        if (n > 0u) {
            denom = mod(float(n), 2.0);
            result += denom * inv_base;
            inv_base = inv_base / 2.0;
            n = uint(float(n) / 2.0);
        }
    }

    return result;
}

vec2 hammersley(uint i, uint n) {
    return vec2(float(i) / float(n), van_der_corput(i, 2u));
}

vec3 importance_sample_ggx(vec2 xi, vec3 n, float roughness) {
    float a = roughness * roughness;

    float phi = 2.0 * pi * xi.x;
    float costheta = sqrt((1.0 - xi.y) / (1.0 + (a * a - 1.0) * xi.y));
    float sintheta = sqrt(1.0 - costheta * costheta);

    vec3 h;
    h.x = cos(phi) * sintheta;
    h.y = sin(phi) * sintheta;
    h.z = costheta;

    vec3 up = abs(n.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
    vec3 tangent = normalize(cross(up, n));
    vec3 bitangent = cross(n, tangent);

    vec3 sample_vec = tangent * h.x + bitangent * h.y + n * h.z;

    return normalize(sample_vec);
}

float distribution_ggx(float n_dot_h, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float n_dot_h2 = n_dot_h * n_dot_h;

    float nom = a2;
    float denom = (n_dot_h2 * (a2 - 1.0) + 1.0);
    denom = pi * denom * denom;

    return nom / denom;
}