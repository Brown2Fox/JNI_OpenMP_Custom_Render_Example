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
			Face f = {0};
			int v = 0;
			int vt = 0;
			int vn = 0;
			int idx = 0;
			iss >> trash;
            while (iss >> v >> trash >> vt >> trash >> vn) 
			{
                f.set(v, vt, vn, idx++);
            }
            f_list.push_back(f);
        }
    }
    std::cerr << "# v# " << v_list.size() << " f# "  << f_list.size() << " vt# " << vt_list.size() << " vn# " << vn_list.size() << std::endl;
}

Mesh::~Mesh() {
}




