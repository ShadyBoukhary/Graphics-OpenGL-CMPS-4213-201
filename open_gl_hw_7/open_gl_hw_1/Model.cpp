
#include "Model.h"
#include "Angel.h"
#include <SOIL.h>
#include <vector>
Model::Model()
{
}

Model::~Model()
{
}

// Parameratized constructor that reads in the model rply file
Model::Model(string Oname)
{
//	PLYModel plymodel("monkey.ply", true, false);//true normals , false colors
	model = PLYModel((Oname + ".ply").c_str());
	name = Oname;

	//for (int i = 0; i < model.positions.size(); i++) {
	//	cout << model.positions[i].x << "," << model.positions[i].y << ","
	//		<< model.positions[i].z << endl;
	//}
	/*for(int i=0;i<model.faces.size();i++)
		cout << model.faces[i].x << "," << model.faces[i].y << ","
				<< model.faces[i].z << endl;*/
}


// This method loads the model into OpenGL(ie onto the GPU)
// Have NOT included color yet!
bool Model::Load(GLuint program)
{
	cout << "Loading " << name <<" onto gpu"<< endl;
	string texName = name + ".png";
	//glUseProgram(program);
	// Create a vertex array object
	// Each obj has its own vao and its stored in the class
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	int width, height, channels; //these are returned
	string iname = name + ".png";
	unsigned char *image = SOIL_load_image(
		iname.c_str(),
		&width, &height, &channels,
		SOIL_LOAD_RGB
	);     
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);   
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);   
	SOIL_free_image_data(image);
	//glBindTexture(GL_TEXTURE_2D, 0);// unbinds this texture
	// Generate the faces index buffer. These must be GLushort
	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	int si=model.indices.size()*2;
	int sp = model.positions.size() * 16;//vec4
	int sn = model.normals.size() * 12;//vec3
	int sc = model.colors.size() * 12;
	int tc = model.uvs.size() * 8;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, si, &model.indices[0],GL_STATIC_DRAW);

	// Create and initialize a buffer object to hold the data
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sp + sn + sc + tc,// reserve space for pos and norms
		NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sp, &model.positions[0]);// copy over the positions
	glBufferSubData(GL_ARRAY_BUFFER, sp,// copy over the normals
		sn, &model.normals[0]);
	if(sc>0)//if using colors
	  glBufferSubData(GL_ARRAY_BUFFER, sp+sn,// copy over the colors
		sn, &model.colors[0]);
	if (tc > 0) {
		glBufferSubData(GL_ARRAY_BUFFER, sp + sn + sc,// copy over the colors
			tc, &model.uvs[0]);
		GLuint vTexCoordLoc = glGetAttribLocation(program, "vTexCoord");
		glEnableVertexAttribArray(vTexCoordLoc);
		glVertexAttribPointer(vTexCoordLoc, 2, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(sp + sn + sc));// starting location of the texture coordinates
		GLuint texMapLoc;
		texMapLoc = glGetUniformLocation(program, "texMap");
		glUniform1i(texMapLoc, 0); //Make it unit 0
	}
	// set up vertex arrays
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(0));

	GLuint vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(vNormal);
	glVertexAttribPointer(vNormal, 3, GL_FLOAT, GL_FALSE, 0,
		BUFFER_OFFSET(sp));
	if (sc > 0) {// if using colors
		GLuint vColor = glGetAttribLocation(program, "vColor");
		glEnableVertexAttribArray(vColor);
		glVertexAttribPointer(vColor, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(sp + sn));
	}

	glBindVertexArray(0);// unbind this VAO

	// This code we will study later when we get to
	// lighting and materials.
	// Initialize shader lighting parameters
	// Normally you should get these from the object data
	// I have set them to a constant here since ply does not
	// have this data.
	vec4 light_position(1.0, 1.0, 1.0, 0.0);
	vec4 light_ambient(0.2, 0.2, 0.2, 1.0);
	vec4 light_diffuse(.5, .5, .5, 1.0);
	vec4 light_specular(1.0, 1.0, 1.0, 1.0);

	vec4 material_ambient(1.0, 0.9, 0.31, 1.0);
	vec4 material_diffuse(1.0, .5, .31, 1.0);
	vec4 material_specular(.5, .5, .5, 1.0);
	float  material_shininess = 100.0;

	vec4 ambient_product = light_ambient * material_ambient;
	vec4 diffuse_product = light_diffuse * material_diffuse;
	vec4 specular_product = light_specular * material_specular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"),
		1, ambient_product);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"),
		1, diffuse_product);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"),
		1, specular_product);

	glUniform4fv(glGetUniformLocation(program, "LightPosition"),
		1, light_position);

	glUniform1f(glGetUniformLocation(program, "Shininess"),

		material_shininess);


	return true;
}


void Model::Draw(GLuint program)
{
	glBindVertexArray(vao);
	glBindTexture(GL_TEXTURE_2D, tex);// turns on texturing for this tex object
	//glDrawElements(GL_TRIANGLES, model.indices.size(), GL_UNSIGNED_SHORT, NULL);
	// you normally change the objects material and lighting settings
	// here if you know them. Ply files do not have lighting and materials
	glDrawElements(GL_TRIANGLES, model.indices.size(), GL_UNSIGNED_SHORT, NULL);
	glBindTexture(GL_TEXTURE_2D, 0);// turns off texturing for this tex object
}
