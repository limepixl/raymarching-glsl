#version 460 core
out vec4 color;
uniform vec2 windowSize;
uniform vec2 mousePos;
uniform float scale;

float map(float current, float oldX, float oldY, float newX, float newY)
{
    return (current.x - oldX) / (oldY - oldX) * (newY - newX) + newX;
}

void main()
{
    vec4 borders = vec4(-2.0, 0.5, -1.25, 1.25);
    int iterations = 100;

    vec2 fragPos = gl_FragCoord.xy;

    float cx = map(fragPos.x, mousePos.x, windowSize.x + mousePos.x, borders.x, borders.y);
    float cy = map(fragPos.y, mousePos.y, windowSize.y + mousePos.y, borders.z, borders.w);

    float zx = 0;
    float zy = 0;

    for(int k = 0; k < iterations; k++)
    {
        highp float val = float(k);
        val = map(val, 0, iterations-1, 0.0, 1.0);

        float tempZX = zx*zx - zy*zy + cx;
        float tempZY = 2*zx*zy + cy;

        zx = tempZX;
        zy = tempZY;

        color = vec4(val, 0.0, 0.0, 1.0);

        float dist = distance(vec2(zx, zy), vec2(0.0, 0.0));
        if(dist > 2)
            break;
    }
}