//--------------------------------------------------------------------------------------
// BTH - Stefan Petersson 2014.
//     - updated by FLL
//--------------------------------------------------------------------------------------
#include <windows.h>

#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include <vector>

#define IMGUI_IMPL_OPENGL_LOADER_GLEW

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "glew/include/GL/glew.h"
#include "glfw/include/GLFW/glfw3.h"

// read docs: https://github.com/g-truc/glm/blob/master/manual.md#section1
#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/ext.hpp"
#include <gl/GL.h>

#include "stb_image.h"

#include "GameTimer.h"
#include "Scene.h"
#include "Skybox.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "glfw3.lib")

#define _360_DEGREES 6.2831853072f //In radians

#define WIDTH 1280.0f
#define HEIGHT 720.0f
GLFWwindow *gWindow;

using namespace std;

void initWindow(unsigned int w, unsigned int h);


// OpenGL uses unsigned integers to keep track of
// created resources (shaders, vertices, textures, etc)
// For simplicity, we make them global here, but it is
// safe to put them in a class and pass around...
GLuint gShaderProgram = 0;

//Shader for animated models
GLuint gShaderProgramAnim = 0;

//Particle shader
GLuint gShaderProgramPS = 0;

//Skybox shader
GLuint gShaderProgramSkybox = 0;

//Blend-mapping shader
GLuint gShaderProgramBlend = 0;

//Compute shader program
GLuint gShaderProgramCompute = 0;

// full screen quad stuff
GLuint gVertexBufferFS = 0;
GLuint gVertexAttributeFS = 0;
GLuint gShaderProgramFS = 0;

//PF
GLuint gVertexBufferSM = 0;
GLuint gVertexAtrributeSM = 0;
GLuint gShaderProgramSM = 0;

float gClearColour[3] {};

//MVP-Matrix
GLint model_id = -1;
GLint model_id_anim = -1;
GLint model_id_skybox = -1;
GLint model_id_blend = -1;
GLint model_id_sm = -1;
GLint model_id_ps = -1;
glm::mat4 model_matrix;

GLint view_id = -1;
GLint view_id_anim = -1;
GLint view_id_skybox = -1;
GLint view_id_blend = -1;
GLint view_id_ps = -1;
glm::mat4 view_matrix;

GLint projection_id = -1;
GLint projection_id_anim = -1;
GLint projection_id_skybox = -1;
GLint projection_id_blend = -1;
GLint projection_id_ps = -1;
glm::mat4 projection_matrix;

GLuint shadow_id = -1; //PF
GLuint shadow_id2 = -1;
GLuint shadow_id3 = -1;
glm::mat4 shadow_matrix; //PF

//Camera variables
glm::vec3 camPos	= glm::vec3(0.5f, 0.5f, 8.0f); //Default camera position
glm::vec3 camFront	= glm::vec3(0.0f, 0.0f, -1.0f); //Default camera front
glm::vec3 camUp		= glm::vec3(0.0f, 1.0f, 0.0f); //Default camera up-vector
float FoV = 45.0f; //Field-of-view
float camYaw = -90.0f;
float camPitch = 0.0f;
float mouseLastX = WIDTH / 2; //At centre of the screen
float mouseLastY = HEIGHT / 2; //At centre of the screen

GameTimer timer;

// macro that returns "char*" with offset "i"
// BUFFER_OFFSET(5) transforms in "(char*)nullptr+(5)"
#define BUFFER_OFFSET(i) ((char *)nullptr + (i))

//PF
unsigned int depthMapFbo;
unsigned int depthMapAttachment[1];
const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;
int CreateFrameBufferSM() {
	int err = 0;

	glGenTextures(1, depthMapAttachment);
	glBindTexture(GL_TEXTURE_2D, depthMapAttachment[0]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	glGenFramebuffers(1, &depthMapFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapAttachment[0], 0);

	// check if framebuffer is complete (usable):
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		err = 0;
	}
	else
		err = -1;

	// bind default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return err;
}

unsigned int gFbo;
unsigned int gFboTextureAttachments[2]; // first for colour, second for depth
int CreateFrameBuffer() {
	int err = 0;
	// =================== COLOUR BUFFER =======================================
	// add "Attachments" to the framebuffer (textures to write to/read from)
	glGenTextures(2, gFboTextureAttachments);
	glBindTexture(GL_TEXTURE_2D, gFboTextureAttachments[0]);
	// define storage for texture
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	// define sampler settings
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// attach texture to framebuffer object

	// ===================== DEPTH BUFFER ==================================== 
	glBindTexture(GL_TEXTURE_2D, gFboTextureAttachments[1]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WIDTH, HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glGenFramebuffers(1, &gFbo);
	glBindFramebuffer(GL_FRAMEBUFFER, gFbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gFboTextureAttachments[0], 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, gFboTextureAttachments[1],0);

	// check if framebuffer is complete (usable):
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE)
	{
		err = 0; 
	} 
	else
		err = -1;

	// bind default framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return err;
}

