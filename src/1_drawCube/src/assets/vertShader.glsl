#version 430

layout (location = 0) in vec3 position;
uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;

uniform float tf;

out vec4 varyingColor;

// declare functions:
mat4 buildTranslate(float x, float y, float z);
mat4 buildRotateX(float rad);
mat4 buildRotateY(float rad);
mat4 buildRotateZ(float rad);
mat4 buildScale(float x, float y, float z);

void main(void)
{
    float i = gl_InstanceID + tf;
    
    float a = sin(203.0 * i/8000.0) * 403.0;
    float b = cos(301.0 * i/4001.0) * 401.0;
    float c = sin(400.0 * i/6003.0) * 405.0;

    mat4 localRotX = buildRotateX(1000 * i);
    mat4 localRotY = buildRotateY(1000 * i);
    mat4 localRotZ = buildRotateZ(1000 * i);
    mat4 localTrans = buildTranslate(a, b, c);

    // build the model matrix and then the model-view matrix
    mat4 newM_matrix = m_matrix * localTrans * localRotX * localRotY * localRotZ;
    
    mat4 mv_matrix = v_matrix * newM_matrix;


    // old code
    gl_Position = proj_matrix * mv_matrix * vec4(position, 1.0);

    varyingColor = vec4(position, 1.0) * 0.5 + vec4(0.5, 0.5, 0.5, 0.5);
}

//builds and returns a translation matrix
mat4 buildTranslate(float x, float y, float z)
{
    mat4 trans = mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        x, y, z, 1.0
    );

    return trans;
}

// builds and retursn a matrix that performs a rotation around X axis
mat4 buildRotateX(float rad)
{
    mat4 xRot = mat4(

        1.0, 0.0, 0.0, 0.0,
        0.0, cos(rad), -sin(rad), 0.0,
        0.0, sin(rad), cos(rad), 0.0,
        0.0, 0.0, 0.0, 1.0

    );

    return xRot;
}

mat4 buildRotateY(float rad)
{
    mat4 yRot = mat4(
        cos(rad), 0.0, sin(rad), 0.0,
        0.0, 1.0, 0.0, 0.0,
        -sin(rad), 0.0, cos(rad), 0.0,
        0.0, 0.0, 0.0, 1.0
    );

    return yRot;
}

mat4 buildRotateZ(float rad)
{
    mat4 zRot = mat4(
        cos(rad), -sin(rad), 0.0, 0.0,
        sin(rad), cos(rad), 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        0.0, 0.0, 0.0, 1.0
    );

    return zRot;
}

mat4 buildScale(float x, float y, float z)
{
    mat4 scale = mat4(
        x, 0.0, 0.0, 0.0,
        0.0, y, 0.0, 0.0,
        0.0, 0.0, z, 0.0,
        0.0, 0.0, 0.0, 1.0
    );

    return scale;
}