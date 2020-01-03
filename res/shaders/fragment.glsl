#version 460 core
out vec4 color;
uniform vec2 windowSize;
uniform float time;
uniform vec3 cameraPosition;

const int MAXSTEPS = 500;
const float MINDISTANCE = 0.01;
const float MAXDISTANCE = 100.0;

mat2 Rotate(float angle)
{
	float sine = sin(angle);
	float cosine = cos(angle);
	
	return mat2(cosine, -sine, sine, cosine);
}

float sphereSDF(vec3 point, float radius)
{
    return length(mod(point + 0.5, 1.0) - 0.5) - radius;
}

float pillSDF(vec3 point, vec3 A, vec3 B, float radius)
{
	vec3 AB = B - A;
	vec3 AP = point - A;
	
	float part = dot(AP, AB) / dot(AB, AB);
	part = clamp(part, 0, 1);
	vec3 linePoint = A + part * AB;
	
	float dist = length(point - linePoint) - radius;
	return dist;
}

float torusSDF(vec3 point, vec3 center, float r1, float r2)
{
	vec2 pXZ = point.xz;
	float x = length(pXZ) - r1;
	float y = point.y;
	
	float dist = length(vec2(x, y)) - r2;
	return dist;
}

// Returns the distance to the closest hit
float SceneDist(vec3 point)
{	
    float dist1 = sphereSDF(point, 0.2);

    float minDist = dist1;
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
    vec2 fragCoord = gl_FragCoord.xy / windowSize;
    vec2 uv = fragCoord - 0.5;
    uv.x *= windowSize.x / windowSize.y;

    vec3 direction = normalize(vec3(uv.x, uv.y, -1));

    float dist = March(cameraPosition, direction);
    vec3 point = cameraPosition + dist*direction;

    // Simple point light
    vec3 lightPos = vec3(0.0, 10.0, -1.0);

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