void CreateShader(const string &fileName, GLuint &shader, const GLuint shaderType) {
	char buff[1024];
	memset(buff, 0, 1024);
	GLint compileResult = 0;

	shader = glCreateShader(shaderType);

	ifstream shaderFile(fileName);
	std::string shaderText((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
	shaderFile.close();

	const char* shaderTextPtr = shaderText.c_str();

	glShaderSource(shader, 1, &shaderTextPtr, nullptr);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
	if (compileResult == GL_FALSE) {

		glGetShaderInfoLog(shader, 1024, nullptr, buff);

		OutputDebugStringA(buff);
	}
}

//PF
void CreateSMShaders() {
	char buff[1024];
	memset(buff, 0, 1024);
	GLint compileResult = 0;

	GLuint vs = 0;
	GLuint fs = 0;

	CreateShader("VertexShaderSM.glsl", vs, GL_VERTEX_SHADER);
	CreateShader("FragmentSM.glsl", fs, GL_FRAGMENT_SHADER);

	gShaderProgramSM = glCreateProgram();
	glAttachShader(gShaderProgramSM, fs);
	glAttachShader(gShaderProgramSM, vs);
	glLinkProgram(gShaderProgramSM);

	compileResult = GL_FALSE;
	glGetProgramiv(gShaderProgramSM, GL_LINK_STATUS, &compileResult);
	if (compileResult == GL_FALSE) {

		memset(buff, 0, 1024);
		glGetProgramInfoLog(gShaderProgramSM, 1024, nullptr, buff);

		OutputDebugStringA(buff);
	}

	glDetachShader(gShaderProgramSM, vs);
	glDetachShader(gShaderProgramSM, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
}

void CreateBlendmapShaders() {
	// local buffer to store error strings when compiling.
	char buff[1024];
	memset(buff, 0, 1024);
	GLint compileResult = 0;

	GLuint vs = 0;
	GLuint fs = 0;
	GLuint gs = 0;

	CreateShader("VertexShaderBlend.glsl", vs, GL_VERTEX_SHADER);
	CreateShader("FragmentShaderBlend.glsl", fs, GL_FRAGMENT_SHADER);
	CreateShader("GeometryShaderBlend.glsl", gs, GL_GEOMETRY_SHADER);

	//link shader program (connect vs, gs and ps)
	gShaderProgramBlend = glCreateProgram();
	glAttachShader(gShaderProgramBlend, fs);
	glAttachShader(gShaderProgramBlend, vs);
	glAttachShader(gShaderProgramBlend, gs);
	glLinkProgram(gShaderProgramBlend);

	// check once more, if the Vertex Shader, Geometry Shader and the Fragment Shader can be used together
	compileResult = GL_FALSE;
	glGetProgramiv(gShaderProgramBlend, GL_LINK_STATUS, &compileResult);
	if (compileResult == GL_FALSE) {
		// query information about the compilation (nothing if compilation went fine!)
		memset(buff, 0, 1024);
		glGetProgramInfoLog(gShaderProgramBlend, 1024, nullptr, buff);
		// print to Visual Studio debug console output
		OutputDebugStringA(buff);
	}
	// in any case (compile sucess or not), we only want to keep the 
	// Program around, not the shaders.
	glDetachShader(gShaderProgramBlend, vs);
	glDetachShader(gShaderProgramBlend, fs);
	glDetachShader(gShaderProgramBlend, gs);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glDeleteShader(gs);
}

void CreateComputeShader() {
	// local buffer to store error strings when compiling.
	char buff[1024];
	memset(buff, 0, 1024);
	GLint compileResult = 0;

	GLuint cs = 0;

	CreateShader("ComputeShader.glsl", cs, GL_COMPUTE_SHADER);

	gShaderProgramCompute = glCreateProgram();
	glAttachShader(gShaderProgramCompute, cs);
	glLinkProgram(gShaderProgramCompute);

	// check once more, if the Vertex Shader and the Fragment Shader can be used
	// together
	compileResult = GL_FALSE;
	glGetProgramiv(gShaderProgramCompute, GL_LINK_STATUS, &compileResult);
	if (compileResult == GL_FALSE) {
		// query information about the compilation (nothing if compilation went fine!)
		memset(buff, 0, 1024);
		glGetProgramInfoLog(gShaderProgramCompute, 1024, nullptr, buff);
		// print to Visual Studio debug console output
		OutputDebugStringA(buff);
	}

	glDetachShader(gShaderProgramCompute, cs);
	glDeleteShader(cs);
}

void CreateFSShaders() {
	// local buffer to store error strings when compiling.
	char buff[1024]; 
	memset(buff, 0, 1024);
	GLint compileResult = 0;

	GLuint vs = 0;
	GLuint fs = 0;

	CreateShader("VertexShaderFS.glsl", vs, GL_VERTEX_SHADER);
	CreateShader("FragmentFS.glsl", fs, GL_FRAGMENT_SHADER);

	//link shader program (connect vs and ps)
	gShaderProgramFS = glCreateProgram();
	glAttachShader(gShaderProgramFS, fs);
	glAttachShader(gShaderProgramFS, vs);
	glLinkProgram(gShaderProgramFS);

	// check once more, if the Vertex Shader and the Fragment Shader can be used
	// together
	compileResult = GL_FALSE;
	glGetProgramiv(gShaderProgramFS, GL_LINK_STATUS, &compileResult);
	if (compileResult == GL_FALSE) {
		// query information about the compilation (nothing if compilation went fine!)
		memset(buff, 0, 1024);
		glGetProgramInfoLog(gShaderProgramFS, 1024, nullptr, buff);
		// print to Visual Studio debug console output
		OutputDebugStringA(buff);
	}
	// in any case (compile sucess or not), we only want to keep the 
	// Program around, not the shaders.
	glDetachShader(gShaderProgramFS, vs);
	glDetachShader(gShaderProgramFS, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
}

void CreateShaders() {
	// local buffer to store error strings when compiling.
	char buff[1024]; 
	memset(buff, 0, 1024);
	GLint compileResult = 0;

	GLuint vs = 0;
	GLuint fs = 0;
	GLuint gs = 0;

	CreateShader("VertexShader.glsl", vs, GL_VERTEX_SHADER);
	CreateShader("Fragment.glsl", fs, GL_FRAGMENT_SHADER);
	CreateShader("GeometryShader.glsl", gs, GL_GEOMETRY_SHADER);

	//link shader program (connect vs, gs and ps)
	gShaderProgram = glCreateProgram();
	glAttachShader(gShaderProgram, fs);
	glAttachShader(gShaderProgram, vs);
	glAttachShader(gShaderProgram, gs);
	glLinkProgram(gShaderProgram);

	// check once more, if the Vertex Shader, Geometry Shader and the Fragment Shader can be used together
	compileResult = GL_FALSE;
	glGetProgramiv(gShaderProgram, GL_LINK_STATUS, &compileResult);
	if (compileResult == GL_FALSE) {
		// query information about the compilation (nothing if compilation went fine!)
		memset(buff, 0, 1024);
		glGetProgramInfoLog(gShaderProgram, 1024, nullptr, buff);
		// print to Visual Studio debug console output
		OutputDebugStringA(buff);
	}
	// in any case (compile sucess or not), we only want to keep the 
	// Program around, not the shaders.
	glDetachShader(gShaderProgram, vs);
	glDetachShader(gShaderProgram, fs);
	glDetachShader(gShaderProgram, gs);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glDeleteShader(gs);
}

void CreateParticleShaders() {
	char buff[1024];
	memset(buff, 0, 1024);
	GLint compileResult = 0;

	GLuint vs = 0;
	GLuint fs = 0;
	GLuint gs = 0;

	CreateShader("VertexShaderPS.glsl", vs, GL_VERTEX_SHADER);
	CreateShader("FragmentShaderPS.glsl", fs, GL_FRAGMENT_SHADER);
	CreateShader("GeometryShaderPS.glsl", gs, GL_GEOMETRY_SHADER);

	gShaderProgramPS = glCreateProgram();
	glAttachShader(gShaderProgramPS, fs);
	glAttachShader(gShaderProgramPS, vs);
	glAttachShader(gShaderProgramPS, gs);
	glLinkProgram(gShaderProgramPS);

	compileResult = GL_FALSE;
	glGetProgramiv(gShaderProgramPS, GL_LINK_STATUS, &compileResult);
	if (compileResult == GL_FALSE) {
		// query information about the compilation (nothing if compilation went fine!)
		memset(buff, 0, 1024);
		glGetProgramInfoLog(gShaderProgramPS, 1024, nullptr, buff);
		// print to Visual Studio debug console output
		OutputDebugStringA(buff);
	}
	// in any case (compile sucess or not), we only want to keep the 
	// Program around, not the shaders.
	glDetachShader(gShaderProgramPS, vs);
	glDetachShader(gShaderProgramPS, fs);
	glDetachShader(gShaderProgramPS, gs);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glDeleteShader(gs);
}

void CreateSkyboxShaders() {
	// local buffer to store error strings when compiling.
	char buff[1024];
	memset(buff, 0, 1024);
	GLint compileResult = 0;

	GLuint vs = 0;
	GLuint fs = 0;

	CreateShader("VertexShaderSkybox.glsl", vs, GL_VERTEX_SHADER);
	CreateShader("FragmentShaderSkybox.glsl", fs, GL_FRAGMENT_SHADER);

	//link shader program (connect vs, gs and ps)
	gShaderProgramSkybox = glCreateProgram();
	glAttachShader(gShaderProgramSkybox, fs);
	glAttachShader(gShaderProgramSkybox, vs);
	glLinkProgram(gShaderProgramSkybox);

	// check once more, if the Vertex Shader, Geometry Shader and the Fragment Shader can be used together
	compileResult = GL_FALSE;
	glGetProgramiv(gShaderProgramSkybox, GL_LINK_STATUS, &compileResult);
	if (compileResult == GL_FALSE) {
		// query information about the compilation (nothing if compilation went fine!)
		memset(buff, 0, 1024);
		glGetProgramInfoLog(gShaderProgramSkybox, 1024, nullptr, buff);
		// print to Visual Studio debug console output
		OutputDebugStringA(buff);
	}
	// in any case (compile sucess or not), we only want to keep the 
	// Program around, not the shaders.
	glDetachShader(gShaderProgramSkybox, vs);
	glDetachShader(gShaderProgramSkybox, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
}

void CreateAnimShaders() {
	// local buffer to store error strings when compiling.
	char buff[1024];
	memset(buff, 0, 1024);
	GLint compileResult = 0;

	GLuint vs = 0;
	GLuint fs = 0;
	GLuint gs = 0;

	CreateShader("VertexShaderAnim.glsl", vs, GL_VERTEX_SHADER);
	CreateShader("FragmentAnim.glsl", fs, GL_FRAGMENT_SHADER);
	CreateShader("GeometryShaderAnim.glsl", gs, GL_GEOMETRY_SHADER);

	//link shader program (connect vs, gs and ps)
	gShaderProgramAnim = glCreateProgram();
	glAttachShader(gShaderProgramAnim, fs);
	glAttachShader(gShaderProgramAnim, vs);
	glAttachShader(gShaderProgramAnim, gs);
	glLinkProgram(gShaderProgramAnim);

	// check once more, if the Vertex Shader, Geometry Shader and the Fragment Shader can be used together
	compileResult = GL_FALSE;
	glGetProgramiv(gShaderProgramAnim, GL_LINK_STATUS, &compileResult);
	if (compileResult == GL_FALSE) {
		// query information about the compilation (nothing if compilation went fine!)
		memset(buff, 0, 1024);
		glGetProgramInfoLog(gShaderProgramAnim, 1024, nullptr, buff);
		// print to Visual Studio debug console output
		OutputDebugStringA(buff);
	}
	// in any case (compile sucess or not), we only want to keep the 
	// Program around, not the shaders.
	glDetachShader(gShaderProgramAnim, vs);
	glDetachShader(gShaderProgramAnim, fs);
	glDetachShader(gShaderProgramAnim, gs);
	glDeleteShader(vs);
	glDeleteShader(fs);
	glDeleteShader(gs);
}

void CreateFullScreenQuad() {
	struct Pos2UV {
		float x,y;
		float u,v;
	};
	Pos2UV myQuad[6] = {
		-1,-1, 0, 0,	// TOP		LEFT
		-1,+1, 0, 1,	// BOTTOM	LEFT
		+1,+1, 1, 1,	// BOTTOM	RIGHT
		-1,-1, 0, 0,	// TOP		LEFT
		+1,+1, 1, 1,	// BOTTOM	RIGHT
		+1,-1, 1, 0,	// TOP		RIGHT
	};

	// Vertex Array Object (VAO), description of the inputs to the GPU 
	glGenVertexArrays(1, &gVertexAttributeFS);
	// bind is like "enabling" the object to use it
	glBindVertexArray(gVertexAttributeFS);
	// this activates the first and second attributes of this VAO
	// think of "attributes" as inputs to the Vertex Shader
	glEnableVertexAttribArray(0); 
	glEnableVertexAttribArray(1);

	// create a vertex buffer object (VBO) id (out Array of Structs on the GPU side)
	glGenBuffers(1, &gVertexBufferFS);

	// Bind the buffer ID as an ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, gVertexBufferFS);

	// This "could" imply copying to the GPU, depending on what the driver wants to do, and
	// the last argument (read the docs!)
	glBufferData(GL_ARRAY_BUFFER, sizeof(myQuad), myQuad, GL_STATIC_DRAW);

	// tell OpenGL about layout in memory (input assembler information)
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Pos2UV), BUFFER_OFFSET(0));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Pos2UV), BUFFER_OFFSET(sizeof(float)*2));
};

GLuint CreateCubemapTexture(string faces[]) {
	GLuint texture = 0;
	glGenTextures(1, &texture); // Generate the texture
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture); //Bind trhe generated texture

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // First arg. is target, second is setting we want to edit & axis, last one is texture wrapping mode.
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	int width, height, colourChannels;

	for (int i = 0; i < 6; i++) {
		const char* filePath = faces[i].c_str(); //Path to image file
		cout << "Skybox texture path: " << filePath << endl; //Debug

		unsigned char* data = stbi_load(filePath, &width, &height, &colourChannels, 0);
		if (data) {
			// function args. in order | Target | Mipmap | Image format | Width | Height | Legacy, need to be 0 | Format | Datatype | Image data | 
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); //By increasing the enum we loop over all face orientations (Right, left, top, bottom, back, front)
		}
		else {
			cout << "Failed to load skybox texture nr: " << i << " Reason: " << stbi_failure_reason() << endl; //Output error message if texture file is not found
		}
		stbi_image_free(data); // Free image memory
	}
	return texture; //Returns an unsigned int/textureID
}

