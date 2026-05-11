// Medina Villa Samuel 320249538
// Eric Ramírez 423095203
// Proyecto Final
// Fecha de entrega: 13 de mayo de 2026




#include <iostream>
#include <cmath>

// GLEW
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// Other Libs
#include "stb_image.h"

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//Load Models
#include "SOIL2/SOIL2.h"


// Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "ModeloAnimado.h"
#include "Texture.h"


// Function prototypes
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
void MouseCallback(GLFWwindow* window, double xPos, double yPos);
void DoMovement();
void Animation();

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;
int SCREEN_WIDTH, SCREEN_HEIGHT;

// Camera
Camera  camera(glm::vec3(0.0f, 0.7f, -3.0f));
GLfloat lastX = WIDTH / 2.0;
GLfloat lastY = HEIGHT / 2.0;
bool keys[1024];
bool firstMouse = true;
// Light attributes
glm::vec3 lightPos(0.0f, 0.0f, 0.0f);
bool active;
// Variable para el cambio de modo dia/noche L
bool modoDia = true;

// Positions of the point lights
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f,2.0f, 0.0f),
	glm::vec3(0.0f,0.0f, 0.0f),
	glm::vec3(0.0f,0.0f,  0.0f),
	glm::vec3(0.0f,0.0f, 0.0f)
};

float vertices[] = {
	 -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	   -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	   -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};


glm::vec3 Light1 = glm::vec3(0);
//Anim

float FLegL = 0.0f; // Front Left
float FLegR = 0.0f; // Front Right
float RLegL = 0.0f; // Rear Left
float RLegR = 0.0f; // Rear Right
float head = 0.0f;
float tail = 0.0f;

float IncliDog = 0.0f;

//  ANIMACIÓN PRESENTADOR 
float expositorRotMano = 0.0f;
float expositorRotAntebrazo = 0.0f;
float expositorRotCabeza = 0.0f;
float expositorTiempo = 0.0f;
bool animExpositor = true;



bool animVisitante = true;
float visPosX = 0.0f;
float visRotY = 0.0f;
float tiempoAnimVisitante = 0.0f;

#define VIS_MAX_FRAMES 10
int vis_i_max_steps = 120;
int vis_i_curr_steps = 0;

typedef struct _vis_frame {
	float visPosX;    float visPosXInc;
	float visRotY;    float visRotYInc;
} VIS_FRAME;

VIS_FRAME VisKF[VIS_MAX_FRAMES];
int visPlayIndex = 0;
bool visPlay = false;

void visResetElements(void) {
	visPosX = VisKF[0].visPosX;
	visRotY = VisKF[0].visRotY;
}

void visInterpolation(void) {
	VisKF[visPlayIndex].visPosXInc = (VisKF[visPlayIndex + 1].visPosX - VisKF[visPlayIndex].visPosX) / vis_i_max_steps;
	VisKF[visPlayIndex].visRotYInc = (VisKF[visPlayIndex + 1].visRotY - VisKF[visPlayIndex].visRotY) / vis_i_max_steps;
}


Model* logoOracle;
Model* logoPG;
Model* logoAmazon;

float tiempoOracle = 0.0f;
float tiempoAmazon = 0.0f;
float tiempoPG = 0.0f;


bool playOracle = false;
bool playAmazon = false;
bool playPG = false;

// Perro Robot
float tiempoPerro = 0.0f;
bool playPerro = false;

// Camara de seguridad (animacion automatica)
float tiempoCamera = 0.0f;
float rotCamera = 0.0f;


// Deltatime
GLfloat deltaTime = 0.0f;	// Time between current frame and last frame
GLfloat lastFrame = 0.0f;  	// Time of last frame

