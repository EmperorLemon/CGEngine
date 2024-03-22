#version 460
layout (location = 0) in vec3 aPos;

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

uniform mat4 MODEL_MATRIX;

void main()
{
    gl_Position = CAMERA_PROJECTION_MATRIX * CAMERA_VIEW_MATRIX * MODEL_MATRIX * vec4(aPos, 1.0);
}