GLuint CreateTexture(string path) {
	GLuint texture = 0;
	glGenTextures(1, &texture); //Generate the texture, first input is amt of textures, second is where we store them.
	glBindTexture(GL_TEXTURE_2D, texture); // We bind the genereated texture to be able to change it.

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//	First arg. is target, this case a 2D texture, second is setting we want to edit and axis,
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);// last one is what texture wrapping mode we want to use. 

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);//Following two lines are for scaling setting it to use (bi)linear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, colourChannels;
	const char* filePath = path.c_str(); //Path to image file
	cout << "Texture path: " << filePath << endl; //Debug

	//The stbi_load funcions opens the file, and width and height values from the image and the amount of colour channels in the image.
	unsigned char* data = stbi_load(filePath, &width, &height, &colourChannels, 0); 
	if (data) {
		// function args. in order | Target | Mipmap | Image format | Width | Height | Legacy, need to be 0 | Format | Datatype | Image data | 
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D); // Generate the required mipmaps.
	}
	else {
		cout << "Failed to load texture. Reason: " << stbi_failure_reason() << endl; //Output error message if texture file is not found
	}
	stbi_image_free(data); // Free image memory
	return texture; //Returns an unsigned int/textureID
}

void CreateScene(Scene& scene) {
	//Fill the scene object with models to render
	scene.addModel("Resources/Models/ship.obj");
	scene.addModel("Resources/Models/cruiser.obj"); //Model borrowed from: http://www.prinmath.com/csci5229/OBJ/index.html
	scene.addModel("Resources/Models/cube.obj");

	scene.addBlendmapModel("Resources/Models/plane.obj");

	scene.addAnimatedModel("Resources/Models/anim_test2.dae");
	scene.addAnimatedModel("Resources/Models/model.dae");

	for (int i = 0; i < scene.getModelCount(); i++) {
		//calcTangentBasis(scene.models[i]->vertices, scene.models[i]->positions, scene.models[i]->uvs, scene.models[i]->normals, scene.models[i]->tangent, scene.models[i]->bitangent);
		scene.models[i]->setTextureID(CreateTexture(scene.models[i]->getTexturePath())); //Create texture
		scene.models[i]->setNormalID(CreateTexture(scene.models[i]->getNormalTexturePath()));
	}

	for (int i = 0; i < scene.getBlendmapModelCount(); i++) {
		scene.blendmapModels[i]->setTextureID(CreateTexture(scene.blendmapModels[i]->getTexturePath()));
		scene.blendmapModels[i]->setNormalID(CreateTexture(scene.blendmapModels[i]->getNormalTexturePath()));

		scene.blendmapModels[i]->blendmapPath = "Resources/Textures/basic_blendmap3.jpg";
		scene.blendmapModels[i]->rTexPath = "Resources/Textures/brickwall_normal.jpg"; //Test
		scene.blendmapModels[i]->gTexPath = "Resources/Textures/container.jpg"; //Test
		scene.blendmapModels[i]->bTexPath = "Resources/Textures/texture1.jpg"; //Test

		scene.blendmapModels[i]->blendTexID = CreateTexture(scene.blendmapModels[i]->blendmapPath);
		scene.blendmapModels[i]->rTexID = CreateTexture(scene.blendmapModels[i]->rTexPath);
		scene.blendmapModels[i]->gTexID = CreateTexture(scene.blendmapModels[i]->gTexPath);
		scene.blendmapModels[i]->bTexID = CreateTexture(scene.blendmapModels[i]->bTexPath);
	}

	for (int i = 0; i < scene.getAnimModelCount(); i++) {
		scene.animatedModels[i]->setTextureID(CreateTexture(scene.animatedModels[i]->getTexturePath())); //Create texture

		//Calling this function is vital to be able to render a model. Always call it before rendering!
		//If the model will only be rendered once, this can be called after creating it.
		scene.animatedModels[i]->prepare(gShaderProgramAnim);
	}

	//Add lights
	scene.addLight(glm::vec3(4.0, 6.0, 2.0), glm::vec3(1.0f, 0.82f, 0.73f));
	//scene.addLight(glm::vec3(-8.0, 6.0, 2.0), glm::vec3(1.0f, 0.0f, 0.0f)); //A red light

	//Skybox
	scene.skybox.textureID = CreateCubemapTexture(scene.skybox.faces);
	scene.skybox.prepareBuffers();
	scene.skybox.prepare(gShaderProgramSkybox);

	//Particle system
	scene.particleSystem.textureID = CreateTexture(scene.particleSystem.getTexPath());
	scene.particleSystem.prepareBuffers();
	scene.particleSystem.prepare(gShaderProgramPS);
}

