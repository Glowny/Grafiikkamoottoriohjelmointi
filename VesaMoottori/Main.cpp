#include "CMWindow.h"
#include "glew.h"
#include "ResourceManager.h"
#include "CMSprite.h"
#include "RenderingContext.h"
#include "glm\glm.hpp"
#include "glm\gtc\matrix_transform.hpp"
#include "glm\gtx\transform.hpp"
#include "CMInput.h"
#include "Camera.h"
#include <vector>
#include <string>

#include <iostream>


GLuint _texture[2];
GLint _positionIndex;
GLint _colorIndex;
GLint _textureIndex;


GLuint buffers[2];
RenderingContext rendContext;
RenderingContext rendContext2;

void Render(HWND asd);
int main()
{
	CML::CMWindow window(0, L"asd", 800, 600);
	window.ShowCMWindow();
	rendContext = RenderingContext(&window);

	//activate attribute arrays
	
	_positionIndex = glGetAttribLocation(rendContext.getProgramIndex(), "attrPosition");
	assert(_positionIndex >= 0);
	glEnableVertexAttribArray(_positionIndex);

	_colorIndex = glGetAttribLocation(rendContext.getProgramIndex(), "attrColor");
	assert(_colorIndex >= 0);
	glEnableVertexAttribArray(_colorIndex);

	_textureIndex = glGetAttribLocation(rendContext.getProgramIndex(), "attrTexCoord");
	assert(_textureIndex >= 0);
	glEnableVertexAttribArray(_textureIndex);


	const GLint _samplerLocation = glGetUniformLocation(rendContext.getProgramIndex(), "myTexture");
	assert(_samplerLocation >= 0);

	const GLint _projectionIndex = glGetUniformLocation(rendContext.getProgramIndex(), "unifProjection");
	assert(_projectionIndex >= 0);

	const GLint _worldIndex = glGetUniformLocation(rendContext.getProgramIndex(), "unifWorld");
	assert(_projectionIndex >= 0);

	const GLint _cameraIndex = glGetUniformLocation(rendContext.getProgramIndex(), "unifCamera");
	assert(_cameraIndex >= 0);

	GLint _alphaChannel = glGetUniformLocation(rendContext.getProgramIndex(), "alphaChannel");
	assert(_alphaChannel >= 0);
	//texture stuff

	glGenTextures(2, _texture);//generates texture

	CML::CMSprite sprite2 = CML::CMSprite::CMSprite(0.0f, 0.0f, 200.0f, 150.0f, "sample.png");
	CML::CMSprite sprite = CML::CMSprite::CMSprite(0.0f, 0.0f, 200.0f, 150.0f, "tribaltatuointi.png");

	glBindTexture(GL_TEXTURE_2D, _texture[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, sprite.GetImage().getWidth(), sprite.GetImage().getHeight(), 0, sprite.GetImage().getImageFormat(), GL_UNSIGNED_BYTE, FreeImage_GetBits(sprite.GetImage().getBITMAP()));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, _texture[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, sprite2.GetImage().getWidth(), sprite2.GetImage().getHeight(), 0, sprite2.GetImage().getImageFormat(), GL_UNSIGNED_BYTE, FreeImage_GetBits(sprite2.GetImage().getBITMAP()));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glBindTexture(GL_TEXTURE_2D, 0u);

	

	glGenBuffers(2, buffers);

		int _windowHeight = 600;
		int _windowWidht = 800;






	//Data buhveli, in order: 2x position, 3x colour, 2x texture coordinates
	GLfloat _vertexBufferInput[] = {

		-1.0f, 0.0f, -1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f,

		-1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		5.0f, 0.0f,

		1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,
		5.0f, 3.0f,

		1.0f, 0.0f, -1.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 3.0f,

		//wall
		-0.3f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f,

		-0.3f, 0.5f, 0.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, 1.0f,

		0.3f, 0.5f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 1.0f,

		0.3f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,
		1.0f, 0.0f,


	};
	std::vector<glm::vec3> vPosition;
	std::vector<glm::vec3> tPosition;
	std::vector<glm::ivec3> index;
	int indexSize;
	int vertexSize;

	GLfloat* box;
	GLuint* boxi;
	FILE * file = fopen("Poshpot.obj", "r");
	if (!file)
	{ 
		printf("kaik meni infernaalisesti\n");
	}
	else
	{
		while (1)
		{
			char line[128];
		
			int IsEof = fscanf(file, "%s", line);
			printf("%s\n", &line);
			if (IsEof == EOF)
				break;
			if (strcmp(line, "v") == 0)
			{
				glm::vec3 vertex;
				fscanf(file, "%f",&vertex.x);
				fscanf(file, "%f", &vertex.y);
				fscanf(file, "%f", &vertex.z);
				vPosition.push_back(vertex);
				printf("hubailu");
			}
			if (strcmp(line, "vt") == 0)
			{
				glm::vec3 uv;
				fscanf(file, "%f", &uv.x);
				fscanf(file, "%f", &uv.y);
				tPosition.push_back(uv);
			}
			if (strcmp(line, "f") == 0)
			{
				unsigned int blah;
				glm::ivec3 tempus_index;
				fscanf(file, "%d/%d", &tempus_index.x, &blah);
				fscanf(file, "%d/%d", &tempus_index.y, &blah);
				fscanf(file, "%d/%d", &tempus_index.z, &blah);
				index.push_back(tempus_index);
			}
		}
		vertexSize = vPosition.size();
		box = new GLfloat[vertexSize * 8];
		for (int i = 0; i < vPosition.size(); i++)
		{
			box[i * 8 + 0] = vPosition.at(i).x;
			box[i * 8 + 1] = vPosition.at(i).y;
			box[i * 8 + 2] = vPosition.at(i).z;
			box[i * 8 + 3] = 0.0f;
			box[i * 8 + 4] = 0.0f;
			box[i * 8 + 5] = 0.0f;
			box[i * 8 + 6] = tPosition.at(i).x;
			box[i * 8 + 7] = tPosition.at(i).y;
			
		}
		boxi = new GLuint[3 * index.size()];
		indexSize = index.size();
		for (int i = 0; i < indexSize; i++)
		{
			boxi[i * 3] = index.at(i).x - 1;
			boxi[i * 3 + 1] = index.at(i).y - 1;
			boxi[i * 3 + 2] = index.at(i).z - 1;
		}

		//for (int i = 0; i < 8 * 8; i++)
		//	std::cout << box[i] << std::endl;
		//for (int i = 0; i < 3 * index.size(); i++)
		//	std::cout << boxi[i * 3] << " , " << boxi[i * 3 + 1] << " , " <<  boxi[i * 3 + 2] << std::endl;
	}
 




	if (_vertexBufferInput) //this is the data given to vertex buffer, rename as needed
	{
		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
		glBufferData(GL_ARRAY_BUFFER, 8 * vertexSize * sizeof(GLuint), box, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		std::cout << "vertexBufferInput works, yay" << std::endl;
	}

	GLint _indexBufferInput[] = 
	{
		0u, 1u, 2u, 2u, 3u, 0u,
		//0u, 4u, 5u, 0u, 5u, 1u,
		//1u, 5u, 2u, 2u, 5u, 6u,
		//2u, 6u, 3u, 6u, 3u, 7u,
		//3u, 0u, 7u, 7u, 0u, 4u,
		4u, 5u, 6u, 6u, 7u, 4u,

	};

	if (_indexBufferInput) //this is the data given to fragment buffer, rename as needed
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3 * indexSize*sizeof(GLuint), boxi, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		std::cout << "FragmentBufferInput works too, another yay" << std::endl;
	}

	glm::mat4 _projection2 = glm::perspective(60.0f, static_cast<float>(window._windowWidht)/window._windowHeight, 0.01f, 1000.0f);

	glUseProgram(rendContext.getProgramIndex());
	glUniformMatrix4fv(_projectionIndex, 1, GL_FALSE, reinterpret_cast<const float*>(&_projection2));
	glUseProgram(0u);


	glClearColor(0.2f, 0.4f, 0.8f, 1.0f);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	//glBlendEquation();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	HWND asd = window.CMWindowHandle();
	float i = 0;
	float x = 0;
	float z = -3;
	float y = -1;
	float v = 0;
	float h = 0;

	
	while (true)
	{
		wglMakeCurrent(GetDC(asd), rendContext.getRenderingContext());


	

		glUseProgram(rendContext.getProgramIndex());
		if (CML::CMInput::isKeyPressed(CML::CMInput::D)) x += 1.1f;
		if (CML::CMInput::isKeyPressed(CML::CMInput::A)) x += -1.1f;
		if (CML::CMInput::isKeyPressed(CML::CMInput::W)) z += 1.1f;
		if (CML::CMInput::isKeyPressed(CML::CMInput::S)) z += -1.1f;
		if (CML::CMInput::isKeyPressed(CML::CMInput::Up)) y += 1.1f;
		if (CML::CMInput::isKeyPressed(CML::CMInput::Down)) y += -1.1f;
		v = CML::CMInput::getMouseY(asd) - 450;
		h = CML::CMInput::getMouseX(asd) - 800;


		glm::mat4 _worldtransform = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f)) * glm::rotate(30.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 _cameraObscura = glm::translate(glm::vec3(x, y, z));// * glm::lookAt(glm::vec3(x, y, z), glm::vec3(v, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f))* glm::lookAt(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, h, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));


		//_projection2 *= glm::rotate(v, glm::vec3(1.0f, 0.0f, 0.0f));//* glm::rotate(h, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(_projectionIndex, 1, GL_FALSE, reinterpret_cast<const float*>(&_projection2));
		glUniformMatrix4fv(_worldIndex, 1, GL_FALSE, reinterpret_cast<const float*>(&_worldtransform));
		glUniformMatrix4fv(_cameraIndex, 1, GL_FALSE, reinterpret_cast<const float*>(&_cameraObscura));


		window.WindowMessageCheck();

		i++;

		
		


		glFlush();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glUseProgram(rendContext.getProgramIndex());
	


		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);

		glVertexAttribPointer(_positionIndex, 3, GL_FLOAT, GL_FALSE, 32, reinterpret_cast<GLvoid*>(0));

		glVertexAttribPointer(_colorIndex, 3, GL_FLOAT, GL_FALSE, 32, reinterpret_cast<GLvoid*>(12));

		glVertexAttribPointer(_textureIndex, 2, GL_FLOAT, GL_FALSE, 32, reinterpret_cast<GLvoid*>(24));


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);

		
		
		//glEnable(GL_STENCIL_TEST);
		//glStencilFunc(GL_ALWAYS, 1, 0xFF);
		//glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		//glStencilMask(0xFF);
		//glDepthMask(GL_FALSE);
		//glClear(GL_STENCIL_BUFFER_BIT);

		glBindTexture(GL_TEXTURE_2D, _texture[0]);


		_worldtransform = glm::scale(glm::vec3(1.0f, -1.0f, 1.0f));// *glm::rotate(30.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(_worldIndex, 1, GL_FALSE, reinterpret_cast<const float*>(&_worldtransform));
			
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>(0));


		//glStencilFunc(GL_EQUAL, 1, 0xFF);
		//glStencilMask(0x00);
		//glDepthMask(GL_TRUE);
	
		//glUniform1f(_alphaChannel, -0.5f);

		//glBindTexture(GL_TEXTURE_2D, _texture[0]);

		_worldtransform = glm::scale(glm::vec3(1.0f, -1.0f, 1.0f)) *glm::rotate(-i, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(_worldIndex, 1, GL_FALSE, reinterpret_cast<const float*>(&_worldtransform));

		glDrawElements(GL_TRIANGLES, 3 * indexSize, GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>(0));


		//glDisable(GL_STENCIL_TEST);

		//glUniform1f(_alphaChannel, 0.0f);
		

		//_worldtransform = glm::scale(glm::vec3(1.0f, 1.0f, 1.0f)) * glm::rotate(i, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(_worldIndex, 1, GL_FALSE, reinterpret_cast<const float*>(&_worldtransform));

		//glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>(0));


	
		

		

		


	

		glBindTexture(GL_TEXTURE_2D, 0u);

	




		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glUseProgram(0);

		SwapBuffers(rendContext.getHDC());//Bring back buffer to foreground


		if (CML::CMInput::isKeyPressed(CML::CMInput::Escape))
			break;
	}
		
	//poista kun debugger luokka valmis
	//system("PAUSE");
	return 0;
}
void Render(HWND windowh)
{
	
}