int main()
{
	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);*/

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "ProyectoFinal-Samuel Medina Villa-Eric Ramirez", nullptr, nullptr);

	if (nullptr == window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();

		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	glfwGetFramebufferSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);

	// Set the required callback functions
	glfwSetKeyCallback(window, KeyCallback);
	glfwSetCursorPosCallback(window, MouseCallback);

	// GLFW Options
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	if (GLEW_OK != glewInit())
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return EXIT_FAILURE;
	}

	// Define the viewport dimensions
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	logoOracle = new Model((char*)"Models/logos/oracle.obj");
	logoAmazon = new Model((char*)"Models/logos/amazon.obj");
	logoPG = new Model((char*)"Models/logos/pg.obj");

	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

	// Nuevo shader para skybox
	Shader skyboxshader("Shader/skybox.vs", "Shader/skybox.frag");

	// Shader para animacion con huesos
	Shader shaderAnimacion("Shader/anim.vs", "Shader/anim.frag");


	//Modelos
	// Modelos del puente
	Model Puente((char*)"Models/Puente/Puente.obj");


	//mamparas basicas
	Model mamp1((char*)"Models/stands_2/mamp_1.obj");
	Model mamp2((char*)"Models/stands_2/mamp_2.obj");
	Model mamp3((char*)"Models/stands_2/mamp_3.obj");
	Model mamp4((char*)"Models/stands_2/mamp_4.obj");
	Model mamp5((char*)"Models/stands_2/mamp_5.obj");
	Model mamp6((char*)"Models/stands_2/mamp_6.obj");
	Model mamp7((char*)"Models/stands_2/mamp_7.obj");
	Model mamp8((char*)"Models/stands_2/mamp_8.obj");

	//stands de empresas
	Model stand_pag((char*)"Models/stands_1/pg.obj");
	Model stand_amazon((char*)"Models/stands_1/amazon.obj");
	Model stand_oracle((char*)"Models/stands_1/oracle.obj");

	////stands de asociaciones
	Model stand_crofi((char*)"Models/stands_1/crofi.obj");
	Model stand_aero((char*)"Models/stands_1/aero.obj");
	Model stand_siafi((char*)"Models/stands_1/siafi.obj");

	// Modelo del presentador
	Model presentadorCuerpo((char*)"Models/Presentador/cuerpo.obj");
	Model presentadorCabeza((char*)"Models/Presentador/cabeza.obj");
	Model presentadorAntebrazo((char*)"Models/Presentador/antebrazo.obj");
	Model presentadorMano((char*)"Models/Presentador/mano.obj");

	//pumagua
	Model pumagua((char*)"Models/extras/pumagua/pumagua.obj");

	// Camara de seguridad
	Model camBase((char*)"Models/extras/camara/cam_base.obj");
	Model camCabeza((char*)"Models/extras/camara/cam_cabeza.obj");


	// Modelos con animacion
	ModeloAnimado visitante((GLchar*)"Models/Visitante/visitante.fbx");
	ModeloAnimado perroRobot((GLchar*)"Models/perroRobot/perro.fbx");



	// Definicion de la caja 
	GLfloat skyboxVertices[] = {
		// Positions
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		1.0f,  1.0f, -1.0f,
		1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		1.0f, -1.0f,  1.0f
	};


	GLuint indices[] =
	{  // Note that we start from 0!
		0,1,2,3,
		4,5,6,7,
		8,9,10,11,
		12,13,14,15,
		16,17,18,19,
		20,21,22,23,
		24,25,26,27,
		28,29,30,31,
		32,33,34,35
	};


	// First, set the container's VAO (and VBO)

	// Se modifican los buffers para el skybox
	GLuint VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);


	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);



	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Set texture units
	lightingShader.Use();
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.difuse"), 0);
	glUniform1i(glGetUniformLocation(lightingShader.Program, "Material.specular"), 1);

	//Skybox

	GLuint skyboxVBO, skyboxVAO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);


	//Load textures del skybox

	vector  < const GLchar*> faces;
	faces.push_back("Skybox/left.jpg");
	faces.push_back("Skybox/right.jpg");
	faces.push_back("Skybox/top.jpg");
	faces.push_back("Skybox/bottom.jpg");
	faces.push_back("Skybox/front.jpg");
	faces.push_back("Skybox/back.jpg");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	// Skybox exterior DIA
	vector<const GLchar*> facesExterior;
	facesExterior.push_back("SkyboxExterior/right.jpg");
	facesExterior.push_back("SkyboxExterior/left.jpg");
	facesExterior.push_back("SkyboxExterior/top.jpg");
	facesExterior.push_back("SkyboxExterior/bottom.jpg");
	facesExterior.push_back("SkyboxExterior/front.jpg");
	facesExterior.push_back("SkyboxExterior/back.jpg");



	// Skybox interior NOCHE
	vector<const GLchar*> facesNoche;
	facesNoche.push_back("SkyboxNoche/left.jpg");
	facesNoche.push_back("SkyboxNoche/right.jpg");
	facesNoche.push_back("SkyboxNoche/top.jpg");
	facesNoche.push_back("SkyboxNoche/bottom.jpg");
	facesNoche.push_back("SkyboxNoche/front.jpg");
	facesNoche.push_back("SkyboxNoche/back.jpg");

	GLuint cubemapTextureNoche = TextureLoading::LoadCubemap(facesNoche);

	// Skybox exterior NOCHE
	vector<const GLchar*> facesExteriorNoche;
	facesExteriorNoche.push_back("SkyboxExteriorNoche/right.jpg");
	facesExteriorNoche.push_back("SkyboxExteriorNoche/left.jpg");
	facesExteriorNoche.push_back("SkyboxExteriorNoche/top.jpg");
	facesExteriorNoche.push_back("SkyboxExteriorNoche/bottom.jpg");
	facesExteriorNoche.push_back("SkyboxExteriorNoche/back.jpg");
	facesExteriorNoche.push_back("SkyboxExteriorNoche/front.jpg");

	GLuint cubemapTextureExterior = TextureLoading::LoadCubemap(facesExterior);
	GLuint cubemapTextureExteriorNoche = TextureLoading::LoadCubemap(facesExteriorNoche);

	// Restaurar el binding del cubemap interior
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);


	glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 100.0f);


	// Valores de los KeyFrames

	//                    posX     rotY
	VisKF[0] = { -250.0f, 0,   0.0f, 0 };  // Inicio
	VisKF[1] = { 80.0f, 0,   0.0f, 0 };  // Llega al final
	VisKF[2] = { 80.0f, 0, -75.0f, 0 };  // Mira stand derecho
	VisKF[3] = { 80.0f, 0, -75.0f, 0 };  // Sostiene mirada
	VisKF[4] = { 80.0f, 0,  75.0f, 0 };  // Mira stand izquierdo
	VisKF[5] = { 80.0f, 0,  75.0f, 0 };  // Sostiene mirada
	VisKF[6] = { 80.0f, 0, 180.0f, 0 };  // Gira para regresar
	VisKF[7] = { -250.0f, 0, 180.0f, 0 };  // Regresa al inicio
	VisKF[8] = { -250.0f, 0, 360.0f, 0 };  // Gira para mirar al fondo
	VisKF[9] = { -250.0f, 0, 360.0f, 0 };  // Sostiene (loop)



	visResetElements();
	visPlay = true;
	visPlayIndex = 0;
	vis_i_curr_steps = 0;
	visInterpolation();

	// Game loop
	while (!glfwWindowShouldClose(window))
	{

		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();
		DoMovement();
		Animation();

		// Clear the colorbuffer
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// OpenGL options
		glEnable(GL_DEPTH_TEST);

		// Skybox Exterior
		glm::mat4 viewExt = camera.GetViewMatrix();
		glm::mat4 modelExt = glm::mat4(1);
		modelExt = glm::translate(modelExt, glm::vec3(-0.0413f, 1.1f, -2.9433f));
		modelExt = glm::scale(modelExt, glm::vec3(1.7f, 1.0f, 1.7f));

		glDepthFunc(GL_LEQUAL);
		glDepthMask(GL_FALSE);
		skyboxshader.Use();
		glUniform1i(glGetUniformLocation(skyboxshader.Program, "skybox"), 1);
		glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(viewExt));
		glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelExt));

		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, modoDia ? cubemapTextureExterior : cubemapTextureExteriorNoche);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);


		glm::mat4 modelTemp = glm::mat4(1.0f); //Temp



		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);


		// Directional light
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -0.2f, -1.0f, -0.3f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.6f, 0.6f, 0.6f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.3f, 0.3f, 0.3f);


		// Point light 1
		glm::vec3 lightColor;
		lightColor.x = abs(sin(glfwGetTime() * Light1.x));
		lightColor.y = abs(sin(glfwGetTime() * Light1.y));
		lightColor.z = sin(glfwGetTime() * Light1.z);


		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), lightColor.x, lightColor.y, lightColor.z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 1.0f, 0.2f, 0.2f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.075f);


		// SpotLight
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.2f, 0.2f, 0.8f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));


		// Set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 5.0f);

		// Create camera transformations
		glm::mat4 view;
		view = camera.GetViewMatrix();

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		glm::mat4 model(1);



		//Carga de modelo 
		// Puente
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		Puente.Draw(lightingShader);


		// Stands

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		stand_pag.Draw(lightingShader);
		stand_amazon.Draw(lightingShader);
		stand_oracle.Draw(lightingShader);
		stand_crofi.Draw(lightingShader);
		stand_aero.Draw(lightingShader);
		stand_siafi.Draw(lightingShader);


		///////// PRESENTADOR //////////
		glm::mat4 modelPres = glm::mat4(1);
		modelPres = glm::translate(modelPres, glm::vec3(0.0f, 0.0f, -3.0f));
		modelPres = glm::scale(modelPres, glm::vec3(0.005f, 0.005f, 0.005f));

		// Cuerpo 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPres));
		presentadorCuerpo.Draw(lightingShader);

		// Antebrazo 
		glm::mat4 antebrazoMat = modelPres;
		antebrazoMat = glm::translate(antebrazoMat, glm::vec3(-16.23f, 124.96f, -13.10f));
		antebrazoMat = glm::rotate(antebrazoMat, glm::radians(expositorRotAntebrazo), glm::vec3(1.0f, 0.0f, 0.0f));
		antebrazoMat = glm::translate(antebrazoMat, glm::vec3(16.23f, -124.96f, 13.10f));
		antebrazoMat = glm::translate(antebrazoMat, glm::vec3(0.3f, -0.1f, -0.62f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(antebrazoMat));
		presentadorAntebrazo.Draw(lightingShader);

		// Cabeza 
		glm::mat4 cabezaMat = modelPres;
		cabezaMat = glm::translate(cabezaMat, glm::vec3(-13.19f, 126.10f, -18.28f));
		cabezaMat = glm::rotate(cabezaMat, glm::radians(expositorRotCabeza), glm::vec3(0.0f, 1.0f, 0.0f));
		cabezaMat = glm::translate(cabezaMat, glm::vec3(13.19f, -126.10f, 18.28f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(cabezaMat));
		presentadorCabeza.Draw(lightingShader);

		// Mano 
		glm::mat4 manoMat = antebrazoMat;
		manoMat = glm::translate(manoMat, glm::vec3(-13.99f, 129.99f, -9.76f));
		manoMat = glm::rotate(manoMat, glm::radians(expositorRotMano), glm::vec3(0.0f, 1.0f, 0.0f));
		manoMat = glm::translate(manoMat, glm::vec3(13.99f, -129.99f, 9.76f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(manoMat));
		presentadorMano.Draw(lightingShader);

		///////// VISITANTE CAMINANDO //////////
		{
			shaderAnimacion.Use();
			GLint modelLocAnim = glGetUniformLocation(shaderAnimacion.Program, "model");
			GLint viewLocAnim = glGetUniformLocation(shaderAnimacion.Program, "view");
			GLint projLocAnim = glGetUniformLocation(shaderAnimacion.Program, "projection");
			glUniformMatrix4fv(viewLocAnim, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projLocAnim, 1, GL_FALSE, glm::value_ptr(projection));
			glUniform3f(glGetUniformLocation(shaderAnimacion.Program, "viewPos"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
			glUniform3f(glGetUniformLocation(shaderAnimacion.Program, "material.specular"), 0.05f, 0.05f, 0.05f);
			glUniform1f(glGetUniformLocation(shaderAnimacion.Program, "material.shininess"), 32.0f);
			glUniform3f(glGetUniformLocation(shaderAnimacion.Program, "light.direction"), -0.2f, -1.0f, -0.3f);
			glUniform3f(glGetUniformLocation(shaderAnimacion.Program, "light.ambient"), 0.6f, 0.6f, 0.6f);
			glUniform3f(glGetUniformLocation(shaderAnimacion.Program, "light.diffuse"), 0.6f, 0.6f, 0.6f);
			glUniform3f(glGetUniformLocation(shaderAnimacion.Program, "light.specular"), 0.3f, 0.3f, 0.3f);

			glm::mat4 modelVis = glm::mat4(1.0f);
			modelVis = glm::translate(modelVis, glm::vec3(0.0f, 0.0f, -3.0f));
			modelVis = glm::scale(modelVis, glm::vec3(0.005f, 0.005f, 0.005f));
			glUniformMatrix4fv(modelLocAnim, 1, GL_FALSE, glm::value_ptr(modelVis));

			tiempoAnimVisitante += deltaTime;
			visitante.Draw(shaderAnimacion, tiempoAnimVisitante);

			// ---- PERRO ROBOT ----
			glm::mat4 modelPerro = glm::mat4(1.0f);
			modelPerro = glm::translate(modelPerro, glm::vec3(0.0f, 0.0f, -3.0f));
			modelPerro = glm::translate(modelPerro, glm::vec3(-1.12f, 0.52f, 0.31f));
			modelPerro = glm::scale(modelPerro, glm::vec3(0.0001f, 0.0001f, 0.0001f));
			glUniformMatrix4fv(modelLocAnim, 1, GL_FALSE, glm::value_ptr(modelPerro));
			perroRobot.Draw(shaderAnimacion, tiempoPerro);

			//dibujamos logo oracle
			lightingShader.Use();
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

			glm::vec3 centroOracle(-255.8f, 136.83f, -48.15f); //(oordenadas tomadas de la escena en lender
			glm::vec3 centroPG(239.0f, 149.88f, -41.31f);
			glm::vec3 centroAmazon(7.27f, 141.24f, -44.42f);


			glm::mat4 modelOracle = glm::mat4(1.0f);
			modelOracle = glm::translate(modelOracle, glm::vec3(0.0f, 0.0f, -3.0f));
			modelOracle = glm::scale(modelOracle, glm::vec3(0.005f, 0.005f, 0.005f));
			modelOracle = glm::translate(modelOracle, centroOracle);          //mueve pivote al centroOracle

			if (playOracle)
				modelOracle = glm::rotate(modelOracle, tiempoOracle, glm::vec3(0.0f, 1.0f, 0.0f));
			modelOracle = glm::translate(modelOracle, -centroOracle);         //regresa pivote

			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelOracle));
			logoOracle->Draw(lightingShader);

			glm::mat4 modelAmazon = glm::mat4(1.0f);
			modelAmazon = glm::translate(modelAmazon, glm::vec3(0.0f, 0.0f, -3.0f));
			modelAmazon = glm::scale(modelAmazon, glm::vec3(0.005f, 0.005f, 0.005f));
			modelAmazon = glm::translate(modelAmazon, centroAmazon);

			if (playAmazon)
				modelAmazon = glm::rotate(modelAmazon, tiempoAmazon, glm::vec3(0.0f, 1.0f, 0.0f));
			modelAmazon = glm::translate(modelAmazon, -centroAmazon);

			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelAmazon));
			logoAmazon->Draw(lightingShader);

			glm::mat4 modelPG = glm::mat4(1.0f);
			modelPG = glm::translate(modelPG, glm::vec3(0.0f, 0.0f, -3.0f));
			modelPG = glm::scale(modelPG, glm::vec3(0.005f, 0.005f, 0.005f));
			modelPG = glm::translate(modelPG, centroPG);

			if (playPG)
				modelPG = glm::rotate(modelPG, tiempoPG, glm::vec3(0.0f, 1.0f, 0.0f));
			modelPG = glm::translate(modelPG, -centroPG);

			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPG));
			logoPG->Draw(lightingShader);

			lightingShader.Use();
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		}


		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		mamp1.Draw(lightingShader);
		mamp2.Draw(lightingShader);
		mamp3.Draw(lightingShader);
		mamp4.Draw(lightingShader);
		mamp5.Draw(lightingShader);
		mamp6.Draw(lightingShader);
		mamp7.Draw(lightingShader);
		mamp8.Draw(lightingShader);
		pumagua.Draw(lightingShader);

		//CAMARA DE SEGURIDAD
		glm::vec3 centroCamCabeza(-202.53f, 156.96f, -65.422f);

		// Base fija
		glm::mat4 modelCamBase = glm::mat4(1.0f);
		modelCamBase = glm::translate(modelCamBase, glm::vec3(0.0f, 0.0f, -3.0f));
		modelCamBase = glm::scale(modelCamBase, glm::vec3(0.005f, 0.005f, 0.005f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"),
			1, GL_FALSE, glm::value_ptr(modelCamBase));
		camBase.Draw(lightingShader);

		// Cabeza giratoria
		glm::mat4 modelCamCabeza = glm::mat4(1.0f);
		modelCamCabeza = glm::translate(modelCamCabeza, glm::vec3(0.0f, 0.0f, -3.0f));
		modelCamCabeza = glm::translate(modelCamCabeza, centroCamCabeza * 0.005f);     // pivot en espacio OpenGL
		modelCamCabeza = glm::rotate(modelCamCabeza, glm::radians(rotCamera), glm::vec3(0.0f, 1.0f, 0.0f));
		modelCamCabeza = glm::translate(modelCamCabeza, -centroCamCabeza * 0.005f);    // regresa pivot
		modelCamCabeza = glm::scale(modelCamCabeza, glm::vec3(0.005f, 0.005f, 0.005f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"),
			1, GL_FALSE, glm::value_ptr(modelCamCabeza));
		camCabeza.Draw(lightingShader);



		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		// Set matrices
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
		model = glm::mat4(1);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		// Draw the light object (using light's vertex attributes)

		model = glm::mat4(1);
		model = glm::translate(model, pointLightPositions[0]);
		model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);

		//Dibujo del skybox


		glDepthFunc(GL_LEQUAL);
		skyboxshader.Use();
		glUniform1i(glGetUniformLocation(skyboxshader.Program, "skybox"), 1);
		view = camera.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		// Modelo para posicionar y escalar la caja alrededor del puente
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-0.0413f, 0.6800f, -2.9475f));// Centro del puente
		model = glm::scale(model, glm::vec3(1.4619f, 0.1850f, 0.4875f));    // Imagenes al tamaño del puente
		glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, modoDia ? cubemapTexture : cubemapTextureNoche);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);

		///*glDepthFunc(GL_LEQUAL);
		//skyboxshader.Use();
		//view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		//glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		//glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


		//glBindVertexArray(skyboxVAO);
		//glActiveTexture(GL_TEXTURE1);
		//glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		//glDrawArrays(GL_TRIANGLES, 0, 36);
		//glBindVertexArray(0);
		//glDepthFunc(GL_LESS);*/


		// Swap the screen buffers
		glfwSwapBuffers(window);
	}

	// Se borra el buffer el skybox

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &skyboxVAO);




	// Terminate GLFW, clearing any resources allocated by GLFW.
	glfwTerminate();



	return 0;
}

