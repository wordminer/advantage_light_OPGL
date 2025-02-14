#include "handle_data.hpp"

Buffer_data::Buffer_data(void)
{
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
}

Buffer_data::~Buffer_data(){
	glDeleteVertexArrays(1,&this->VAO);
	glDeleteBuffers(1,&this->VBO);
}

// Buffer_data::~Buffer_data()
// {
// 	glDeleteVertexArrays(1, &VAO);
// 	glDeleteBuffers(1, &VBO);
// }

// void Buffer_data::Bind_vertex_array() 
// {
// 	glBindVertexArray(VAO);
// }

// void Buffer_data::Buffer_array_data(const void* array_buffer, GLsizeiptr size_array, GLenum Type_usage)
// {
// 	glBufferData(GL_ARRAY_BUFFER, size_array, array_buffer, Type_usage);
// }

// void Buffer_data::Element_buffer_data(GLuint* element_array_buffer, GLsizeiptr size_array, GLenum Type_usage)
// {
// 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size_array, element_array_buffer, Type_usage);
// }

// void Buffer_data::AttribPointer(GLuint index_ptr, GLint size, GLenum type_ptr, GLboolean Is_normalized, GLsizei stride_distance, const void* start_point)
// {
// 	glVertexAttribPointer(index_ptr, size, type_ptr, Is_normalized, stride_distance, start_point);
// 	glEnableVertexAttribArray(index_ptr);
// }

