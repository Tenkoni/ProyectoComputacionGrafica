#include "toroide.h"

Toroide::Toroide(GLfloat radio, GLfloat radio_menor, GLfloat radio_mayor):
	radio(radio){
	toroide_VAO[4] = ( -1,-1,-1,-1 );
	toroide_VBO[4] = (-1, -1, -1, -1);
	this->r_menor = radio_menor;
	this->r_mayor = radio_mayor;
}

Toroide::~Toroide()
{
	glDeleteVertexArrays(4, toroide_VAO);
	glDeleteBuffers(4, toroide_VBO);
}

void Toroide::init()
{
	const int nn = PARALELOS * MERIDIANOS * 3;
	GLfloat toroide_nor[nn]; // normal
	// generate the toroide data
	GLfloat x, y, z, a, b, da, db;
	int ia, ib, ix, iy;
	da = (GLfloat)2.0*M_PI / GLfloat(MERIDIANOS);
	db = (GLfloat)M_PI / GLfloat(PARALELOS - 1);
	// [Generate toroide point data]
	// spherical angles a,b covering whole toroide surface
	for (ix = 0, b = (GLfloat)-0.5*M_PI, ib = 0; ib < PARALELOS; ib++, b += db)
		for (a = 0.0, ia = 0; ia < MERIDIANOS; ia++, a += da, ix += 3)
		{
			// unit toroide
			x = (r_mayor + r_menor * cos(b))*cos(a);
			y = (r_mayor + r_menor *cos(b))*sin(a);
			z = r_menor * sin(b);
			toroide_pos[ix + 0] = x * r_menor;
			toroide_pos[ix + 1] = y * r_menor;
			toroide_pos[ix + 2] = z * r_menor;
			toroide_nor[ix + 0] = x;
			toroide_nor[ix + 1] = y;
			toroide_nor[ix + 2] = z;
		}
	// [Generate GL_TRIANGLE indices]
	for (ix = 0, iy = 0, ib = 1; ib < PARALELOS; ib++)
	{
		for (ia = 1; ia < MERIDIANOS; ia++, iy++)
		{
			// first half of QUAD
			toroide_index[ix] = iy;      ix++;
			toroide_index[ix] = iy + 1;    ix++;
			toroide_index[ix] = iy + MERIDIANOS;   ix++;
			// second half of QUAD
			toroide_index[ix] = iy + MERIDIANOS;   ix++;
			toroide_index[ix] = iy + 1;    ix++;
			toroide_index[ix] = iy + MERIDIANOS + 1; ix++;
		}
		// first half of QUAD
		toroide_index[ix] = iy;       ix++;
		toroide_index[ix] = iy + 1 - MERIDIANOS;  ix++;
		toroide_index[ix] = iy + MERIDIANOS;    ix++;
		// second half of QUAD
		toroide_index[ix] = iy + MERIDIANOS;    ix++;
		toroide_index[ix] = iy - MERIDIANOS + 1;  ix++;
		toroide_index[ix] = iy + 1;     ix++;
		iy++;
	}
	// [VAO/VBO stuff]
	GLuint i;
	glGenVertexArrays(4, toroide_VAO);
	glGenBuffers(4, toroide_VBO);
	glBindVertexArray(toroide_VAO[0]);
	i = 0; // vertex
	glBindBuffer(GL_ARRAY_BUFFER, toroide_VBO[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(toroide_pos), toroide_pos, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	i = 1; // normal
	glBindBuffer(GL_ARRAY_BUFFER, toroide_VBO[i]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(toroide_nor), toroide_nor, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 3, GL_FLOAT, GL_FALSE, 0, 0);
	i = 2; // indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, toroide_VBO[i]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(toroide_index), toroide_index, GL_STATIC_DRAW);
	glEnableVertexAttribArray(i);
	glVertexAttribPointer(i, 4, GL_UNSIGNED_INT, GL_FALSE, 0, 0);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
}

void Toroide::render()
{
	//glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CCW);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glBindVertexArray(toroide_VAO[0]);
	//  glDrawArrays(GL_POINTS,0,sizeof(toroide_pos)/sizeof(GLfloat));                   // POINTS ... no indices for debug
	glDrawElements(GL_TRIANGLES, sizeof(toroide_index) / sizeof(GLuint), GL_UNSIGNED_INT, 0);    // indices (choose just one line not both !!!)
	glBindVertexArray(0);
}