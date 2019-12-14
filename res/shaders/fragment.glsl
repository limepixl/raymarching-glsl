#version 460 core
out vec4 color;
uniform vec2 windowSize;

float map(float current, float oldX, float oldY, float newX, float newY)
{
    return (current.x - oldX) / (oldY - oldX) * (newY - newX) + newX;
}

void main()
{
    int iterations = 100;
    vec2 fragPos = gl_FragCoord.xy;

    float cx = map(fragPos.x, 0.0, windowSize.x, -2.0, 0.5);
    float cy = map(fragPos.y, 0.0, windowSize.y, -1.25, 1.25);

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

        color = vec4(0.0, val, 0.0, 1.0);

        float dist = distance(vec2(zx, zy), vec2(0.0, 0.0));
        if(dist > 2)
        {
            break;
        }
    }
}