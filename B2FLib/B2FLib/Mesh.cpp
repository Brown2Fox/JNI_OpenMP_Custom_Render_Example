#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "Mesh.h"

Mesh::Mesh(const char *filename) : v_list(), f_list(), vn_list(), vt_list()
{
    std::ifstream in;
    in.open (filename, std::ifstream::in);
    if (in.fail()) return;
    std::string line;
    while (!in.eof()) 
	{
        std::getline(in, line);
        std::istringstream iss(line.c_str());
        char trash;
        if (!line.compare(0, 2, "v ")) 
		{
            iss >> trash;
            Vec3f v;
            for (int i = 0; i < 3; i++) iss >> v[i];
            v_list.push_back(v);
        } 
		else if (!line.compare(0, 3, "vn ")) 
		{
            iss >> trash >> trash;
            Vec3f vn;
            for (int i = 0; i < 3; i++) iss >> vn[i];
            vn_list.push_back(vn);
        } 
		else if (!line.compare(0, 3, "vt ")) 
		{
            iss >> trash >> trash;
            Vec2f vt;
            for (int i = 0; i < 2; i++) iss >> vt[i];
            vt_list.push_back(vt);
        }  
		else if (!line.compare(0, 2, "f ")) 
		{
			Face f;
			int v = 0;
			int vt = 0;
			int vn = 0;
			iss >> trash;
            while (iss >> v >> trash >> vt >> trash >> vn) 
			{
                f.push_back(v, vt, vn);
            }
            f_list.push_back(f);
        }
    }
    std::cout << "# v# " << v_list.size() << " f# "  << f_list.size() << " vt# " << vt_list.size() << " vn# " << vn_list.size() << std::endl;
}

Mesh::~Mesh() {
}

Vec3f Mesh::getV(int face_idx, int v_idx) const
{
	return v_list[f_list[face_idx].v_idx_list[v_idx]];
}

Vec2f Mesh::getVt(int face_idx, int vt_idx) const
{
	return vt_list[f_list[face_idx].vt_idx_list[vt_idx]];
}

Vec3f Mesh::getVn(int face_idx, int vn_idx) const
{
	return vn_list[f_list[face_idx].vn_idx_list[vn_idx]];
}

int Mesh::nfaces() const 
{
	return f_list.size();
}



