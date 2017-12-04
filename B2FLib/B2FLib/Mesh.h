#ifndef __MESH_H__
#define __MESH_H__

#include <vector>
#include <glm/glm.hpp>

struct face 
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

class mesh 
{

public:

	std::vector<glm::vec3> v_list;	// v 1.0 1.0 1.0
	std::vector<glm::vec3> vn_list; // vn 1.0 1.0 1.0
	std::vector<glm::vec2> vt_list; // vt 1.0 1.0
	std::vector<face> f_list;	//  f v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3 v4/vt4/vn4 ...

	mesh(const char *filename);
	~mesh();

	glm::vec3 getV(int face_idx, int v_idx) const;
	glm::vec2 getVt(int face_idx, int vt_idx) const;
	glm::vec3 getVn(int face_idx, int vn_idx) const;
	int nfaces() const;
	int nverts() const;

};

#endif //__MESH_H__