void CreateMatrixData(GLuint shaderProg, GLint &projectionID, GLint &viewID) {
	projection_matrix = glm::perspective(glm::radians(FoV), WIDTH / HEIGHT, 0.1f, 100.0f);
	projectionID = glGetUniformLocation(shaderProg, "PROJ_MAT");
	if (projectionID == -1) {
		OutputDebugStringA("Error, cannot find 'projection_id' attribute in Geometry shader\n");
		return;
	}

	view_matrix = glm::lookAt(camPos, camPos + camFront, camUp); //Camera position, Looking direction and Up vector
	viewID = glGetUniformLocation(shaderProg, "VIEW_MAT");
	if (viewID == -1) {
		OutputDebugStringA("Error, cannot find 'view_id' attribute in Geometry shader\n");
		return;
	}
}

void CreateModelMatrix(float rotationValue, glm::vec3 translation, GLuint shaderProg, GLint &modelID) {
	glm::mat4 identity_mat = glm::mat4(1.0f);
	model_matrix = glm::translate(identity_mat, translation);
	model_matrix = glm::rotate(model_matrix, rotationValue, glm::vec3(0.0f, 1.0f, 0.0f));
	modelID = glGetUniformLocation(shaderProg, "MODEL_MAT");
	if (modelID == -1) {
		OutputDebugStringA("Error, cannot find 'model_id' attribute in Geometry shader\n");
		return;
	}
}

//PF
void CreateShadowMatrixData(glm::vec3 lightPos) {

	glm::mat4 depthProjectionMatrix = glm::ortho<float>( -3, 3, -3, 3, -1, 10); //An orthographic matrix
	glm::mat4 depthViewMatrix = glm::lookAt(lightPos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); //View from the light position towards origo
	glm::mat4 depthMVP = depthProjectionMatrix * depthViewMatrix;

	glm::mat4 shadowBias = glm::mat4(0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0);


	shadow_matrix = depthMVP * shadowBias;
	shadow_id = glGetUniformLocation(gShaderProgramSM, "SHADOW_MAT");
	if (shadow_id == -1) {
		OutputDebugStringA("Error, cannot find 'shadow_id' attribute in Vertex shader SM\n");
		return;
	}
	shadow_id2 = glGetUniformLocation(gShaderProgram, "SHADOW_MAT");
	if (shadow_id2 == -1) {
		OutputDebugStringA("Error, cannot find 'shadow_id2' attribute in Vertex shader\n");
		return;
	}
	shadow_id3 = glGetUniformLocation(gShaderProgramBlend, "SHADOW_MAT");
	if (shadow_id3 == -1) {
		OutputDebugStringA("Error, cannot find 'shadow_id3' attribute in Vertex shader Blend\n");
		return;
	}
}

void SetViewport() {
	// usually (not necessarily) this matches with the window size
	glViewport(0, 0, WIDTH, HEIGHT);
}

