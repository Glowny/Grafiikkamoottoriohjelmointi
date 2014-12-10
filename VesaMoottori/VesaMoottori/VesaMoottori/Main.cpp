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
#include <vector>

using namespace glm;



void readObject(std::string filename, std::vector<glm::vec3> &out_verticies, std::vector<glm::vec2> &out_uvs);

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

	
	std::string filename = "box.obj";

	
	pekka.Register();
	pekka.Show();

	bool a = shaders.CreateShader("vertexShader.txt",  GL_VERTEX_SHADER);
	assert(a == true);
	a = shaders.CreateShader("fragmentShader.txt",  GL_FRAGMENT_SHADER);
	assert(a == true);
	 shaders.LinkProgram();



	// Buffereiden luonti
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	readObject(filename, vertices, uvs);
	GLuint vertexBuffer = buffer.CreateVertexBuffers(&vertices[0], vertices.size()*sizeof(glm::vec3));
	GLuint uvBuffer = buffer.CreateVertexBuffers(&uvs[0], uvs.size()*sizeof(glm::vec2));
	//GLuint indexBuffer = buffer.CreateIndexBuffers(&indexDataVec.front(), indexDataVec.size()*sizeof(GLuint));
	//setting uniforms
	glClearColor(0.0f, 0.8f, 0.0f, 0.0f);
	glUseProgram(shaders.getProgramObject());
	
	/*glScissor(200, 200, 500, 500);
	glEnable(GL_SCISSOR_TEST);*/
	GLint viewIndex = glGetUniformLocation(shaders.getProgramObject(), "unifView");		// shadereissa jotai hämärää, onnistuuko lataus?

	assert(viewIndex != -1);

	GLint alphaIndex = glGetUniformLocation(shaders.getProgramObject(), "unifAlpha");
	assert(alphaIndex != -1);

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
			//glEnable(GL_STENCIL_TEST);
			//glStencilFunc(GL_ALWAYS, 1, 0xFF);
			//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			//glStencilMask(0xFF);
			//glDepthMask(GL_FALSE);
			
			//
			 /** rotate(5.0f, vec3(1.0f, 0.0f, 0.0f));*/
			viewTransform = translate(vec3(0.0f, -1.0f, -100.0f))* rotate (wow,vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(worldIndex, 1, GL_FALSE,
				reinterpret_cast<float*>(&worldTransform));
			glUniformMatrix4fv(viewIndex, 1, GL_FALSE,
				reinterpret_cast<float*>(&viewTransform));
			wow+=0.5;
			glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

			glVertexAttribPointer(posLocation, 3u, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>(0));	// testaa
			//glVertexAttribPointer(colorLocation, 3u, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));

			glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
			glVertexAttribPointer(texLocation, 2u, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>(0));
	
			glEnableVertexAttribArray(posLocation);
		//	glEnableVertexAttribArray(colorLocation);
			glEnableVertexAttribArray(texLocation);

			// järjestys:
			//Lattia, heijastus, seinä

			//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);       !
			glBindTexture(GL_TEXTURE_2D, texture.GetTexture());
			// 18 edellisessä, indeksien mukaan.
			// heijastus alle

			glUniformMatrix4fv(worldIndex, 1, GL_FALSE, reinterpret_cast<float*>(&worldTransform));
			glUniform1f(alphaIndex, 1.0f);
			//glDrawElements(GL_TRIANGLES, vertices.size() , GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>(0));
			glDrawArrays(GL_TRIANGLES, 0, vertices.size());
	
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
			//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0u);			!
			glBindBuffer(GL_ARRAY_BUFFER, 0u);
			glUseProgram(0);

	}

	//glDeleteTextures(1, &texture);
	glDeleteBuffers(1, &vertexBuffer);
	//glDeleteBuffers(1, &indexBuffer);          !
	return (int) messages.wParam;
}
void readObject(std::string filename, std::vector<glm::vec3> & out_verticies, std::vector<glm::vec2> & out_uvs)
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<GLuint> vertexIndices;
	std::vector<GLuint> uvIndicies;

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
				glm::vec3 vertice;
				fscanf(file, "%f", &vertice.x);
				fscanf(file, "%f", &vertice.y);
				fscanf(file, "%f", &vertice.z);
				vertices.push_back(vertice);
			}

			if (strcmp(line, "vt") == 0)
			{
				glm::vec2 uv;
				fscanf(file, "%f", &uv.x);
				fscanf(file, "%f", &uv.y);
				uvs.push_back(uv);
			}

			if (strcmp(line, "f") == 0)
			{
				glm::ivec3 temp_index;
				glm::ivec3 temp_textureIndex;
				fscanf(file, "%d/%d", &temp_index.x, &temp_textureIndex.x);
				fscanf(file, "%d/%d", &temp_index.y, &temp_textureIndex.y);
				fscanf(file, "%d/%d", &temp_index.z, &temp_textureIndex.z);
				vertexIndices.push_back(temp_index.x - 1);
				vertexIndices.push_back(temp_index.y - 1);
				vertexIndices.push_back(temp_index.z - 1);
				uvIndicies.push_back(temp_textureIndex.x - 1);
				uvIndicies.push_back(temp_textureIndex.y - 1);
				uvIndicies.push_back(temp_textureIndex.z - 1);
			}
		}


		for (unsigned i = 0; i < vertexIndices.size(); i++)
		{
			unsigned int vertexIndex = vertexIndices[i];
			glm::vec3 vertex = vertices[vertexIndex];

			out_verticies.push_back(vertex);
			
			// uvIndicies saman kokoinen

			unsigned int uvIndex = uvIndicies[i];
			glm::vec2 uv = uvs[uvIndex];

			out_uvs.push_back(uv);

		}

		
		
	}
	fclose(file);
}