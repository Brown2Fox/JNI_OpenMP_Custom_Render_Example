#ifndef __MESH_H__
#define __MESH_H__

#include <vector>
#include "Geometry.h"

struct Face 
{
	std::vector<int> v_idx_list;
	std::vector<int> vn_idx_list;
	std::vector<int> vt_idx_list;

	void set(unsigned int vlist_idx, unsigned int vnlist_idx, unsigned int vtlist_idx, unsigned short idx)
	{
		v_idx_list[idx] = vlist_idx-1;
		vn_idx_list[idx] = vnlist_idx-1;
		vt_idx_list[idx] = vtlist_idx-1;
	}

	void push_back(unsigned int vlist_idx, unsigned int vnlist_idx, unsigned int vtlist_idx)
	{
		v_idx_list.push_back(vlist_idx - 1);
		vn_idx_list.push_back(vnlist_idx - 1);
		vt_idx_list.push_back(vtlist_idx - 1);
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

	Vec3f getV(int face_idx, int v_idx) const;
	Vec2f getVt(int face_idx, int vt_idx) const;
	Vec3f getVn(int face_idx, int vn_idx) const;
	int nfaces() const;

};

#endif //__MESH_H__
