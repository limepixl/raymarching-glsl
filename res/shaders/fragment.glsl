#version 460 core
out vec4 color;
uniform vec2 windowSize;
uniform float time;

const int MAXSTEPS = 500;
const float MINDISTANCE = 0.01;
const float MAXDISTANCE = 100.0;

float sphereSDF(vec3 point, vec4 sphereData)
{
    return length(point - sphereData.xyz) - sphereData.w;
}

float boxSDF(vec3 point, vec3 box)
{
    vec3 q = abs(point) - box;
    return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0) - 0.1;
}

// Returns the distance to the closest hit
float SceneDist(vec3 point)
{
    vec4 sphere = vec4(1.1, 1.0, -6.0, 1.0);
    vec4 sphere2 = vec4(-1.1, 1.0, -6.0, 1.0);
    float sphereDist1 = sphereSDF(point, sphere);
    float sphereDist2 = sphereSDF(point, sphere2);
    float planeDist = point.y;

    float minDist = min(min(sphereDist1, sphereDist2), planeDist);
    return minDist;
}

float March(vec3 origin, vec3 direction)
{
    float currentDistance = 0.0;
    for(int i = 0; i < MAXSTEPS; i++)
    {
        vec3 point = origin + direction * currentDistance;
        float dist = SceneDist(point);

        currentDistance += dist;

        // If marching very close to object, OR
        // marching towards infinity
        if(dist < MINDISTANCE || dist > MAXDISTANCE)
            break;
    }

    return currentDistance;
}

// TODO: Visualize this
vec3 GetNormal(vec3 point)
{
    float dist = SceneDist(point);
    float delta = 0.01;   // Used for secant point2

    vec3 n = dist - vec3(SceneDist(point - vec3(delta, 0.0, 0.0)),
                  SceneDist(point - vec3(0.0, delta, 0.0)),
                  SceneDist(point - vec3(0.0, 0.0, delta)));

    return normalize(n);
}

void main()
{
    vec2 uv = (gl_FragCoord.xy - 0.5 * windowSize.xy) / windowSize.y;

    vec3 center = vec3(0.0, 1.0, 0.0);
    vec3 direction = normalize(vec3(uv.x, uv.y, -1));

    float dist = March(center, direction);
    vec3 point = center + dist*direction;

    // Simple point light
    vec3 lightPos = vec3(0.0, 10.0, -1.0);
    lightPos.x += 10.0 * sin(time);
    lightPos.z += 10.0 * cos(time);

    vec3 lightDir = normalize(lightPos - point);
    vec3 normalVector = GetNormal(point);
    float diffuse = max(0.0, dot(lightDir, normalVector));

    vec3 ambient = vec3(0.1);

    // Raymarch towards the light to see if point is in shadow
    // Raising the point that the raymarch starts from because the 
    // marching ends right away if it starts at the found point from the last march.
    float distToLight = March(point + normalVector * MINDISTANCE*2.0, normalize(lightPos - point));
    if(distToLight < length(lightPos - point))
        diffuse *= 0.1;

    vec3 objectColor = vec3(1.0);
    color = vec4(diffuse * objectColor, 1.0);
}