#version 460
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in vec3 aTangent;

out VS_OUT 
{
    vec3 FragPos;
    vec4 FragLightPos;
    vec3 Normal;
    vec2 TexCoords;
    mat3 TBN;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} vs_out;

layout (std430, binding = 0) buffer  Instance
{
    mat4 INSTANCE_TRANSFORMS[];
};

layout (std140, binding = 1) uniform Camera
{                                  // base alignment   // aligned offset
    mat4 CAMERA_PROJECTION_MATRIX; // 16               0   (column 0)
                                   // 16               16  (column 1)
                                   // 16               32  (column 2)
                                   // 16               48  (column 3)
    mat4 CAMERA_VIEW_MATRIX;       // 16               64  (column 0)
                                   // 16               80  (column 1)
                                   // 16               96  (column 2)
                                   // 16               112 (column 3)
    vec4 CAMERA_VIEW_POSITION;     // 16               128
};

layout (std140, binding = 2) uniform Shadow
{
	mat4 LIGHT_TRANSFORM_MATRIX;
};

void main()
{
    mat4 MODEL_MATRIX = INSTANCE_TRANSFORMS[gl_InstanceID];

    vs_out.FragPos        = vec3(MODEL_MATRIX * vec4(aPos, 1.0));
    vs_out.FragLightPos   = LIGHT_TRANSFORM_MATRIX * vec4(vs_out.FragPos, 1.0);
    
    vs_out.Normal         = transpose(inverse(mat3(MODEL_MATRIX))) * aNormal;
    vs_out.TexCoords      = aTexCoords;

    vec3 T = normalize(vec3(MODEL_MATRIX * vec4(aTangent, 0.0)));
    vec3 N = normalize(vec3(MODEL_MATRIX * vec4(aNormal,  0.0)));

    // Re-Orthogonalize T with respect to N
    T = normalize(T - dot(T, N) * N);

    // Retrieve perpendicular vector B with the cross product of T and N
    vec3 B = cross(N, T);

    // Tangent-Bitangent-Normal matrix (world space to tangent space)
    vs_out.TBN = mat3(T, B, N);
    vs_out.TangentViewPos  = vs_out.TBN * CAMERA_VIEW_POSITION.xyz;
    vs_out.TangentFragPos  = vs_out.TBN * vs_out.FragPos;

    gl_Position = CAMERA_PROJECTION_MATRIX * CAMERA_VIEW_MATRIX * MODEL_MATRIX * vec4(aPos, 1.0);
}