void PrePassRender(Scene& scene, float rotationVal) {
	glUseProgram(gShaderProgramSM);

	scene.models[1]->setWorldPosition(glm::vec3(5.0f, 1.0f, 0.0f));
	scene.models[2]->setWorldPosition(glm::vec3(-3.0f, 0.0f, -4.0f));
	scene.blendmapModels[0]->setWorldPosition(glm::vec3(0.0f, -1.0f, 0.0f));

	scene.models[0]->setWorldRotation(rotationVal);

	glUniformMatrix4fv(shadow_id, 1, GL_FALSE, glm::value_ptr(shadow_matrix));

	//Draws all static models in the scene
	for (int i = 0; i < scene.getModelCount(); i++) {
		//Send mvp matrix data per model
		CreateShadowMatrixData(scene.lightPositions[0]); //Creates the matrix every frame
		CreateModelMatrix(scene.models[i]->getWorldRotation(), scene.models[i]->getWorldPosition(), gShaderProgramSM, model_id_sm);
		glUniformMatrix4fv(model_id_sm, 1, GL_FALSE, glm::value_ptr(model_matrix));

		glBindVertexArray(scene.models[i]->vaoID); // bind is like "enabling" the object to use it
		glBindBuffer(GL_ARRAY_BUFFER, scene.models[i]->vboID); // Bind the buffer ID as an ARRAY_BUFFER
		glEnableVertexAttribArray(0);
		GLint vertexPos = glGetAttribLocation(gShaderProgramSM, "vertex_position");
		glVertexAttribPointer(vertexPos, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(0));

		glDrawArrays(GL_TRIANGLES, 0, scene.models[i]->getVertCount());
	}
	//Draws all blendmap models in the scene
	for (int i = 0; i < scene.getBlendmapModelCount(); i++) {
		//Send mvp matrix data per model
		CreateShadowMatrixData(scene.lightPositions[0]); //Creates the matrix every frame
		CreateModelMatrix(scene.blendmapModels[i]->getWorldRotation(), scene.blendmapModels[i]->getWorldPosition(), gShaderProgramSM, model_id_sm);  //Exchange rotation for "0.0f" to stop rotation
		glUniformMatrix4fv(model_id_sm, 1, GL_FALSE, glm::value_ptr(model_matrix));

		// tell opengl we are going to use the VAO we described earlier
		glBindVertexArray(scene.blendmapModels[i]->vaoID);
		glBindBuffer(GL_ARRAY_BUFFER, scene.blendmapModels[i]->vboID); // Bind the buffer ID as an ARRAY_BUFFER
		glEnableVertexAttribArray(0);
		GLint vertexPos = glGetAttribLocation(gShaderProgramSM, "vertex_position");
		glVertexAttribPointer(vertexPos, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, BUFFER_OFFSET(0));

		glDrawArrays(GL_TRIANGLES, 0, scene.blendmapModels[i]->getVertCount());
	}
}

