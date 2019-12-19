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

float DistanceEstimator(vec3 point)
{
    return length(point) - 1;
}

float shortestDistance(vec3 origin, vec3 direction)
{
    float currentDistance = 0.0;
    for(int i = 0; i < MAXSTEPS; i++)
    {
        vec3 point = origin + direction * currentDistance;
        float dist = DistanceEstimator(point);

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
    return normalize(vec3(xy, -z));
}

vec3 normal(vec3 point)
{
    vec3 n = vec3(DistanceEstimator(vec3(point.x + MINDISTANCE, point.y, point.z)) - DistanceEstimator(vec3(point.x - MINDISTANCE, point.y, point.z)),
                  DistanceEstimator(vec3(point.x, point.y + MINDISTANCE, point.z)) - DistanceEstimator(vec3(point.x, point.y - MINDISTANCE, point.z)),
                  DistanceEstimator(vec3(point.x, point.y, point.z + MINDISTANCE)) - DistanceEstimator(vec3(point.x, point.y, point.z - MINDISTANCE)));
    return normalize(n);
}

void main()
{
    vec3 center = vec3(0.0, 0.0, 6.0);
    vec3 direction = rayDirection(45.0, windowSize, gl_FragCoord.xy);

    float dist = shortestDistance(center, direction);
    vec3 n = normal(center + direction*dist);
    float shading = max(dot(-direction, n), 0.0);
    
    if(dist > MAXDISTANCE - MINDISTANCE)
    {
        color = vec4(0.0);
        return;
    }

    color = vec4(shading * vec3(1.0), 1.0);
}