// Moves/alters the camera positions based on user input
void DoMovement()
{


	// Camera controls
	if (keys[GLFW_KEY_W] || keys[GLFW_KEY_UP])
	{
		camera.ProcessKeyboard(FORWARD, deltaTime);

	}

	if (keys[GLFW_KEY_S] || keys[GLFW_KEY_DOWN])
	{
		camera.ProcessKeyboard(BACKWARD, deltaTime);


	}

	if (keys[GLFW_KEY_A] || keys[GLFW_KEY_LEFT])
	{
		camera.ProcessKeyboard(LEFT, deltaTime);


	}

	if (keys[GLFW_KEY_D] || keys[GLFW_KEY_RIGHT])
	{
		camera.ProcessKeyboard(RIGHT, deltaTime);


	}

	/*if (keys[GLFW_KEY_T])
	{
		pointLightPositions[0].x += 0.01f;
	}
	if (keys[GLFW_KEY_G])
	{
		pointLightPositions[0].x -= 0.01f;
	}

	if (keys[GLFW_KEY_Y])
	{
		pointLightPositions[0].y += 0.01f;
	}

	if (keys[GLFW_KEY_H])
	{
		pointLightPositions[0].y -= 0.01f;
	}
	if (keys[GLFW_KEY_U])
	{
		pointLightPositions[0].z -= 0.1f;
	}
	if (keys[GLFW_KEY_J])
	{
		pointLightPositions[0].z += 0.01f;
	}*/

}