void Render(Scene& scene, float rotationVal) {
	// set the color TO BE used (this does not clear the screen right away)
	glClearColor(gClearColour[0], gClearColour[1],gClearColour[2],1.0);
	// use the color to clear the color buffer (clear the color buffer only)
	glClear(GL_COLOR_BUFFER_BIT);

	//Choose model placement (default is origo)
	scene.models[1]->setWorldPosition(glm::vec3(5.0f, 1.0f, 0.0f));
	scene.models[2]->setWorldPosition(glm::vec3(-3.0f, 0.0f, -4.0f));
	scene.animatedModels[0]->setWorldPosition(glm::vec3(2.0f, 1.0f, -5.0f));
	scene.animatedModels[1]->setWorldPosition(glm::vec3(-6.0f, 2.0f, -2.5f));
	scene.blendmapModels[0]->setWorldPosition(glm::vec3(0.0f, -1.0f, 0.0f));
	

	//Chose model rotations (default is 0.0f)
	scene.models[0]->setWorldRotation(rotationVal);

	////////// Render static models //////////
	glUseProgram(gShaderProgram); //Choose a shader
	glUniformMatrix4fv(projection_id, 1, GL_FALSE, glm::value_ptr(projection_matrix));  //Sends data about projection-matrix to geometry-shader
	glUniformMatrix4fv(view_id, 1, GL_FALSE, glm::value_ptr(view_matrix));				//Sends data about view-matrix to geometry-shader
	glUniformMatrix4fv(shadow_id2, 1, GL_FALSE, glm::value_ptr(shadow_matrix));

	glUniform3fv(glGetUniformLocation(gShaderProgram, "light_positions"), scene.getLightCount(), glm::value_ptr(scene.lightPositions[0]));  //Sends light position data to fragment-shader
	glUniform3fv(glGetUniformLocation(gShaderProgram, "light_colors"), scene.getLightCount(), glm::value_ptr(scene.lightColors[0]));		//Sends light color data to fragment-shader

	//Draws all static models in the scene
	for (int i = 0; i < scene.getModelCount(); i++) {
		//Send model matrix data per model
		CreateModelMatrix(scene.models[i]->getWorldRotation(), scene.models[i]->getWorldPosition(), gShaderProgram, model_id);  //Exchange rotation for "0.0f" to stop rotation
		glUniformMatrix4fv(model_id, 1, GL_FALSE, glm::value_ptr(model_matrix)); //Sends data about model-matrix to geometry-shader
		scene.models[i]->prepare(gShaderProgram);

		//Send texture data
		glActiveTexture(GL_TEXTURE0); //Activate the texture unit
		glBindTexture(GL_TEXTURE_2D, scene.models[i]->getTextureID()); //Bind the texture
		glActiveTexture(GL_TEXTURE1); //Activate texture unit for normalmap
		glBindTexture(GL_TEXTURE_2D, scene.models[i]->getNormalID());
		glActiveTexture(GL_TEXTURE2); //PF
		glBindTexture(GL_TEXTURE_2D, depthMapAttachment[0]); //PF

		glUniform1i(glGetUniformLocation(gShaderProgram, "shadowMap"), 2); //PF

		glUniform3fv(scene.models[i]->ambID, 1, glm::value_ptr(scene.models[i]->ambientVal));		//Ambient
		glUniform3fv(scene.models[i]->diffID, 1, glm::value_ptr(scene.models[i]->diffuseVal));	//Diffuse
		glUniform3fv(scene.models[i]->specID, 1, glm::value_ptr(scene.models[i]->specularVal));	//Specular

		// tell opengl we are going to use the VAO we described earlier
		glBindVertexArray(scene.models[i]->vaoID);

		glDrawArrays(GL_TRIANGLES, 0, scene.models[i]->getVertCount());
	}
	glUseProgram(0); //Unbind program

	////////// Render animated models using another shader program //////////
	glUseProgram(gShaderProgramAnim); //Choose a shader
	glUniformMatrix4fv(projection_id_anim, 1, GL_FALSE, glm::value_ptr(projection_matrix));  //Sends data about projection-matrix to geometry-shader
	glUniformMatrix4fv(view_id_anim, 1, GL_FALSE, glm::value_ptr(view_matrix));				 //Sends data about view-matrix to geometry-shader

	glUniform3fv(glGetUniformLocation(gShaderProgramAnim, "light_positions"), scene.getLightCount(), glm::value_ptr(scene.lightPositions[0]));  //Sends light position data to fragment-shader
	glUniform3fv(glGetUniformLocation(gShaderProgramAnim, "light_colors"), scene.getLightCount(), glm::value_ptr(scene.lightColors[0]));		//Sends light color data to fragment-shader

	//Draws all animated models in the scene
	for (int i = 0; i < scene.getAnimModelCount(); i++) {
		CreateModelMatrix(scene.animatedModels[i]->getWorldRotation(), scene.animatedModels[i]->getWorldPosition(), gShaderProgramAnim, model_id_anim);
		glUniformMatrix4fv(model_id_anim, 1, GL_FALSE, glm::value_ptr(model_matrix));
		//scene.animatedModels[i]->prepare(gShaderProgramAnim);

		scene.animLoader.CalcJointTransform(timer.GetTimeSec(), scene.animatedModels[i]); //Calculate joint transforms
		for (int j = 0; j < scene.animatedModels[i]->jointCount; j++) {
			glUniformMatrix4fv(scene.animatedModels[i]->jointLocations[j], 1, GL_TRUE, (const GLfloat*)&scene.animatedModels[i]->jointTransforms[j]); //Send the matrices to the shader (the transpose is intentional)
		}

		//Send texture data
		glActiveTexture(GL_TEXTURE0); //Activate the texture unit
		glBindTexture(GL_TEXTURE_2D, scene.animatedModels[i]->getTextureID()); //Bind the texture

		glUniform3fv(scene.animatedModels[i]->ambID, 1, glm::value_ptr(scene.animatedModels[i]->ambientVal));	//Ambient
		glUniform3fv(scene.animatedModels[i]->diffID, 1, glm::value_ptr(scene.animatedModels[i]->diffuseVal));	//Diffuse
		glUniform3fv(scene.animatedModels[i]->specID, 1, glm::value_ptr(scene.animatedModels[i]->specularVal));	//Specular

		// tell opengl we are going to use the VAO we described earlier
		glBindVertexArray(scene.animatedModels[i]->vaoID);

		glDrawElements(GL_TRIANGLES, scene.animatedModels[i]->numIndices, GL_UNSIGNED_INT, 0);
	}
	glUseProgram(0); //Unbind program

	////////// Blend-mapped objects //////////
	glUseProgram(gShaderProgramBlend); //Choose a shader
	glUniformMatrix4fv(projection_id_blend, 1, GL_FALSE, glm::value_ptr(projection_matrix));  //Sends data about projection-matrix to geometry-shader
	glUniformMatrix4fv(view_id_blend, 1, GL_FALSE, glm::value_ptr(view_matrix));			  //Sends data about view-matrix to geometry-shader
	glUniformMatrix4fv(shadow_id3, 1, GL_FALSE, glm::value_ptr(shadow_matrix));

	glUniform3fv(glGetUniformLocation(gShaderProgramBlend, "light_positions"), scene.getLightCount(), glm::value_ptr(scene.lightPositions[0]));  //Sends light position data to fragment-shader
	glUniform3fv(glGetUniformLocation(gShaderProgramBlend, "light_colors"), scene.getLightCount(), glm::value_ptr(scene.lightColors[0]));		//Sends light color data to fragment-shader

	//Draws all static models in the scene
	for (int i = 0; i < scene.getBlendmapModelCount(); i++) {
		//Send model matrix data per model
		CreateModelMatrix(scene.blendmapModels[i]->getWorldRotation(), scene.blendmapModels[i]->getWorldPosition(), gShaderProgramBlend, model_id_blend);  //Exchange rotation for "0.0f" to stop rotation
		glUniformMatrix4fv(model_id_blend, 1, GL_FALSE, glm::value_ptr(model_matrix)); //Sends data about model-matrix to geometry-shader
		scene.blendmapModels[i]->prepare(gShaderProgramBlend);

		//Send texture data
		glActiveTexture(GL_TEXTURE0); //Activate the texture unit
		glBindTexture(GL_TEXTURE_2D, scene.blendmapModels[i]->getTextureID()); //Bind the texture
		glActiveTexture(GL_TEXTURE1); //Activate texture unit for normalmap
		glBindTexture(GL_TEXTURE_2D, scene.blendmapModels[i]->getNormalID());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, scene.blendmapModels[i]->blendTexID);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, scene.blendmapModels[i]->rTexID);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, scene.blendmapModels[i]->gTexID);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, scene.blendmapModels[i]->bTexID);
		glActiveTexture(GL_TEXTURE6); //PF
		glBindTexture(GL_TEXTURE_2D, depthMapAttachment[0]); //PF

		glUniform1i(glGetUniformLocation(gShaderProgramBlend, "blendmapSampler"), 2);
		glUniform1i(glGetUniformLocation(gShaderProgramBlend, "rTexSampler"), 3);
		glUniform1i(glGetUniformLocation(gShaderProgramBlend, "gTexSampler"), 4);
		glUniform1i(glGetUniformLocation(gShaderProgramBlend, "bTexSampler"), 5);
		glUniform1i(glGetUniformLocation(gShaderProgramBlend, "shadowMap"), 6); //PF

		glUniform3fv(scene.blendmapModels[i]->ambID, 1, glm::value_ptr(scene.blendmapModels[i]->ambientVal));	//Ambient
		glUniform3fv(scene.blendmapModels[i]->diffID, 1, glm::value_ptr(scene.blendmapModels[i]->diffuseVal));	//Diffuse
		glUniform3fv(scene.blendmapModels[i]->specID, 1, glm::value_ptr(scene.blendmapModels[i]->specularVal));	//Specular

		// tell opengl we are going to use the VAO we described earlier
		glBindVertexArray(scene.blendmapModels[i]->vaoID);

		glDrawArrays(GL_TRIANGLES, 0, scene.blendmapModels[i]->getVertCount());
	}
	glUseProgram(0); //Unbind program

	////////// Skybox ////////// (Rendered last (except for particles)! This is more efficient)
	glUseProgram(gShaderProgramSkybox);
	glDepthFunc(GL_LEQUAL); //Passes the depth test with values less than or equal to the depth buffer

	glUniformMatrix4fv(projection_id_skybox, 1, GL_FALSE, glm::value_ptr(projection_matrix));  //Sends data about projection-matrix to vertex-shader
	glm::mat4 view = glm::mat3(view_matrix); //Modified view-matrix
	glUniformMatrix4fv(view_id_skybox, 1, GL_FALSE, glm::value_ptr(view)); //Sends data about view-matrix to vertex-shader
	//scene.skybox.prepare(gShaderProgramSkybox);

	glActiveTexture(GL_TEXTURE0); //Activate the texture unit
	glBindTexture(GL_TEXTURE_CUBE_MAP, scene.skybox.textureID); //Bind the texture

	glBindVertexArray(scene.skybox.vaoID);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDepthFunc(GL_LESS); //Return to default
	glUseProgram(0); //Unbind program

	/////////// Compute shader //////////
	glUseProgram(gShaderProgramCompute);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, scene.particleSystem.vboID);
	glUniform1f(glGetUniformLocation(gShaderProgramCompute, "dt"), timer.GetDeltaTime()); //Send delta time
	glUniform1f(glGetUniformLocation(gShaderProgramCompute, "max_lifetime"), MAX_LIFETIME); //Send the max lifetime of particles
	glDispatchCompute(scene.particleSystem.getCount(), 1, 1);
	//cout << sizeof(scene.particleSystem.particles) << endl; // Debug
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glUseProgram(0);

	////////// Particle System  //////////
	glUseProgram(gShaderProgramPS);
	glEnable(GL_BLEND); //For transparency
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	CreateModelMatrix(0.0f, glm::vec3(0.0, 5.0, 0.0), gShaderProgramPS, model_id_ps);
	glUniformMatrix4fv(projection_id_ps, 1, GL_FALSE, glm::value_ptr(projection_matrix)); //Sends data about projection-matrix
	glUniformMatrix4fv(view_id_ps, 1, GL_FALSE, glm::value_ptr(view_matrix));			  //Sends data about view-matrix
	glUniformMatrix4fv(model_id_ps, 1, GL_FALSE, glm::value_ptr(model_matrix));			  //Sends data about model-matrix

	scene.particleSystem.update(timer.GetDeltaTime());

	glActiveTexture(GL_TEXTURE0); //Activate the texture unit
	glBindTexture(GL_TEXTURE_2D, scene.particleSystem.textureID);

	glBindVertexArray(scene.particleSystem.vaoID);
	glDrawArrays(GL_POINTS, 0, scene.particleSystem.getCount());

	glDisable(GL_BLEND);
	glUseProgram(0);
}

