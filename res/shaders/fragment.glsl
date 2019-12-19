#version 460 core
out vec4 color;
uniform vec2 windowSize;

const int MAXSTEPS = 500;
const float MINDISTANCE = 0.01;
const float MAXDISTANCE = 100.0;

float map(float current, float oldX, float oldY, float newX, float newY)
{
    return (current.x - oldX) / (oldY - oldX) * (newY - newX) + newX;
}

float sphereSDF(vec3 point)
{
    return length(point) - 1;
}

float boxSDF(vec3 point, vec3 box)
{
    vec3 q = abs(point) - box;
    return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0) - 0.1;
}

float shortestDistance(vec3 origin, vec3 direction)
{
    float currentDistance = 0.0;
    for(int i = 0; i < MAXSTEPS; i++)
    {
        vec3 point = origin + direction * currentDistance;
        float dist = boxSDF(point, vec3(0.5));

        if(dist < MINDISTANCE) 
            return currentDistance;

        currentDistance += dist;

        if(dist >= MAXDISTANCE)
            return MAXDISTANCE;
    }

    return MAXDISTANCE;
}

vec3 rayDirection(float fieldOfView, vec2 size, vec2 fragCoord) {
    vec2 xy = fragCoord - size / 2.0;
    float z = size.y / tan(radians(fieldOfView) / 2.0);
    return normalize(vec3(xy.x, xy.y + 500, -z));
}

vec3 normal(vec3 point)
{
    vec3 box = vec3(0.5);
    vec3 n = vec3(boxSDF(vec3(point.x + MINDISTANCE, point.y, point.z), box) - boxSDF(vec3(point.x - MINDISTANCE, point.y, point.z), box),
                  boxSDF(vec3(point.x, point.y + MINDISTANCE, point.z), box) - boxSDF(vec3(point.x, point.y - MINDISTANCE, point.z), box),
                  boxSDF(vec3(point.x, point.y, point.z + MINDISTANCE), box) - boxSDF(vec3(point.x, point.y, point.z - MINDISTANCE), box));
    return normalize(n);
}

void main()
{
    vec3 center = vec3(0.0, -2.0, 10.0);
    vec3 direction = rayDirection(45.0, windowSize, gl_FragCoord.xy);
    vec3 lightPos = vec3(0.0, -1.0, 0.0);

    float dist = shortestDistance(center, direction);
    vec3 n = normal(center + direction*dist);
    float shading = max(dot(normalize(n - lightPos), n), 0.0);
    
    if(dist > MAXDISTANCE - MINDISTANCE)
    {
        color = vec4(0.0);
        return;
    }

    color = vec4(shading * vec3(1.0), 1.0);
}