// Is called whenever a key is pressed/released via GLFW
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (keys[GLFW_KEY_L])
	{
		modoDia = !modoDia;
	}


	if (GLFW_KEY_ESCAPE == key && GLFW_PRESS == action)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			keys[key] = false;
		}
	}

	// Tecla para controlar la animacion del presentador 
	if (keys[GLFW_KEY_E])
	{
		animExpositor = !animExpositor;
		if (!animExpositor)
		{
			expositorRotMano = 0.0f;
			expositorRotCabeza = 0.0f;
			expositorTiempo = 0.0f;
		}
	}

	// Tecla para controlar la animacion del visitante
	if (key == GLFW_KEY_V && action == GLFW_PRESS)
	{
		animVisitante = !animVisitante;
		visPlay = animVisitante;
		if (!animVisitante) {
			visPlayIndex = 0;
			vis_i_curr_steps = 0;
			visResetElements();
			visInterpolation();
		}
		printf("Animacion Visitante: %s\n", animVisitante ? "ON" : "OFF");
	}

	//tecla 1 para controlar la rotación del logo de oracle
	if (key == GLFW_KEY_1 && action == GLFW_PRESS)
	{
		playOracle = !playOracle;
		if (!playOracle) tiempoOracle = 0.0f; // Reinicia si se apaga
		printf("Animacion Oracle: %s\n", playOracle ? "ON" : "OFF");
	}
	//tecla 2 para controlar la rotaciónb del logo de amazon
	if (key == GLFW_KEY_2 && action == GLFW_PRESS)
	{
		playAmazon = !playAmazon;
		if (!playAmazon) tiempoAmazon = 0.0f;
		printf("Animacion Amazon: %s\n", playAmazon ? "ON" : "OFF");
	}

	//tecla 3 para controlar la rotaciónb del logo de P&G
	if (key == GLFW_KEY_3 && action == GLFW_PRESS)
	{
		playPG = !playPG;
		if (!playPG) tiempoPG = 0.0f;
		printf("Animacion P&G: %s\n", playPG ? "ON" : "OFF");
	}

	// Tecla P para controlar la animacion del perro robot
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		playPerro = !playPerro;
		if (!playPerro) tiempoPerro = 0.0f;
		printf("Animacion Perro Robot: %s\n", playPerro ? "ON" : "OFF");
	}

	if (keys[GLFW_KEY_SPACE])
	{
		active = !active;
		if (active)
		{
			Light1 = glm::vec3(0.2f, 0.8f, 1.0f);

		}
		else
		{
			Light1 = glm::vec3(0);//Cuado es solo un valor en los 3 vectores pueden dejar solo una componente
		}
	}


}
void Animation() {
	// ANIMACIÓN PRESENTADOR 
	if (animExpositor)
	{
		expositorTiempo += deltaTime;
		expositorRotMano = 25.0f * sin(expositorTiempo * 5.0f);
		expositorRotCabeza = 15.0f * sin(expositorTiempo * 1.5f);
		expositorRotAntebrazo = 20.0f * sin(expositorTiempo * 5.0f);
	}

	// ANIMACIÓN VISITANTE POR KEYFRAMES
	if (visPlay) {
		if (vis_i_curr_steps >= vis_i_max_steps) {
			visPlayIndex++;
			if (visPlayIndex > VIS_MAX_FRAMES - 2) {
				visPlayIndex = 0;
				visResetElements();
			}
			vis_i_curr_steps = 0;
			visInterpolation();
		}
		else {
			visPosX += VisKF[visPlayIndex].visPosXInc;
			visRotY += VisKF[visPlayIndex].visRotYInc;
			vis_i_curr_steps++;
		}
	}

	//ANIMACIONES DE ROTACIÓN DE LOGOS
	if (playOracle) {
		tiempoOracle += deltaTime;
	}
	if (playAmazon) {
		tiempoAmazon += deltaTime;
	}
	if (playPG) {
		tiempoPG += deltaTime;
	}
	if (playPerro) {
		tiempoPerro += deltaTime;
	}

	// Camara de seguridad — gira automaticamente de lado a lado cada 10 segundos
	tiempoCamera += deltaTime;
	rotCamera = 45.0f * sin(tiempoCamera * (glm::pi<float>() / 10.0f));

}

void MouseCallback(GLFWwindow* window, double xPos, double yPos)
{
	if (firstMouse)
	{
		lastX = xPos;
		lastY = yPos;
		firstMouse = false;
	}

	GLfloat xOffset = xPos - lastX;
	GLfloat yOffset = lastY - yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = xPos;
	lastY = yPos;

	camera.ProcessMouseMovement(xOffset, yOffset);
}