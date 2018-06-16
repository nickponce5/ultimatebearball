/*
CPE/CSC 471 Lab base code Wood/Dunn/Eckhardt
*/

#include <iostream>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "GLSL.h"
#include "Program.h"
#include "MatrixStack.h"

#include "WindowManager.h"
#include "Shape.h"
// value_ptr for glm
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GRAVITY -9.80665

using namespace std;
using namespace glm;

shared_ptr<Shape> shape1;
shared_ptr<Shape> shape2;
shared_ptr<Shape> shape3;
shared_ptr<Shape> shape4;
shared_ptr<Shape> shape5;

// Oriented Bounding Box
typedef struct {
	glm::vec3 bounds[8];
	glm::vec3 max;
	glm::vec3 min;
} OBB;

// Bounding Sphere
typedef struct {
	float radius;
	glm::vec3 center;
} BS;

double get_last_elapsed_time()
{
	static double lasttime = glfwGetTime();
	double actualtime = glfwGetTime();
	double difference = actualtime- lasttime;
	lasttime = actualtime;
	return difference;
}
class camera
{
public:
	glm::vec3 pos, rot;
	int w, a, s, d, q, e, i, j, k, l;
	camera()
	{
		w = a = s = d = q = e = i = j = k = l = 0;
		rot = glm::vec3(0, 0, 0);
		pos = glm::vec3(0, 0, -10);
	}
	glm::mat4 process(double ftime)
	{
		float zspeed, xspeed, yspeed;
		xspeed = yspeed = zspeed = 0;
		if (q == 1)
		{
			zspeed = 5*ftime;
		}
		else if (e == 1)
		{
			zspeed = -5*ftime;
		}
		if (k == 1)
		{
			yspeed = 5*ftime;
		}
		else if (i == 1)
		{
			yspeed = -5*ftime;
		}
		if (j == 1)
		{
			xspeed = 5*ftime;
		}
		else if (l == 1)
		{
			xspeed = -5*ftime;
		}
		float yangle, xangle;
		yangle = xangle = 0;
		if (a == 1)
			yangle = -ftime;
		else if(d==1)
			yangle = ftime;
		rot.y += yangle;
		if (w == 1)
			xangle = -ftime;
		else if(s==1)
			xangle = ftime;
		rot.x += xangle;
		glm::mat4 Ry = glm::rotate(glm::mat4(1), rot.y, glm::vec3(0, 1, 0));
		glm::mat4 Rx = glm::rotate(glm::mat4(1), rot.x, glm::vec3(1, 0, 0));
		glm::vec4 dir = glm::vec4(xspeed, yspeed, zspeed,1);
		dir = dir * Ry * Rx;
		pos += glm::vec3(dir.x, dir.y, dir.z);
		glm::mat4 T = glm::translate(glm::mat4(1), pos);
		return Ry * Rx * T;
	}
};

camera mycam;
float stagefwd = 0.0, stageback = 0.0, stageleft = 0.0, stageright = 0.0;

class Application : public EventCallbacks
{

public:

	WindowManager * windowManager = nullptr;

	// Shader Programs
	std::shared_ptr<Program> stageProgram;
	std::shared_ptr<Program> ballProgram;
	std::shared_ptr<Program> bearProgram;
	std::shared_ptr<Program> giftNubProgram;

	// VAO IDs
	GLuint VertexArrayID[1];

	// VBO IDs
	GLuint VertexBufferIDs[3];

	// Texture IDs
	GLuint Texture[1];

	// Bounding Box Data
	OBB cubeBound;

