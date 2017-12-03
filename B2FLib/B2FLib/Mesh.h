#ifndef __MESH_H__
#define __MESH_H__

#include <vector>
#include "Geometry.h"

struct Face 
{
	std::vector<int> v_face;
	std::vector<int> vn_face;
	std::vector<int> vt_face;

	void set(unsigned int vlist_idx, unsigned int vnlist_idx, unsigned int vtlist_idx, unsigned short idx)
	{
		v_face[idx] = vlist_idx;
		vn_face[idx] = vnlist_idx;
		vt_face[idx] = vtlist_idx;
	}
};

class Mesh 
{

public:

	std::vector<Vec3f> v_list;	// v 1.0 1.0 1.0
	std::vector<Vec3f> vn_list; // vn 1.0 1.0 1.0
	std::vector<Vec2f> vt_list; // vt 1.0 1.0
	std::vector<Face> f_list;	//  f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 v4/vt4/vn4 ...

	Mesh(const char *filename);
	~Mesh();

};

#endif //__MESH_H__
