#include "GraphicsDevice.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "GL\glew.h"
#include "Buffers.h"
#include "ShaderProgram.h"
#include "Sprite.h"
#include "cassert"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\transform.hpp"

using namespace glm;
//static const GLfloat triangleData[] =
//{
////  X	  Y     Z
//	0.0f, 0.8f, 0.0f,	//0	// Positio.
//	1.0f, 0.0f, 0.0f,	// Väri.
//	0.0f, 0.0f,			// Tekstuuri.
//
//	-0.5f, -0.5f, 0.0f,//1
//	0.0f, 1.0f, 0.0f,
//	0.0f, 1.0f,
//
//	0.0f, -0.8f, -0.8f, //2
//	0.0f, 0.0f, 1.0f,
//	1.0f, 1.0f,
//	// 
//	0.5f, -0.5f, 0.0f,  //3
//	1.0f, 0.0f, 1.0f,
//	1.0f, 0.0f,
//
//	0.0f, -0.8f, 0.8f, //4
//	1.0f, 1.0f, 0.0f,
//	0.0f, 0.0f,
//};
//
//static const GLuint indexData[] = // koko 18
//{
//	0, 1, 2,
//	0, 2, 3,
//	0, 3, 4,
//	0, 1, 4,
//	1, 2, 3,
//	4, 3, 1
//
//};

//static const GLfloat triangleData [] =
//{
//	// lattia
//	-1.0f, 0.0f, 1.0f,	// 0
//	0.0f, 0.0f, 1.0f,
//	0.0f, 0.0f,
//
//	-1.0f, 0.0f, -1.0f,	// 1
//	0.0f, 0.0f, 1.0f,
//	0.0f, 0.0f,
//
//	1.0f, 0.0f, 1.0f,	// 2
//	0.0f, 0.0f, 1.0f,
//	0.0f, 0.0f,
//
//	1.0f, 0.0f, -1.0f,	// 3
//	0.0f, 0.0f, 1.0f,
//	0.0f, 0.0f,
//
//	// pohja
//	-0.8f, 0.0f, 0.8f,	// 4
//	0.0f, 1.0f, 0.0f,
//	0.0f, 0.0f,
//
//	-0.8f, 0.0f, -0.8f,	// 5
//	0.0f, 1.0f, 0.0f,
//	0.0f, 0.0f,
//
//	0.8f, 0.0f, 0.8f,	// 6
//	0.0f, 1.0f, 0.0f,
//	0.0f, 0.0f,
//
//	0.8f, 0.0f, -0.8f,	// 7
//	0.0f, 1.0f, 0.0f,
//	0.0f, 0.0f,
//
//	// katto
//	-0.8f, 0.8f, 0.8f,	// 8
//	1.0f, 0.0f, 0.0f,
//	0.0f, 0.0f,
//
//	-0.8f, 0.8f, -0.8f,	// 9
//	1.0f, 0.0f, 0.0f,
//	0.0f, 0.0f,
//
//	0.8f, 0.8f, 0.8f,	// 10
//	1.0f, 0.0f, 0.0f,
//	0.0f, 0.0f,
//
//	0.8f, 0.8f, -0.8f,	// 11
//	1.0f, 0.0f, 0.0f,
//	0.0f, 0.0f
//
//};
//static const GLuint indexData [] =
//{
//	//lattia
//	0, 1, 2,
//	1, 2, 3,
//	//pohja
//	4, 5, 6,
//	5, 6, 7,
//	//katto
//	8, 9, 10,
//	9, 10, 11,
//	//seinä1
//	4, 6, 8,
//	6, 8, 10,
//	//seinä2
//	4, 5, 8,
//	5, 8, 9,
//	//seinä3
//	5, 7, 9,
//	7, 9, 11,
//	//seinä4
//	6, 7, 10,
//	7, 10, 11
//
//
//};
GLfloat* vertexData;
GLuint*	 indexData;
GLuint	 indexSize;
GLuint   vertexSize;


void readObject(std::string filename);