void keyboardUpdate() {
	float movementSpeed = 7.5f * timer.GetDeltaTime();
	if (GetAsyncKeyState(GLFW_KEY_W)) //GetAsyncKeyState Determines whether a key is up or down at the time of the function call.
		camPos += movementSpeed * camFront;

	if (GetAsyncKeyState(GLFW_KEY_A))
		camPos -= glm::normalize(glm::cross(camFront, camUp)) * movementSpeed;

	if (GetAsyncKeyState(GLFW_KEY_S))
		camPos -= movementSpeed * camFront;

	if (GetAsyncKeyState(GLFW_KEY_D))
		camPos += glm::normalize(glm::cross(camFront, camUp)) * movementSpeed;
}

//This function uses output from the mouse wheel
void scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
	FoV = FoV - 5 * yOffset;
	if (FoV <= 10.0f) {
		FoV = 10.0f;
	}
	if (FoV >= 100.0f) {
		FoV = 100.0f;
	}
	cout << "Current FoV: " << FoV << endl;
}

void mouseCallback(GLFWwindow* window, double xPos, double yPos) {
	float mouseSpeed = 0.05f;

	float xOffset = xPos - mouseLastX;
	float yOffset = mouseLastY - yPos;
	mouseLastX = xPos;
	mouseLastY = yPos;

	xOffset *= mouseSpeed;
	yOffset *= mouseSpeed;

	camYaw += xOffset;
	camPitch += yOffset;

	//Constraining camera movement to avoid flipping
	if (camPitch >= 89.0f) {
		camPitch = 89.0f;;
	}
	if (camPitch <= -89.0f) {
		camPitch = -89.0f;
	}

	//Calculating direction vector
	glm::vec3 direction = glm::vec3(
		cos(glm::radians(camPitch)) * cos(glm::radians(camYaw)),
		sin(glm::radians(camPitch)),
		cos(glm::radians(camPitch)) * sin(glm::radians(camYaw)));
	camFront = glm::normalize(direction);

	//This provides feedback to console for mouse movement
	//cout << "X-Position: " << xPos << endl;
	//cout << "Y-Position: " << yPos << endl;
	//cout << endl;
}

//This function specifies the layout of debug messages
void APIENTRY glDebugOutput(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam) {
	//Take out 131185 for example to test debug messages
	if (id == 131169 || id == 131185 || id == 131218 || id == 131204) { //Insignificant errors/notifications
		return;
	}

	freopen("CON", "w", stdout); //Redirects the string stream to the debug console

	cout << "---------------" << endl;
	cout << "Debug message (" << id << "): " << message << endl;

	switch (source) {
	case GL_DEBUG_SOURCE_API:             cout << "Source: API"; break;
	case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   cout << "Source: Window System"; break;
	case GL_DEBUG_SOURCE_SHADER_COMPILER: cout << "Source: Shader Compiler"; break;
	case GL_DEBUG_SOURCE_THIRD_PARTY:     cout << "Source: Third Party"; break;
	case GL_DEBUG_SOURCE_APPLICATION:     cout << "Source: Application"; break;
	case GL_DEBUG_SOURCE_OTHER:           cout << "Source: Other"; break;
	} cout << endl;

	switch (type) {
	case GL_DEBUG_TYPE_ERROR:               cout << "Type: Error"; break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: cout << "Type: Deprecated Behaviour"; break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  cout << "Type: Undefined Behaviour"; break;
	case GL_DEBUG_TYPE_PORTABILITY:         cout << "Type: Portability"; break;
	case GL_DEBUG_TYPE_PERFORMANCE:         cout << "Type: Performance"; break;
	case GL_DEBUG_TYPE_MARKER:              cout << "Type: Marker"; break;
	case GL_DEBUG_TYPE_PUSH_GROUP:          cout << "Type: Push Group"; break;
	case GL_DEBUG_TYPE_POP_GROUP:           cout << "Type: Pop Group"; break;
	case GL_DEBUG_TYPE_OTHER:               cout << "Type: Other"; break;
	} cout << endl;

	switch (severity) {
	case GL_DEBUG_SEVERITY_HIGH:         cout << "Severity: high"; break;
	case GL_DEBUG_SEVERITY_MEDIUM:       cout << "Severity: medium"; break;
	case GL_DEBUG_SEVERITY_LOW:          cout << "Severity: low"; break;
	case GL_DEBUG_SEVERITY_NOTIFICATION: cout << "Severity: notification"; break;
	} cout << endl;
	cout << endl;
}

