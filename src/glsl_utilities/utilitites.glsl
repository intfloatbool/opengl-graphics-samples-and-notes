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