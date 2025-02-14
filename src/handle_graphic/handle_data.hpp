#ifndef HANDLE_DATA
#define HANDLE_DATA

#include "GL/glew.h"
#include <iostream>
#include <vector>


struct Buffer_data{
	unsigned int VBO, VAO, EBO;
	Buffer_data(void);
	~Buffer_data();
};
 
#endif