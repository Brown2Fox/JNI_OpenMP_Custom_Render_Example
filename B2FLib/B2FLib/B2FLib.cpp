#include "b2f_lib_B2FLib.h"

#include <iostream>
#include <omp.h>
#include "model.h"

JNIEXPORT void JNICALL Java_b2f_lib_B2FLib_calc(JNIEnv *pEnv, jclass pClass, jint a)
{
	omp_set_num_threads(a);
	
	#pragma omp parallel
	{
		printf("Threads: %i\n", omp_get_num_threads());
	};
}

class Pixel
{
	int x;
	int y;
	int r;
	int g;
	int b;

public:
	Pixel(int x_, int y_, int r_, int g_, int b_)
	{
		x = x_;
		y = y_;
		r = r_;
		g = g_;
		b = b_;
	}
};

void rasterizeTriangle(Vec3i t0, Vec3i t1, Vec3i t2, Vec2i uv0, Vec2i uv1, Vec2i uv2, std::vector<Pixel> &image, float intensity, int *zbuffer)
{
	if (t0.y == t1.y && t0.y == t2.y)
	{
		return;
	} 
	if (t0.y > t1.y)
	{
		std::swap(t0, t1);
		std::swap(uv0, uv1);
	}
	if (t0.y > t2.y)
	{
		std::swap(t0, t2);
		std::swap(uv0, uv2);
	}
	if (t1.y > t2.y)
	{
		std::swap(t1, t2);
		std::swap(uv1, uv2);
	}

	int height_A = t2.y - t0.y;
	Vec3f dir_A = Vec3f(t2 - t0);
	Vec2i dir_uvA = uv2 - uv0;
	int height_B_first = t1.y - t0.y;
	Vec3f dir_B_first = Vec3f(t1 - t0);
	Vec2i dir_uvB_first = uv1 - uv0;
	int height_B_second = t2.y - t1.y;
	Vec3f dir_B_second = Vec3f(t2 - t1);
	Vec2i dir_uvB_second = uv2 - uv1;

	Vec3i B;
	Vec2i uvB;

	for (int y = 0; y < height_A; y++)
	{
		bool second_half = y > height_B_first || t1.y == t0.y;

		int segment_height = second_half ? height_B_second : height_B_first;

		float alpha = (float)y / height_A;
		Vec3i A = t0 + dir_A * alpha;
		Vec2i uvA = uv0 + dir_uvA * alpha;

		if (second_half)
		{
			float beta = (float)(y - height_B_first) / segment_height;
			B = t1 + dir_B_second * beta;
			uvB = uv1 + dir_uvB_second * beta;
		}
		else
		{
			float beta = (float)y / segment_height;
			B = t0 + dir_B_first * beta;
			uvB = uv0 + dir_uvB_first * beta;
		}


		if (A.x > B.x)
		{
			std::swap(A, B);
			std::swap(uvA, uvB);
		}

		for (int x = A.x; x <= B.x; x++)
		{
			float phi = B.x == A.x ? 1.f : (float)(x - A.x) / (float)(B.x - A.x);

			Vec3i P = Vec3f(A) + Vec3f(B - A) * phi;
			Vec2i uvP = uvA + (uvB - uvA) * phi;

			int idx = P.x + P.y * width;
			if (P.z > zbuffer[idx])
			{
				zbuffer[idx] = P.z;
				image.push_back(P.x, P.y, 1 * intensity, 1 * intensity, 1 * intensity);
			}
		}
	}
}






JNIEXPORT jobjectArray JNICALL Java_b2f_lib_B2FLib_render(JNIEnv *pEnv, jclass pClass, jint xSize, jint ySize)
{
	jclass Point = pEnv->FindClass("b2f/Point");

	jint fullSize = xSize * ySize;
	jobjectArray PointArray = pEnv->NewObjectArray(fullSize, Point, NULL);
	
	jmethodID Point_init = pEnv->GetMethodID(Point, "<init>", "(III)V");
	if (NULL == Point_init) return NULL;


	Model *pModel = new Model("obj/african_head.obj");


	for (int i = 0; i < fullSize; i++)
	{
		jobject p = pEnv->NewObject(Point, Point_init, 255, 0, 0);
		pEnv->SetObjectArrayElement(PointArray, i, p);
	}

	return PointArray;
}


