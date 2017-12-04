#include "b2f_lib_B2FLib.h"

#include <iostream>
#include <omp.h>
#include "Mesh.h"

JNIEXPORT void JNICALL Java_b2f_lib_B2FLib_calc(JNIEnv *pEnv, jclass pClass, jint a)
{
	omp_set_num_threads(a);
	
	#pragma omp parallel
	{
		printf("Threads: %i\n", omp_get_num_threads());
	};
}

struct Pixel
{
	int x;
	int y;
	int r;
	int g;
	int b;

	Pixel(int x_, int y_, int r_, int g_, int b_)
	{
		x = x_;
		y = y_;
		r = r_;
		g = g_;
		b = b_;
	}
};

void rasterizeTriangle(glm::vec3 v[3], glm::vec2 vt[3], std::vector<Pixel> &image, float intensity, int *zbuffer, int width)
{
	if (v[0].y == v[1].y && v[0].y == v[2].y)
	{
		return;
	} 
	if (v[0].y > v[1].y)
	{
		std::swap(v[0], v[1]);
		std::swap(vt[0], vt[1]);
	}
	if (v[0].y > v[2].y)
	{
		std::swap(v[0], v[2]);
		std::swap(vt[0], vt[2]);
	}
	if (v[1].y > v[2].y)
	{
		std::swap(v[1], v[2]);
		std::swap(vt[1], vt[2]);
	}

	int height_A = v[2].y - v[0].y;
	glm::vec3 dir_A = v[2] - v[0];
	glm::vec2 dir_vtA = vt[2] - vt[0];
	int height_B_first = v[1].y - v[0].y;
	glm::vec3 dir_B_first = v[1] - v[0];
	glm::vec2 dir_vtB_first = vt[1] - vt[0];
	int height_B_second = v[2].y - v[1].y;
	glm::vec3 dir_B_second = v[2] - v[1];
	glm::vec2 dir_vtB_second = vt[2] - vt[1];

	glm::vec3 B;
	glm::vec2 vtB;

	for (int y = 0; y < height_A; y++)
	{
		bool second_half = y > height_B_first || v[1].y == v[0].y;

		int segment_height = second_half ? height_B_second : height_B_first;

		float alpha = (float)y / height_A;
		glm::vec3 A = v[0] + dir_A * alpha;
		glm::vec2 vtA = vt[0] + dir_vtA * alpha;

		if (second_half)
		{
			float beta = (float)(y - height_B_first) / segment_height;
			B = v[1] + dir_B_second * beta;
			vtB = vt[1] + dir_vtB_second * beta;
		}
		else
		{
			float beta = (float)y / segment_height;
			B = v[0] + dir_B_first * beta;
			vtB = vt[0] + dir_vtB_first * beta;
		}


		if (A.x > B.x)
		{
			std::swap(A, B);
			std::swap(vtA, vtB);
		}

		for (int x = A.x; x <= B.x; x++)
		{
			float phi = B.x == A.x ? 1.f : (float)(x - A.x) / (float)(B.x - A.x);

			glm::vec3 P = A + (B - A) * phi;
			glm::vec2 uvP = vtA + (vtB - vtA) * phi;

			int idx = P.x + P.y * width;
			if (P.z > zbuffer[idx])
			{
				zbuffer[idx] = P.z;

				image.push_back(Pixel(P.x, P.y, 100 * intensity, 1 * intensity, 1 * intensity));
			}
		}
	}
}






JNIEXPORT jobjectArray JNICALL Java_b2f_lib_B2FLib_render(JNIEnv *pEnv, jclass pClass, jint width, jint height, jint depth)
{
	FILE* logFile;
	fopen_s(&logFile, "log.txt", "w+");
	fprintf(logFile, "==========HALO!!!=======\n");

	jclass jPixel = pEnv->FindClass("b2f/Pixel");

	jint fullSize = width * height;
	jobjectArray jImage = pEnv->NewObjectArray(fullSize, jPixel, NULL);

	std::vector<Pixel> image;
	
	jmethodID jPixel_init = pEnv->GetMethodID(jPixel, "<init>", "(IIIII)V");
	if (NULL == jPixel_init) return NULL;

	
	mesh *pMesh = new mesh("D:/Users/Antti/Workflow/BBB/obj/african_head.obj");

	float r = 150.f;
	float l = 150.f;
	float t = 150.f;
	float b = 150.f;
	float f = 1000.f;
	float n = 1.f;

	glm::mat4x4 P = glm::mat4x4({ {2. / (r - l), 0., 0., -(r + l) / (r - l)},
									  {0., 2. / (t - b), 0., -(t + b) / (t - b)},
									  {0., 0., 2. / (f - n), -(f + n) / (f - n)},
									  {0., 0., 0., 1.}});


	std::vector<glm::vec3> clipping_space_coords(pMesh->nverts());

	#pragma omp parallel for shared(pMesh->v_list, clipping_space_coords)
	for (int i = 0; i < pMesh->nverts(); i++)
	{
		// wrap vertex into homogeneous coordinates
		glm::vec4 v = glm::vec4(pMesh->v_list[i], 1);
		// projecting
		glm::vec4 v_p = P * v;
		// convert homogeneous coordinates to normalized device coordinates (NDC) 
		clipping_space_coords[i] = glm::vec3(v_p.x / v_p.w, v_p.y / v_p.w, v_p.z / v_p.w);
	}


	int *zbuffer = new int[width * height];
	for (int i = 0; i < width * height; i++)
	{
		zbuffer[i] = std::numeric_limits<int>::min();
	}



	#pragma omp parallel for shared(zbuffer, pMesh, image) schedule(dynamic)
	for (int i = 0; i < pMesh->nfaces(); i++)
	{
		
		Vec3i screen_coords[3];
		Vec3f world_coords[3];
		
		for (int j = 0; j < 3; j++)
		{
			Vec3f v = pMesh->getV(i, j);
			screen_coords[j] = Vec3i((v.x + 1.) * width / 2., (v.y + 1.) * height / 2., (v.z + 1.) * depth / 2.);
			world_coords[j] = v;
		}
		
		Vec3f n = (world_coords[2] - world_coords[0]) ^ (world_coords[1] - world_coords[0]);
		
		n.normalize();
		float intensity = n * Vec3f(0.2,0,-0.5);
		
		if (intensity > 0)
		{
			Vec2i vt_triangle[3];
			for (int k = 0; k < 3; k++)
			{
				vt_triangle[k] = pMesh->getVt(i, k);
			}
			

			//rasterizeTriangle(screen_coords, vt_triangle, image, intensity, zbuffer, width);
		}
		
	}

	
	for (int i = 0; i < image.size(); i++)
	{
		jobject p = pEnv->NewObject(jPixel, jPixel_init, image[i].x, image[i].y, image[i].r, image[i].g, image[i].b);
		pEnv->SetObjectArrayElement(jImage, i, p);
	}
	
	return jImage;
}