	void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		if (key == GLFW_KEY_W && action == GLFW_PRESS)
		{
			mycam.w = 1;
		}
		if (key == GLFW_KEY_W && action == GLFW_RELEASE)
		{
			mycam.w = 0;
		}
		if (key == GLFW_KEY_S && action == GLFW_PRESS)
		{
			mycam.s = 1;
		}
		if (key == GLFW_KEY_S && action == GLFW_RELEASE)
		{
			mycam.s = 0;
		}
		if (key == GLFW_KEY_A && action == GLFW_PRESS)
		{
			mycam.a = 1;
		}
		if (key == GLFW_KEY_A && action == GLFW_RELEASE)
		{
			mycam.a = 0;
		}
		if (key == GLFW_KEY_D && action == GLFW_PRESS)
		{
			mycam.d = 1;
		}
		if (key == GLFW_KEY_D && action == GLFW_RELEASE)
		{
			mycam.d = 0;
		}
		if (key == GLFW_KEY_Q && action == GLFW_PRESS)
		{
			mycam.q = 1;
		}
		if (key == GLFW_KEY_Q && action == GLFW_RELEASE)
		{
			mycam.q = 0;
		}
		if (key == GLFW_KEY_E && action == GLFW_PRESS)
		{
			mycam.e = 1;
		}
		if (key == GLFW_KEY_E && action == GLFW_RELEASE)
		{
			mycam.e = 0;
		}
		if (key == GLFW_KEY_I && action == GLFW_PRESS)
		{
			mycam.i = 1;
		}
		if (key == GLFW_KEY_I && action == GLFW_RELEASE)
		{
			mycam.i = 0;
		}
		if (key == GLFW_KEY_K && action == GLFW_PRESS)
		{
			mycam.k = 1;
		}
		if (key == GLFW_KEY_K && action == GLFW_RELEASE)
		{
			mycam.k = 0;
		}
		if (key == GLFW_KEY_J && action == GLFW_PRESS)
		{
			mycam.j = 1;
		}
		if (key == GLFW_KEY_J && action == GLFW_RELEASE)
		{
			mycam.j = 0;
		}
		if (key == GLFW_KEY_L && action == GLFW_PRESS)
		{
			mycam.l = 1;
		}
		if (key == GLFW_KEY_L && action == GLFW_RELEASE)
		{
			mycam.l = 0;
		}
		if (key == GLFW_KEY_T && action == GLFW_PRESS)
		{
			stagefwd = 1;
		}
		if (key == GLFW_KEY_T && action == GLFW_RELEASE)
		{
			stagefwd = 0;
		}
		if (key == GLFW_KEY_G && action == GLFW_PRESS)
		{
			stageback = 1;
		}
		if (key == GLFW_KEY_G && action == GLFW_RELEASE)
		{
			stageback = 0;
		}
		if (key == GLFW_KEY_F && action == GLFW_PRESS)
		{
			stageleft = 1;
		}
		if (key == GLFW_KEY_F && action == GLFW_RELEASE)
		{
			stageleft = 0;
		}
		if (key == GLFW_KEY_H && action == GLFW_PRESS)
		{
			stageright = 1;
		}
		if (key == GLFW_KEY_H && action == GLFW_RELEASE)
		{
			stageright = 0;
		}
	}

	// callback for the mouse when clicked move the triangle when helper functions
	// written
	void mouseCallback(GLFWwindow *window, int button, int action, int mods)
	{
		double posX, posY;
		if (action == GLFW_PRESS)
		{
			glfwGetCursorPos(window, &posX, &posY);
			std::cout << "Pos X " << posX <<  " Pos Y " << posY << std::endl;
		}
	}

	//if the window is resized, capture the new size and reset the viewport
	void resizeCallback(GLFWwindow *window, int in_width, int in_height)
	{
		//get the window size - may be different then pixels for retina
		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
	}

	/* Gets the bounds for the cube */
	OBB getCubeBounds(GLfloat cubeVerts[24])
	{
		OBB b;
		for(int i = 0; i < 8; i++) {
			b.bounds[i].x = cubeVerts[i * 3 + 0];
			b.bounds[i].y = cubeVerts[i * 3 + 1];
			b.bounds[i].z = cubeVerts[i * 3 + 2];
		}

		b.min = b.bounds[4];
		b.max = b.bounds[2];

		return b;
	}

	/* Note that any gl calls must always happen after a GL state is initialized */
	void initGeom()
	{
		// Create VAO0
		glGenVertexArrays(1, &VertexArrayID[0]);
		glBindVertexArray(VertexArrayID[0]);

		// Create VBO0: Rectangle
			glGenBuffers(1, &VertexBufferIDs[0]);
			glBindBuffer(GL_ARRAY_BUFFER, VertexBufferIDs[0]);

			// Cube Position Buffer
			GLfloat cube_vertices[] = {
				// front
				-1.0, -1.0,  1.0,
				1.0, -1.0,  1.0,
				1.0,  1.0,  1.0,
				-1.0,  1.0,  1.0,
				// back
				-1.0, -1.0, -1.0,
				1.0, -1.0, -1.0,
				1.0,  1.0, -1.0,
				-1.0,  1.0, -1.0,
			};

			cubeBound = getCubeBounds(cube_vertices);

			// Make the cube a bit smaller
			// for (int i = 0; i < 24; i++)
			// 	cube_vertices[i] *= 0.5;
			glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
		// End VBO0 Section

		// Create VBO1
			glGenBuffers(1, &VertexBufferIDs[1]);
			glBindBuffer(GL_ARRAY_BUFFER, VertexBufferIDs[1]);

			// Cube Color Buffer
			GLfloat cube_colors[] = {
				// front colors
				1.0, 0.0, 0.0,
				0.0, 1.0, 0.0,
				0.0, 0.0, 1.0,
				1.0, 1.0, 1.0,
				// back colors
				1.0, 0.0, 0.0,
				0.0, 1.0, 0.0,
				0.0, 0.0, 1.0,
				1.0, 1.0, 1.0,
			};

			glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		// End VBO1 Section

		// Create VBO2
			glGenBuffers(1, &VertexBufferIDs[2]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VertexBufferIDs[2]);

			GLushort cube_elements[] = {
				// front
				0, 1, 2,
				2, 3, 0,
				// top
				1, 5, 6,
				6, 2, 1,
				// back
				7, 6, 5,
				5, 4, 7,
				// bottom
				4, 0, 3,
				3, 7, 4,
				// left
				4, 5, 1,
				1, 0, 4,
				// right
				3, 2, 6,
				6, 7, 3,
			};

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		// End VBO2 Section

		// De-"select" the VAO
		glBindVertexArray(0);
	// End VAO0 Section

		string resourceDirectory = "../resources" ;
		// Initialize mesh for all shapes.
		shape1 = make_shared<Shape>();
		shape1->loadMesh(resourceDirectory + "/unitsphere.obj");
		shape1->resize();
		shape1->init();

		shape2 = make_shared<Shape>();
		shape2->loadMesh(resourceDirectory + "/txsphere.obj");
		shape2->resize();
		shape2->init();

		shape3 = make_shared<Shape>();
		shape3->loadMesh(resourceDirectory + "/cylinder.obj");
		shape3->resize();
		shape3->init();

		shape4 = make_shared<Shape>();
		shape4->loadMesh(resourceDirectory + "/arch.obj");
		shape4->resize();
		shape4->init();

		shape5 = make_shared<Shape>();
		shape5->loadMesh(resourceDirectory + "/gift.obj");
		shape5->resize();
		shape5->init();

	// Texture Uniform Setting and Binding
		int width, height, channels;
		char filepath[1000];

	// Texture 0: Checkered Pattern
		string str = resourceDirectory + "/checkers.jpg";
		strcpy(filepath, str.c_str());
		unsigned char* data = stbi_load(filepath, &width, &height, &channels, 4);
		glGenTextures(1, &Texture[0]);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture[0]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		GLuint Tex1Location[1];
		Tex1Location[0] = glGetUniformLocation(ballProgram->pid, "checkerTex");

		// Uniform Texture Binding
		glUseProgram(ballProgram->pid);
		glUniform1i(Tex1Location[0], 0);
	}

	//General OGL initialization - set OGL state here
	void init(const std::string& resourceDirectory)
	{
		GLSL::checkVersion();

		// Set background color.
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		// Enable blending for transparency
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// Initialize the GLSL programs.
		
		// Ball Shader Program:
		ballProgram = std::make_shared<Program>();
		ballProgram->setVerbose(true);
		ballProgram->setShaderNames(resourceDirectory + "/ball_shader_vertex.glsl", resourceDirectory + "/ball_shader_fragment.glsl");
		if (!ballProgram->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
		ballProgram->addUniform("P");
		ballProgram->addUniform("V");
		ballProgram->addUniform("M");
		ballProgram->addUniform("semiSphere");
		ballProgram->addUniform("maxDistFromSemi");
		ballProgram->addAttribute("vertPos");

		// Bear Shader Program:
		bearProgram = std::make_shared<Program>();
		bearProgram->setVerbose(true);
		bearProgram->setShaderNames(resourceDirectory + "/bear_shader_vertex.glsl", resourceDirectory + "/bear_shader_fragment.glsl");
		if (!bearProgram->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
		bearProgram->addUniform("P");
		bearProgram->addUniform("V");
		bearProgram->addUniform("M");
		bearProgram->addAttribute("vertPos");
		bearProgram->addAttribute("vertNor");
		bearProgram->addAttribute("vertTex");

		// Stage Shader Program:
		stageProgram = std::make_shared<Program>();
		stageProgram->setVerbose(true);
		stageProgram->setShaderNames(resourceDirectory + "/stage_shader_vertex.glsl", resourceDirectory + "/stage_shader_fragment.glsl");
		if (!stageProgram->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
		stageProgram->addUniform("P");
		stageProgram->addUniform("V");
		stageProgram->addUniform("M");
		stageProgram->addAttribute("vertPos");
		stageProgram->addAttribute("vertColor");

		// Gift Nub Shader Program:
		giftNubProgram = std::make_shared<Program>();
		giftNubProgram->setVerbose(true);
		giftNubProgram->setShaderNames(resourceDirectory + "/giftnub_shader_vertex.glsl", resourceDirectory + "/giftnub_shader_fragment.glsl");
		if (!giftNubProgram->init())
		{
			std::cerr << "One or more shaders failed to compile... exiting!" << std::endl;
			exit(1);
		}
		giftNubProgram->addUniform("P");
		giftNubProgram->addUniform("V");
		giftNubProgram->addUniform("M");
		giftNubProgram->addAttribute("vertPos");
	}

	bool BSIntersectsOBB(BS sphere, OBB other)
	{
		float cornerDists[8];
		glm::vec3 planePoints[3], n, v;
		float minDists[3] = {MAXFLOAT, MAXFLOAT, MAXFLOAT};

		// Get distances to all points
		for(int p = 0; p < 8; p++) {
			cornerDists[p] = glm::distance(other.bounds[p], sphere.center);
			std::cout << "Dist" << p << ": " << cornerDists[p] << std::endl;
		}
		// Get the minimum 3 distances
		for(int p = 0; p < 8; p++) {
			if(cornerDists[p] < minDists[2]) {
				if(cornerDists[p] < minDists[1]) {
					if(cornerDists[p] < minDists[0]) {
						minDists[0] = cornerDists[p];
						planePoints[0] = other.bounds[p];
						std::cout << "Plane 0: x:" << planePoints[0].x << " y:" << planePoints[0].y << " z:" << planePoints[0].z << std::endl;
					}
					else {
						minDists[1] = cornerDists[p];
						planePoints[1] = other.bounds[p];
						std::cout << "Plane 1: x:" << planePoints[1].x << " y:" << planePoints[1].y << " z:" << planePoints[1].z << std::endl;
					}
				}
				else {
					minDists[2] = cornerDists[p];
					planePoints[2] = other.bounds[p];
					std::cout << "Plane 2: x:" << planePoints[2].x << " y:" << planePoints[2].y << " z:" << planePoints[2].z << std::endl;
				}
			}
		}
		std::cout << "minDist0: " << minDists[0] << std::endl;
		std::cout << "minDist1: " << minDists[1] << std::endl;
		std::cout << "minDist2: " << minDists[2] << std::endl;

		// Calculate the normal and normalize
		n = glm::normalize(glm::cross((planePoints[1] - planePoints[0]), (planePoints[2] - planePoints[0])));
		std::cout << "Plane Normal: x:" << n.x << " y:" << n.y << " z:" << n.z << std::endl;

		// Project the vector from the center to an arbitrary point on the plane
		v = sphere.center - planePoints[0];
		float minDistToPlane = abs(glm::dot(v, n));
		std::cout << "Dist from plane: " << minDistToPlane << std::endl;

		return minDistToPlane <= sphere.radius;
	}

	bool BSIntersectsBS(BS sphere1, BS sphere2)
	{
		return (sphere1.center.x + sphere1.radius > sphere2.center.x - sphere2.radius || 
			sphere1.center.y + sphere1.radius > sphere2.center.y + sphere2.radius ||
			sphere1.center.z + sphere1.radius > sphere2.center.z + sphere2.radius);
	}
	
	void printMatrix(const glm::mat4 &mat, const char *name) {
		if (name) {
			printf("%s = [\n", name);
		}
		for (int i = 0; i < 4; ++i) {
			for (int j = 0; j < 4; ++j) {
				printf("%- 5.2f ", mat[i][j]);
			}
			printf("\n");
		}
		if (name) {
			printf("];");
		}
		printf("\n");
	}

	void createOBB_VAO(int vaoID, GLuint * VertexArrayID, GLuint * VertexBufferIDs,
		GLuint * VertexColorIDBox, GLuint * IndexBufferIDBox, OBB boundBox) {
		// Create VAOI
		glGenVertexArrays(1, &VertexArrayID[vaoID]);
		glBindVertexArray(VertexArrayID[vaoID]);

		// Create VBO0: Rectangle
			glGenBuffers(1, &VertexBufferIDs[vaoID]);
			glBindBuffer(GL_ARRAY_BUFFER, VertexBufferIDs[vaoID]);

			// Cube Position Buffer
			GLfloat cube_vertices[] = {
				// front
				boundBox.bounds[0].x, boundBox.bounds[0].y, boundBox.bounds[0].z, // -1,-1,1
				boundBox.bounds[1].x, boundBox.bounds[1].y, boundBox.bounds[1].z, // 1,-1,1
				boundBox.bounds[2].x, boundBox.bounds[2].y, boundBox.bounds[2].z, // 1,1,1
				boundBox.bounds[3].x, boundBox.bounds[3].y, boundBox.bounds[3].z, // -1,1,1
				// back
				boundBox.bounds[4].x, boundBox.bounds[4].y, boundBox.bounds[4].z, //-1,-1,-1
				boundBox.bounds[5].x, boundBox.bounds[5].y, boundBox.bounds[5].z, // 1,-1,-1
				boundBox.bounds[6].x, boundBox.bounds[6].y, boundBox.bounds[6].z, // 1,1,-1
				boundBox.bounds[7].x, boundBox.bounds[7].y, boundBox.bounds[7].z, // -1,1,-1
			};

			glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_DYNAMIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
		// End VBO0 Section

		// Create VBO1
			glGenBuffers(1, &VertexColorIDBox[vaoID]);
			glBindBuffer(GL_ARRAY_BUFFER, VertexColorIDBox[vaoID]);

			// Cube Color Buffer
			GLfloat cube_colors[] = {
				// front colors
				0.0, 1.0, 0.0,
				0.0, 1.0, 0.0,
				0.0, 1.0, 0.0,
				0.0, 1.0, 0.0,
				// back colors
				0.0, 1.0, 0.0,
				0.0, 1.0, 0.0,
				0.0, 1.0, 0.0,
				0.0, 1.0, 0.0,
			};

			glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		// End VBO1 Section

		// Create VBO2
			glGenBuffers(1, &IndexBufferIDBox[vaoID]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferIDBox[vaoID]);

			GLushort cube_elements[] = {
				// front
				0, 1, 2,
				2, 3, 0,
				// top
				1, 5, 6,
				6, 2, 1,
				// back
				7, 6, 5,
				5, 4, 7,
				// bottom
				4, 0, 3,
				3, 7, 4,
				// left
				4, 5, 1,
				1, 0, 4,
				// right
				3, 2, 6,
				6, 7, 3,
			};

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_elements), cube_elements, GL_STATIC_DRAW);
			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		// End VBO2 Section

		// De-"select" the VAO
		glBindVertexArray(0);
	// End VAOI Section
	}

	void printOBB(OBB * bounder) {
		for(int i = 0; i < 8; i++) {
			std::cout << "BoundBox Point" << i << ": x:" << bounder->bounds[i].x << " y:" << bounder->bounds[i].y << " z:" << bounder->bounds[i].z << std::endl;
		}
	}

	OBB applyMatrixToOBB(OBB cubeBound, glm::mat4 Model)
	{
		OBB b = cubeBound;
		for(int i = 0; i < 8; i++) {
			b.bounds[i] = Model * glm::vec4(b.bounds[i], 1.0);
		}

		b.min = b.bounds[4];
		b.max = b.bounds[2];

		return b;
	}

	BS getBoundingSphere(glm::mat4 Model, glm::vec3 center, glm::vec3 size)
	{
		BS bound;
		bound.center = Model * glm::vec4(center, 1.0);
		std::cout << "Sphere center: x:" << bound.center.x << " y:" << bound.center.y << " z:" << bound.center.z << std::endl;
		bound.radius = size.x;
		std::cout << "Sphere Radius: " << bound.radius << std::endl;

		return bound;
	}

	/****DRAW
	This is the most important function in your program - this is where you
	will actually issue the commands to draw any geometry you have set up to
	draw
	********/
	void render()
	{
		// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		// Get current frame buffer size.
		int width, height;
		glfwGetFramebufferSize(windowManager->getHandle(), &width, &height);
		float aspect = width/(float)height;
		glViewport(0, 0, width, height);

		// Clear framebuffer.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		static float globeYAxis = 0.0;
		static float globeXAxis = 0.0;
		if(mycam.a == 1) {
			globeYAxis += 0.1;
		}
		else if(mycam.d == 1) {
			globeYAxis -= 0.1;
		}
		if(mycam.w == 1) {
			globeXAxis += 0.1;
		}
		else if(mycam.s == 1) {
			globeXAxis -= 0.1;
		}
		
		double frametime = get_last_elapsed_time();

		// Create the matrix stacks
		glm::mat4 V, M, PP;
		V = mycam.process(frametime);
		// V = glm::mat4(1);
		// Apply orthographic projection.
		PP = glm::ortho(-1 * aspect, 1 * aspect, -1.0f, 1.0f, -2.0f, 100.0f);		
		if (width < height)
		{
			PP = glm::ortho(-1.0f, 1.0f, -1.0f / aspect,  1.0f / aspect, -2.0f, 100.0f);
		}
		
		glm::mat4 Trans1, Trans2, R, SpinR, TransPlayer, STAGE, ITEM, Rx, Rz, Scale;

	// Draw the Stage using GLSL.
		stageProgram->bind();
		glDisable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		// Render the Basic Stage
			// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			static float stagex = 0.0, stagey = -0.8, stagez = 0.0;
			static float xAngle = 0.0, zAngle = 0.0; // limit at slightly less than pi / 4
			static glm::vec3 player = glm::vec3(0.0, 0.7, 0.0);
			float stageSpeed = frametime/10;
			if(stageback == 1) {
				xAngle += stageSpeed;
				player.z += stageSpeed * 5;
			}
			if(stagefwd == 1) {
				xAngle -= stageSpeed;
				player.z -= stageSpeed * 5;
			}
			// xAngle = std::clamp(xAngle, -M_PI / 4, M_PI / 4);
			if(stageleft == 1) {
				zAngle += stageSpeed;
				player.x -= stageSpeed * 5;
			}
			if(stageright == 1) {
				zAngle -= stageSpeed;
				player.x += stageSpeed * 5;
			}
			// zAngle = std::clamp(zAngle, -M_PI / 4, M_PI / 4);

			glm::vec3 stageSize = glm::vec3(1.0, 0.05, 1.0);
			glm::vec3 stageCenter = glm::vec3(stagex, stagey, stagez);

			Trans1 = glm::translate(glm::mat4(1.0f), stageCenter);
			Rx = glm::rotate(glm::mat4(1.0f), xAngle, glm::vec3(1.0, 0.0, 0.0));
			Rz = glm::rotate(glm::mat4(1.0f), zAngle, glm::vec3(0.0, 0.0, 1.0));
			Scale = glm::scale(glm::mat4(1.0f), stageSize);
			STAGE = Trans1 * Rx * Rz;
			M = STAGE * Scale;

			OBB stageBound = applyMatrixToOBB(cubeBound, M);
			printOBB(&stageBound);

			printMatrix(M, "Stage Matrix");

			// Send the matrices to the shaders
			glUniformMatrix4fv(stageProgram->getUniform("P"), 1, GL_FALSE, &PP[0][0]);
			glUniformMatrix4fv(stageProgram->getUniform("V"), 1, GL_FALSE, &V[0][0]);
			glUniformMatrix4fv(stageProgram->getUniform("M"), 1, GL_FALSE, &M[0][0]);

			glBindVertexArray(VertexArrayID[0]);
			// Actually draw from vertex 0, 3 vertices
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VertexBufferIDs[2]);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
			glBindVertexArray(0);
			
			// glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		// End Basic Stage Rendering

		// Render Test Stage Bounder
		 	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

			M = M * glm::scale(glm::mat4(1.0f), glm::vec3(1.01, 1.1, 1.01));
			glUniformMatrix4fv(stageProgram->getUniform("M"), 1, GL_FALSE, &M[0][0]);

			glBindVertexArray(VertexArrayID[0]);
			// Actually draw from vertex 0, 3 vertices
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VertexBufferIDs[2]);
			glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, (void*)0);
			glBindVertexArray(0);

			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		// End Test Stage Bounder Rendering

		stageProgram->unbind();

	// BEAR BALL RENDERING:
		glm::vec3 chScale = glm::vec3(0.2, 0.2, 0.2);
		glm::mat4 CharacterScale = glm::scale(glm::mat4(1.0f), chScale);
		// Bear Model Rendering
			glDisable(GL_CULL_FACE);
			glEnable(GL_DEPTH_TEST);

			bearProgram->bind();

			TransPlayer = glm::translate(glm::mat4(1.0f), player);
			Scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.3, 0.35, 0.3));

			// Content Control Matrices
			glm::mat4 HEAD, BODY, ARMR, ARML, LEGR, LEGL, EYER, EYEL, NOSE, EARR, EARL;

			// send the matrices to the shaders
			glUniformMatrix4fv(bearProgram->getUniform("P"), 1, GL_FALSE, &PP[0][0]);
			glUniformMatrix4fv(bearProgram->getUniform("V"), 1, GL_FALSE, &V[0][0]);

			// HEAD
			static float headAngle = 0.0;
			headAngle = 3.14159/4 * sin(glfwGetTime());
			
			SpinR = glm::rotate(glm::mat4(1.0f), headAngle, glm::vec3(0.0, 1.0, 0.0));
			Scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.20, 0.15, 0.15));
			Trans1 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.36 * chScale.y, 0.0));

			HEAD = TransPlayer * Trans1 * SpinR;
			M = HEAD * Scale * CharacterScale;
			glUniformMatrix4fv(bearProgram->getUniform("M"), 1, GL_FALSE, &M[0][0]);
			shape2->draw(bearProgram, false);

			// BODY
			Scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.26, 0.31, 0.26));
			Trans1 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.02 * chScale.y, 0.0));

			BODY = TransPlayer * Trans1;
			M = BODY * Scale * CharacterScale;
			glUniformMatrix4fv(bearProgram->getUniform("M"), 1, GL_FALSE, &M[0][0]);
			shape2->draw(bearProgram, false);

			// ARMS
			static float spin = 0.0;
			spin = 3.1416/2 * sin(glfwGetTime()) - 3.1416/4;

			float angle = 3.141592/3;
			Scale = glm::scale(glm::mat4(1.0f), vec3(0.075, 0.25, 0.075));

			// RIGHT ARM
				SpinR = glm::rotate(glm::mat4(1.0f), -spin, glm::vec3(1.0, 0.0, 0.0));
				R = glm::rotate(glm::mat4(1.0f), -angle, glm::vec3(0.0, 0.0, 1.0));
				Trans2 = glm::translate(glm::mat4(1.0f), vec3(0.17 * chScale.x, 0.14 * chScale.y, 0.0));

				ARMR = TransPlayer * Trans2 * SpinR * R;
				M = ARMR * Scale * CharacterScale;
				glUniformMatrix4fv(bearProgram->getUniform("M"), 1, GL_FALSE, &M[0][0]);

				shape3->draw(bearProgram, false);

			// LEFT ARM
				SpinR = glm::rotate(glm::mat4(1.0f), -spin, glm::vec3(1.0, 0.0, 0.0));
				R = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0, 0.0, 1.0));
				Trans2 = glm::translate(glm::mat4(1.0f), vec3(-0.17 * chScale.x, 0.14 * chScale.y, 0.0));

				ARML = TransPlayer * Trans2 * SpinR * R;
				M = ARML * Scale * CharacterScale;
				glUniformMatrix4fv(bearProgram->getUniform("M"), 1, GL_FALSE, &M[0][0]);

				shape3->draw(bearProgram, false);

			// LEGS
			static float langle = 7.2 * 3.141592 / 8;
			Scale = glm::scale(glm::mat4(1.0f), vec3(0.075, 0.2, 0.075));

			// RIGHT LEG
				R = glm::rotate(glm::mat4(1.0f), -langle, glm::vec3(0.0, 0.0, 1.0));
				Trans2 = glm::translate(glm::mat4(1.0f), vec3(0.12 * chScale.x, -0.22 * chScale.y, 0.0));

				LEGR = TransPlayer * Trans2 * R;
				M = LEGR * Scale * CharacterScale;
				glUniformMatrix4fv(bearProgram->getUniform("M"), 1, GL_FALSE, &M[0][0]);

				shape3->draw(bearProgram, false);

			// LEFT LEG
				R = glm::rotate(glm::mat4(1.0f), langle, glm::vec3(0.0, 0.0, 1.0));
				Trans2 = glm::translate(glm::mat4(1.0f), vec3(-0.12 * chScale.x, -0.22 * chScale.y, 0.0));

				LEGL = TransPlayer * Trans2 * R;
				M = LEGL * Scale * CharacterScale;
				glUniformMatrix4fv(bearProgram->getUniform("M"), 1, GL_FALSE, &M[0][0]);

				shape3->draw(bearProgram, false);

			// EYES
			Scale = glm::scale(glm::mat4(1.0f), vec3(0.038, 0.05, 0.038));

			// RIGHT EYE
				Trans2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.05 * chScale.x, 0.06 * chScale.y, 0.12 * chScale.z));

				EYER = HEAD * Trans2;
				M = EYER * Scale * CharacterScale;

				glUniformMatrix4fv(bearProgram->getUniform("M"), 1, GL_FALSE, &M[0][0]);
				shape2->draw(bearProgram, false);

			// LEFT EYE
				Trans2 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.05 * chScale.x, 0.06 * chScale.y, 0.12 * chScale.z));

				EYEL = HEAD * Trans2;
				M = EYEL * Scale * CharacterScale;

				glUniformMatrix4fv(bearProgram->getUniform("M"), 1, GL_FALSE, &M[0][0]);
				shape2->draw(bearProgram, false);

			// NOSE
			Scale = glm::scale(glm::mat4(1.0f), vec3(0.025, 0.025, 0.025));
			Trans2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.028 * chScale.y, 0.145 * chScale.z));

			NOSE = HEAD * Trans2;
			M = NOSE * Scale * CharacterScale;

			glUniformMatrix4fv(bearProgram->getUniform("M"), 1, GL_FALSE, &M[0][0]);
			shape2->draw(bearProgram, false);

			// EARS
			float earAngle = 0.3;
			Scale = glm::scale(glm::mat4(1.0f), vec3(0.05, 0.065, 0.05));

			// RIGHT EAR
				Trans2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.1 * chScale.x, 0.16 * chScale.y, 0.025 * chScale.z));
				R = glm::rotate(glm::mat4(1.0f), -earAngle, glm::vec3(0.0, 0.0, 1.0));

				EARR = HEAD * Trans2 * R;
				M = EARR * Scale * CharacterScale;

				glUniformMatrix4fv(bearProgram->getUniform("M"), 1, GL_FALSE, &M[0][0]);
				shape2->draw(bearProgram, false);

			// LEFT EAR
				Trans2 = glm::translate(glm::mat4(1.0f), glm::vec3(-0.1 * chScale.x, 0.16 * chScale.y, 0.025 * chScale.z));
				R = glm::rotate(glm::mat4(1.0f), earAngle, glm::vec3(0.0, 0.0, 1.0));

				EARL = HEAD * Trans2 * R;
				M = EARL * Scale * CharacterScale;

				glUniformMatrix4fv(bearProgram->getUniform("M"), 1, GL_FALSE, &M[0][0]);
				shape2->draw(bearProgram, false);

			bearProgram->unbind();

		// Bear's Ball Model Rendering
			glm::mat4 RGlobeY, RGlobeX;
			RGlobeY = glm::rotate(glm::mat4(1.0), globeYAxis, glm::vec3(0.0, 1.0, 0.0));
			RGlobeX = glm::rotate(glm::mat4(1.0), globeXAxis, glm::vec3(1.0, 0.0, 0.0));
			M = TransPlayer * RGlobeY * RGlobeX * glm::mat4(1) * CharacterScale;

			float radius = 1.0 * chScale.y;
			float maxDistFromSemi = sqrt(pow(radius, 2) * 2);
			glm::vec3 semiSphere = glm::vec3(0.0, radius, 0.0);
			glm::vec4 tempTransformer = TransPlayer * RGlobeY * RGlobeX * glm::vec4(semiSphere, 1.0);
			semiSphere.x = tempTransformer.x;
			semiSphere.y = tempTransformer.y;
			semiSphere.z = tempTransformer.z;

			ballProgram->bind();

			//send the matrices to the shaders
			glUniformMatrix4fv(ballProgram->getUniform("P"), 1, GL_FALSE, &PP[0][0]);
			glUniformMatrix4fv(ballProgram->getUniform("V"), 1, GL_FALSE, &V[0][0]);
			glUniformMatrix4fv(ballProgram->getUniform("M"), 1, GL_FALSE, &M[0][0]);
			glUniform3fv(ballProgram->getUniform("semiSphere"), 1, &semiSphere.x);
			glUniform1f(ballProgram->getUniform("maxDistFromSemi"), maxDistFromSemi);

			BS playerBound = getBoundingSphere(M, player, chScale);

			float t = frametime;
			static float playerVelocity = 0.0;
			player.y += 0.5 * GRAVITY * t*t + playerVelocity * t;
			playerVelocity += GRAVITY * t;

			if(BSIntersectsOBB(playerBound, stageBound)) {
				player.y -= 0.5 * GRAVITY * t*t + playerVelocity * t;
				playerVelocity = 0.0;
			}

			glDisable(GL_CULL_FACE);
			shape1->draw(ballProgram, false);

			// TEMPORARY!!!
			// Render Test Item Object
			glm::vec3 itemSize = glm::vec3(0.2, 0.2, 0.2);
			glm::vec3 itemCenter = glm::vec3(0.7, -0.55, 0.7);
			Scale = glm::scale(glm::mat4(1.0f), itemSize);
			Trans1 = glm::translate(glm::mat4(1.0f), itemCenter);
			ITEM = Trans1;
			M = ITEM * Scale;

			BS itemBound = getBoundingSphere(M, itemCenter, itemSize);

			glUniformMatrix4fv(ballProgram->getUniform("M"), 1, GL_FALSE, &M[0][0]);

			if(BSIntersectsBS(playerBound, itemBound))
				shape5->draw(ballProgram, false);

			ballProgram->unbind();

		// ARCH stuff
		// Scale = glm::scale(glm::mat4(1.0f), vec3(0.5, 0.5, 0.5));
		// Trans2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.2, -0.8));
		// R = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0, 0.0, 1.0));

		// glm::mat4 ARCH, GIFT;
		// ARCH = TransPlayer * Trans2 * R;
		// M = ARCH * Scale;

		// glUniformMatrix4fv(contentsProgram->getUniform("M"), 1, GL_FALSE, &M[0][0]);
		// shape4->draw(contentsProgram, false);


		// Scale = glm::scale(glm::mat4(1.0f), vec3(0.2, 0.2, 0.2));
		// Trans2 = glm::translate(glm::mat4(1.0f), glm::vec3(1.1, 0.0, -0.8));
		// R = glm::rotate(glm::mat4(1.0f), 0.0f, glm::vec3(0.0, 0.0, 1.0));

		// GIFT = TransPlayer * Trans2 * R;
		// M = GIFT * Scale;

		// glUniformMatrix4fv(contentsProgram->getUniform("M"), 1, GL_FALSE, &M[0][0]);
		// shape5->draw(contentsProgram, false);
	}

};
//******************************************************************************************
int main(int argc, char **argv)
{
	std::string resourceDir = "../resources"; // Where the resources are loaded from
	if (argc >= 2)
	{
		resourceDir = argv[1];
	}

	Application *application = new Application();

	/* your main will always include a similar set up to establish your window
		and GL context, etc. */
	WindowManager * windowManager = new WindowManager();
	windowManager->init(1920, 1080);
	windowManager->setEventCallbacks(application);
	application->windowManager = windowManager;

	/* This is the code that will likely change program to program as you
		may need to initialize or set up different data and state */
	// Initialize scene.
	application->init(resourceDir);
	application->initGeom();

	// Loop until the user closes the window.
	while(! glfwWindowShouldClose(windowManager->getHandle()))
	{
		// Render scene.
		application->render();

		// Swap front and back buffers.
		glfwSwapBuffers(windowManager->getHandle());
		// Poll for and process events.
		glfwPollEvents();
	}

	// Quit program.
	windowManager->shutdown();
	return 0;
}