int main()
{
	ResourceManager resourceManager;
	Texture			texture;
	bool			isRunning = true;
	MSG				messages;
	GraphicsDevice	pekka("eitoimicustomnimi", 800, 800);
	Buffers			buffer;
	ShaderProgram	shaders;
	Sprite			sprite();

	
	std::string filename = "box2.obj";

	
	pekka.Register();
	pekka.Show();

	shaders.CreateShader("vertexShader.txt",  GL_VERTEX_SHADER);
	shaders.CreateShader("fragmentShader.txt",  GL_FRAGMENT_SHADER);
	shaders.LinkProgram();


	// Buffereiden luonti
	readObject(filename);
	GLuint vertexBuffer = buffer.CreateVertexBuffers(vertexData, vertexSize*sizeof(GLfloat));

	GLuint indexBuffer = buffer.CreateIndexBuffers(indexData, indexSize*sizeof(GLuint));

	//setting uniforms
	glClearColor(0.0f, 0.8f, 0.0f, 0.0f);
	glUseProgram(shaders.getProgramObject());
	
	/*glScissor(200, 200, 500, 500);
	glEnable(GL_SCISSOR_TEST);*/
	GLint alphaIndex = glGetUniformLocation(shaders.getProgramObject(), "unifAlpha");
	assert(alphaIndex != -1);

	GLint viewIndex = glGetUniformLocation(shaders.getProgramObject(), "unifView");
	assert(viewIndex != -1);

	GLint projectionIndex = glGetUniformLocation(shaders.getProgramObject(), "unifProjektio");
	assert(projectionIndex != -1);
	
	mat4 projectionTransform = perspective(60.0f, 800.0f / 800.0f, 0.01f, 1000.0f);
	
	glUniformMatrix4fv(projectionIndex, 1, GL_FALSE,
		reinterpret_cast<float*>(&projectionTransform));

	

	GLint worldIndex = glGetUniformLocation(shaders.getProgramObject(), "unifWorld");
	assert(worldIndex != -1);

	float wow = 0;
	mat4 worldTransform;
	mat4 viewTransform;

	glUniformMatrix4fv(viewIndex, 1, GL_FALSE,
		reinterpret_cast<float*>(&viewTransform));
	glUseProgram(0u);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	resourceManager.LoadPicture("boxTex.png");
	texture = Texture(resourceManager.FindImage("boxTex.png"), 305, 329);
	

	//resourceManager.RLoadImage("goofy.png");
	//ImageInfo *image= resourceManager.FindImage("goofy.png");
	//GLuint texture = textureManager.CreateTexture(*image);
	//GLuint texture = resourceManager.FindImage("goofy.png");


	// Tarkistetaan attribuuttien lokaatio.
	const GLint posLocation = shaders.GetAttributeLocation("attrPosition");
	const GLint colorLocation = shaders.GetAttributeLocation("attrColor");
	const GLint texLocation = shaders.GetAttributeLocation("textPosition");


	while (isRunning)
	{
	
		
		while (PeekMessage(&messages, NULL, 0, 0, PM_REMOVE)) // Ikkuna ottaa vastaan viestejä.
		{
			if (messages.message == WM_QUIT)
			{
				isRunning = false;
				//wglDeleteContext(hGLRC); // Tuhotaan renderöinti sisältö - pitää vielä koodata windowiin.
				break;
			}
			
			DispatchMessage(&messages);
			
		}
			pekka.Update();
			shaders.RunProgram();
			glStencilMask(0xFF);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
			//glUseProgram
			worldTransform = mat4();
			//stencil setup
			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_ALWAYS, 1, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			glStencilMask(0xFF);
			glDepthMask(GL_FALSE);
			
			//
			 /** rotate(5.0f, vec3(1.0f, 0.0f, 0.0f));*/
			viewTransform = translate(vec3(0.0f, -1.0f, -100.0f))* rotate (wow,vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(worldIndex, 1, GL_FALSE,
				reinterpret_cast<float*>(&worldTransform));
			glUniformMatrix4fv(viewIndex, 1, GL_FALSE,
				reinterpret_cast<float*>(&viewTransform));
			wow+=0.5;
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

			glVertexAttribPointer(posLocation, 3u, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(0));	// testaa
			glVertexAttribPointer(colorLocation, 3u, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
			glVertexAttribPointer(texLocation, 2u, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(6 * sizeof(GLfloat)));
	
			glEnableVertexAttribArray(posLocation);
			glEnableVertexAttribArray(colorLocation);
			glEnableVertexAttribArray(texLocation);

			// järjestys:
			//Lattia, heijastus, seinä

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
			glBindTexture(GL_TEXTURE_2D, texture.GetTexture());
			// 18 edellisessä, indeksien mukaan.
			// heijastus alle

			glUniformMatrix4fv(worldIndex, 1, GL_FALSE, reinterpret_cast<float*>(&worldTransform));
			glUniform1f(alphaIndex, 1.0f);
			glDrawElements(GL_TRIANGLES, 3*indexSize, GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>(0));
	
			////Stencil setup
			//glStencilFunc(GL_EQUAL, 1, 0xff);
			//glStencilMask(0x00);
			//glDepthMask(GL_TRUE);
			//

			//worldTransform = scale(vec3(1.0f, -1.0f, 1.0f));
			//glUniformMatrix4fv(worldIndex, 1, GL_FALSE, reinterpret_cast<float*>(&worldTransform));
			//glUniform1f(alphaIndex, 0.6f);
			//glDrawElements(GL_TRIANGLES, 42u, GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>(42*sizeof(GLuint))); 

			////disable stencil testing
			//glDisable(GL_STENCIL_TEST);

			//worldTransform = mat4();
			//glUniformMatrix4fv(worldIndex, 1, GL_FALSE, reinterpret_cast<float*>(&worldTransform));
			//glUniform1f(alphaIndex, 1.0f);
			//glDrawElements(GL_TRIANGLES, 42u, GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>(42*sizeof(GLuint))); 

			glBindTexture(GL_TEXTURE_2D, 0u);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);
			glBindBuffer(GL_ARRAY_BUFFER, 0u);
			glUseProgram(0);

	}

	//glDeleteTextures(1, &texture);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
	return (int) messages.wParam;
}
void readObject(std::string filename)
{
	std::vector<glm::vec3> vertexPosition;
	std::vector<glm::vec2> texturePosition;
	std::vector<GLuint> index;
	std::vector<GLuint> textureIndex;

	FILE* file = fopen(filename.c_str(), "r");
	if (!file)
	{
		std::cout << "File " << filename << " missing!" << std::endl;
	}
	
	else
	{
		int isEoF = 0;
		while (1)
		{ 
			char line[128];
			isEoF = fscanf(file, "%s", line);
			if (isEoF == EOF)
			{
				break;
			}
			//printf("%s\n", &line);
			
			if (strcmp(line, "v") == 0)
			{
				glm::vec3 vertex;
				fscanf(file, "%f", &vertex.x);
				fscanf(file, "%f", &vertex.y);
				fscanf(file, "%f", &vertex.z);
				vertexPosition.push_back(vertex);
			}

			if (strcmp(line, "vt") == 0)
			{
				glm::vec2 uv;
				fscanf(file, "%f", &uv.x);
				fscanf(file, "%f", &uv.y);
				texturePosition.push_back(uv);
			}

			if (strcmp(line, "f") == 0)
			{
				glm::ivec3 temp_index;
				glm::ivec3 temp_textureIndex;
				fscanf(file, "%d/%d", &temp_index.x, &temp_textureIndex.x);
				fscanf(file, "%d/%d", &temp_index.y, &temp_textureIndex.y);
				fscanf(file, "%d/%d", &temp_index.z, &temp_textureIndex.z);
				index.push_back(temp_index.x - 1);
				index.push_back(temp_index.y - 1);
				index.push_back(temp_index.z - 1);
				textureIndex.push_back(temp_textureIndex.x - 1);
				textureIndex.push_back(temp_textureIndex.y - 1);
				textureIndex.push_back(temp_textureIndex.z - 1);
			}
		}
		vertexSize = vertexPosition.size()*8;
		indexSize = index.size()*3;		// tekstureita yhtä monta

		vertexData = new GLfloat[vertexSize];
		indexData = new GLuint[indexSize];
		std::vector<GLfloat> vertexDataa;

		for (unsigned int i = 0; i < vertexPosition.size(); i++)
		{
			vertexDataa.push_back(vertexPosition[i].x);
			vertexDataa.push_back(vertexPosition[i].y);
			vertexDataa.push_back(vertexPosition[i].z);
			vertexDataa.push_back(1.0f);
			vertexDataa.push_back(1.0f);
			vertexDataa.push_back(1.0f);
			vertexDataa.push_back(texturePosition[textureIndex[i]].x);
			vertexDataa.push_back(texturePosition[textureIndex[i]].y);

		}
		for (int i = 0; i < vertexSize; i++)
		{
			std::cout << vertexData[i] << std::endl;
		}

		for (unsigned int i = 0; i < index.size(); i++)
		{
			indexData[i] = i;
		}
	}
	
}