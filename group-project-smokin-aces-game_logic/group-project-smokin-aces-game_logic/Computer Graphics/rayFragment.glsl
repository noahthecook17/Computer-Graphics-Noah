#version 330 core

in vec3 ipPos;

//should update the depth buffer

struct Ray
{
	vec3 direction;
	float maxLength;
	vec3 origin;
};

struct Triangle
{
	vec3 v1;
	vec3 v2;
	vec3 v3;
};

struct Camera
{
	vec3 location;
	float fov;

};

struct OBB
{
	vec3 center;
	vec3 normals[3];
	float distances[3];
};

uniform samplerBuffer triangleData;
uniform Camera camera;
uniform Triangle triangle;

float rejectionTest(vec3 origin, vec3 direction, OBB box)
{
	bool firstTest = true;
	float tmax = 0.0f;
	float tmin = 0.0f;
	vec3 p = box.center - origin;
	for(int i = 0; i < 3; i++)
	{
		float e = dot(box.normals[i], p);
		float f = dot(box.normals[i], direction);
		
		if(abs(f) > 1e-20){
			float t1 = (e + box.distances[i])/f;
			float t2 = (e - box.distances[i])/f;
			if(t1 > t2){
				float temp = t1;
				t1 = t2;
				t2 = temp;
			}
			if(firstTest){
				tmin = t1;
				tmax = t2;	
				firstTest = false;
			}	
			else{
				if(t1 > tmin) tmin = t1;
				if(t2 < tmax) tmax = t2;
				if(tmin > tmax) return -1.0f;
				if(tmax < 0) return -1.0f;
				else if( -e - box.distances[i] > 0 || -e + box.distances[i] < 0) return -1.0f;
			}
		}
	}
	if(tmin > 0.0f) return tmin;
	else return tmax;	
};

bool intersectRayTri(Triangle tri, vec3 o, vec3 n) {
    vec3 e1, e2, pvec, qvec, tvec;

    e1 = tri.v2 - tri.v1;
    e2 = tri.v3 - tri.v1;
    pvec = cross(n, e2);
    
    n = normalize(n);
    //NORMALIZE(pvec);
    float det = dot(pvec, e1);

    if (det != 0)
    {
        float invDet = 1.0f / det;
        tvec = o - tri.v1;
        // NORMALIZE(tvec);
        float u = invDet * dot(tvec, pvec);
        if (u < 0.0f || u > 1.0f)
        {

            return false;
        }
        qvec = cross(tvec, e1);
        // NORMALIZE(qvec);
        float v = invDet * dot(qvec, n);
        if (v < 0.0f || u + v > 1.0f)
        {

            return false;
        }
    }
    else return false;
    return true; // det != 0 and all tests for false intersection fail
}

//need to convert fragcoord to ndc of the camera face in model space
//then calculate the direction to the interpolated 

//the clip space ray direction is just the FragCoord after it has been converted to ndc?

void main(){

	Camera camera;
	camera.location = vec3(0.0f, 0.0f, 0.0f); // model space coordinates for the camera
	camera.fov = 0.5f;

	Ray ray;
	ray.origin = camera.location;
	ray.direction = normalize(ipPos + vec3(0.0f, 0.0f, camera.fov));	
	
	Triangle triangle;
	triangle.v1 = vec3(texelFetch(triangleData, 0));
	triangle.v2 = vec3(texelFetch(triangleData, 1));
	triangle.v3 = vec3(texelFetch(triangleData, 2));	

	if(intersectRayTri(triangle, ray.origin, ray.direction)){
		gl_FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);	
	}
	else gl_FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

	//gl_FragColor = vec4(vec3(texelFetch(triangleData, 0)), 1.0f);
				
};