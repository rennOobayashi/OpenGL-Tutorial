#version 330 core
out vec2 FragColor;

in vec2 texcoords;

const float pi = 3.14159265359;

float van_der_corput(uint n, uint base);
float distribution_ggx(float n_dot_h, float roughness);
float geometry_schlick_ggx(float n_dot_v, float roughness);
float geometry_smith(vec3 n, vec3 v, vec3 l, float roughness);
vec2 integrate_brdf(float n_dot_v, float roughness);
vec2 hammersley(uint i, uint n);
vec3 importance_sample_ggx(vec2 xi, vec3 n, float roughness);

void main()
{
    vec2 i_brdf = integrate_brdf(texcoords.x, texcoords.y);

    FragColor = i_brdf;
}

vec2 integrate_brdf(float n_dot_v, float roughness) {
    vec3 v;
    vec3 n = vec3(0.0, 0.0, 1.0);
    float a = 0.0;
    float b = 0.0;
    const uint sample_cnt = 1024u;

    v.x = sqrt(1.0 - n_dot_v * n_dot_v);
    v.y = 0.0;
    v.z = n_dot_v;

    for (uint i = 0u; i < sample_cnt; i++) {
        vec2 xi = hammersley(i, sample_cnt);
        vec3 h = importance_sample_ggx(xi, n, roughness);
        vec3 l = normalize(2.0 * dot(v, h) * h - v);

        float n_dot_l = max(l.z, 0.0);
        float n_dot_h = max(h.z, 0.0);
        float v_dot_h = max(dot(v, h), 0.0);

        if (n_dot_l > 0.0) {
            float g = geometry_smith(n, v, l, roughness);
            float g_vis = (g*v_dot_h) / (n_dot_h * n_dot_v);
            float fc = pow(1.0 - v_dot_h, 5.0);

            a += (1.0 - fc) * g_vis;
            b = fc * g_vis;
        }
    }

    a /= float(sample_cnt);
    b /= float(sample_cnt);

    return vec2(a, b);
}

float geometry_schlick_ggx(float n_dot_v, float roughness) {
    float a = roughness;
    float k = (a * a) / 2.0;

    float nom = n_dot_v;
    float denom = n_dot_v * (1.0 - k) + k;

    return nom / denom;
}
float geometry_smith(vec3 n, vec3 v, vec3 l, float roughness) {
    float n_dot_v = max(dot(n, v), 0.0);
    float n_dot_l = max(dot(n, l), 0.0);
    float ggx1 = geometry_schlick_ggx(n_dot_l, roughness);
    float ggx2 = geometry_schlick_ggx(n_dot_v, roughness);

    return ggx1 * ggx2;
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