int WINAPI wWinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow ) {
	//Attaches a console used for displaying debug messages to the application
	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen("CON", "w", stdout); //Redirects the string stream to the debug console
	cout << "Welcome to the debug console!" << endl;

	initWindow(WIDTH, HEIGHT);
	bool shutdown = false;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	ImGui_ImplGlfw_InitForOpenGL(gWindow, true);
	ImGui_ImplOpenGL3_Init("#version 130");

	ImGui::StyleColorsDark();

	//Enable OpenGL debug context if context allows for debug context
	GLint flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); //Makes sure errors are displayed synchronously
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}

	CreateShaders(); //5. Create vertex- and fragment-shaders
	CreateBlendmapShaders();
	CreateAnimShaders(); //5. Create shaders for animated models
	CreateSkyboxShaders(); //5.
	CreateParticleShaders(); //5.
	CreateFSShaders(); //5. Create vertex- and fragment-shaders
	CreateSMShaders(); //PF
	CreateComputeShader();

	if (CreateFrameBuffer() != 0)
		shutdown = true;

	if (CreateFrameBufferSM() != 0)
		shutdown = true;

	//Create a scene object and fill it
	Scene gameScene;
	CreateScene(gameScene);

	CreateFullScreenQuad();

	float rotation = 0.0f;

	while (!glfwWindowShouldClose(gWindow)) {
		glfwPollEvents();
		if (GLFW_PRESS == glfwGetKey(gWindow, GLFW_KEY_ESCAPE)) {
			glfwSetWindowShouldClose(gWindow, 1);
		}


		//Calculate delta time
		timer.Tick();

		float increment = 0.6f * timer.GetDeltaTime();

		if (rotation >= _360_DEGREES) {
			rotation -= _360_DEGREES;
		}

		rotation += increment;

		keyboardUpdate();

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT); //Set the viewport to the same resolution as the framebuffer to be able to render shadows correctly
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFbo); //PF
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f); //PF
		glClear(GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);
		PrePassRender(gameScene, rotation);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// first pass
		// render all geometry to a framebuffer object
		glViewport(0, 0, WIDTH, HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, gFbo);
		glClearColor(gClearColour[0], gClearColour[1], gClearColour[2], gClearColour[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Prepare IMGUI output
		ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
		ImGui::Begin("Debug");                          // Create a window called "Debug" and append into it.
		ImGui::Text("DV1568 3D-Programming");           // Display some text (you can use a format strings too)   
		ImGui::ColorEdit3("clear color", gClearColour); // Edit 3 floats representing a color
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::Text("Particles in scene: %i", gameScene.particleSystem.getCount());
		static float scale = 1.0f;
		ImGui::SliderFloat("Scale", &scale, 0.0f, 1.0f);
		static bool renderDepth = false;
		ImGui::Checkbox("Show DepthMap", &renderDepth);
		ImGui::End();

		CreateMatrixData(gShaderProgram, projection_id, view_id); //Creates vp-matrices for standard shader
		CreateMatrixData(gShaderProgramBlend, projection_id_blend, view_id_blend); //Creates vp-matrices for blendmap shader
		CreateMatrixData(gShaderProgramAnim, projection_id_anim, view_id_anim); //Creates vp-matrices for animated model-shader
		CreateMatrixData(gShaderProgramSkybox, projection_id_skybox, view_id_skybox); // Creates vp-matrices for skybox
		CreateMatrixData(gShaderProgramPS, projection_id_ps, view_id_ps); // Creates vp-matrices for particle system

		Render(gameScene, rotation); //9. Render

		// first pass is done!
		// now render a second pass
		// bind default framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(gShaderProgramFS);
		glBindVertexArray(gVertexAttributeFS);

		glDisable(GL_DEPTH_TEST);

		// bind texture drawn in the first pass!
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, gFboTextureAttachments[0]);
		glActiveTexture(GL_TEXTURE0 + 1);
		glBindTexture(GL_TEXTURE_2D, gFboTextureAttachments[1]);
		
		glm::mat4 scaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));
		glm::mat4 transform = scaleMat;
		glUniformMatrix4fv(5, 1, GL_TRUE, &transform[0][0]);

		// false
		glUniform1i(3, renderDepth);  // 0 == false
		
		glDrawArrays(GL_TRIANGLES, 0, 6); //Draw quad

		//Render IMGUI interface
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(gWindow); //10. Växla front- och back-buffer
	}

	// SHUTDOWN
	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

	glDeleteFramebuffers(1, &gFbo);
	glDeleteTextures(2, gFboTextureAttachments);
	glDeleteFramebuffers(1, &depthMapFbo); //PF
	glDeleteTextures(1, depthMapAttachment); //PF
	glDeleteVertexArrays(1, &gVertexAttributeFS);
	glDeleteBuffers(1, &gVertexBufferFS);
	glfwTerminate();
	glDeleteProgram(gShaderProgram);
	glDeleteProgram(gShaderProgramAnim);
	glDeleteProgram(gShaderProgramBlend);
	glDeleteProgram(gShaderProgramFS);
	glDeleteProgram(gShaderProgramPS);
	glDeleteProgram(gShaderProgramSkybox);
	glDeleteProgram(gShaderProgramSM);

	return 0;
}

void initWindow(unsigned int w, unsigned int h) {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE); //Allows for Debug output (should generally be commented out in a release build)
	//glfwWindowHint(GLFW_SAMPLES, 4);

	gWindow = glfwCreateWindow(w, h, "DV1568 3D-Programming Project", NULL, NULL);
	if (!gWindow) {
		fprintf(stderr, "error creating window\n");
		exit(-1);
	}
	glfwMakeContextCurrent(gWindow);
	glewExperimental = GL_TRUE;

	glfwSetInputMode(gWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); //Disables mouse cursor (Press ESCAPE to quit application)
	glfwSetCursorPos(gWindow, mouseLastX, mouseLastY); //Initializes cursor position to the middle of the screen
	glfwSetScrollCallback(gWindow, scrollCallback); //Scroll-wheel callback
	glfwSetCursorPosCallback(gWindow, mouseCallback); //Mouse callback

	glfwSwapInterval(1);

	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "Error GLEW: %s\n", glewGetErrorString(err));
	}
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* version = glGetString(GL_VERSION);
	fprintf(stderr, "Renderer: %s\n", renderer);
	fprintf(stderr, "OpenGL version %s\n", version);

	glClearColor(0.9f, 0.2f, 0.2f, 0.0f);
	glClearDepth(1.0f);
	glViewport(0, 0, w, h);

	return;
}

/*
void calcTangentBasis(
	//in:
	vector<RawModel::TriangleVertex> vertices,
	vector<glm::vec3> positions,
	vector<glm::vec2> uvs,
	vector<glm::vec3> normals,
	//out
	vector<glm::vec3> & tangents,
	vector<glm::vec3> & bitangents
) 
{
	//Calculate edges and deltaUVs based on amount of triangles
	for (int i = 0; i < vertices.size(); i + 3) {
		// Shortcuts for verts
		glm::vec3 & v0 = positions[i + 0];
		glm::vec3 & v1 = positions[i + 1];
		glm::vec3 & v2 = positions[i + 2];
		// Shortcuts for UVs
		glm::vec2 & uv0 = uvs[i + 0];
		glm::vec2 & uv1 = uvs[i + 1];
		glm::vec2 & uv2 = uvs[i + 2];
		//Edges of the triangle : position delta
		glm::vec3 deltaPos1 = v1 - v0;
		glm::vec3 deltaPos2 = v2 - v0;
		//UV delta
		glm::vec2 deltaUV1 = uv1 - uv0;
		glm::vec2 deltaUV2 = uv2 - uv0;

		//Use formula as seen on: http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/
		float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
		glm::vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y)*r;
		glm::vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x)*r;

		//Set the same tangent and bitangent to all three vertices of the triangle.
		tangents.push_back(tangent);
		tangents.push_back(tangent);
		tangents.push_back(tangent);

		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);
		bitangents.push_back(bitangent);

	}
	cout << "Tangent and bitangent calculated for a Triangle" << endl;
}
*/


