#version 330 core
out float frag_color;

in vec2 texcoords;

uniform sampler2D gposition;
uniform sampler2D gnormal;
uniform sampler2D noise_tex;

uniform vec3 samples[64];
uniform mat4 projection;

const vec2 noise_scale = vec2(1280.0 / 4.0, 720.0 / 4.0);

int kernel_size = 64; //sample kernel
float radius = 0.5;
float bias = 0.025;

void main()
{
    vec3 frag_pos = texture(gposition, texcoords).xyz;
    vec3 normal = texture(gnormal, texcoords).rgb;
    vec3 random_vec = texture(noise_tex, texcoords * noise_scale).xyz;

    vec3 tangent = normalize(random_vec - normal * dot(random_vec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 tbn = mat3(tangent, bitangent, normal);

    float occlusion = 0.0;

    for (int i = 0; i < kernel_size; i++) {
        vec3 sample_pos = tbn * samples[i];
        
        sample_pos =  frag_pos + sample_pos *radius;

        vec4 offset = vec4(sample_pos, 1.0);
        offset = projection * offset;
        offset.xyz /= offset.w;
        offset.xyz = offset.xyz * 0.5 + 0.5;

        float sample_depth = texture(gposition, offset.xy).z;
        float range_check = smoothstep(0.0, 1.0, radius / abs(frag_pos.z - sample_depth));

        occlusion += (sample_depth >= sample_pos.z + bias ? 1.0 : 0.0) * range_check;
    }

    occlusion = 1.0 - (occlusion / kernel_size);

    frag_color = pow(occlusion, radius); //kernel_size, radius, bias, noise_kernel_size(of have)
}