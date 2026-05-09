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

// Posición y rotación del visitante
float visPosX = 0.0f;   // posición en el eje del pasillo
float visRotY = 0.0f;    // rotación del cuerpo 

// Articulaciones
float visMusloDer = 0.0f;
float visMusloIzq = 0.0f;
float visPantDer = 0.0f;
float visPantIzq = 0.0f;
float visBrazoDer = 0.0f;
float visBrazoIzq = 0.0f;
float visCabeza = 0.0f;

// Sistema de KeyFrames del visitante
#define VIS_MAX_FRAMES 22
int vis_i_max_steps = 250;
int vis_i_curr_steps = 0;

typedef struct _vis_frame {
	float visPosX;       float visPosXInc;
	float visRotY;       float visRotYInc;
	float visMusloDer;   float visMusloDerInc;
	float visMusloIzq;   float visMusloIzqInc;
	float visPantDer;    float visPantDerInc;
	float visPantIzq;    float visPantIzqInc;
	float visBrazoDer;   float visBrazoDerInc;
	float visBrazoIzq;   float visBrazoIzqInc;
	float visCabeza;     float visCabezaInc;
} VIS_FRAME;

VIS_FRAME VisKF[VIS_MAX_FRAMES];
int visPlayIndex = 0;
bool visPlay = false;

void visResetElements(void) {
	visPosX = VisKF[0].visPosX;
	visRotY = VisKF[0].visRotY;
	visMusloDer = VisKF[0].visMusloDer;
	visMusloIzq = VisKF[0].visMusloIzq;
	visPantDer = VisKF[0].visPantDer;
	visPantIzq = VisKF[0].visPantIzq;
	visBrazoDer = VisKF[0].visBrazoDer;
	visBrazoIzq = VisKF[0].visBrazoIzq;
	visCabeza = VisKF[0].visCabeza;
}

void visInterpolation(void) {
	VisKF[visPlayIndex].visPosXInc = (VisKF[visPlayIndex + 1].visPosX - VisKF[visPlayIndex].visPosX) / vis_i_max_steps;
	VisKF[visPlayIndex].visRotYInc = (VisKF[visPlayIndex + 1].visRotY - VisKF[visPlayIndex].visRotY) / vis_i_max_steps;
	VisKF[visPlayIndex].visMusloDerInc = (VisKF[visPlayIndex + 1].visMusloDer - VisKF[visPlayIndex].visMusloDer) / vis_i_max_steps;
	VisKF[visPlayIndex].visMusloIzqInc = (VisKF[visPlayIndex + 1].visMusloIzq - VisKF[visPlayIndex].visMusloIzq) / vis_i_max_steps;
	VisKF[visPlayIndex].visPantDerInc = (VisKF[visPlayIndex + 1].visPantDer - VisKF[visPlayIndex].visPantDer) / vis_i_max_steps;
	VisKF[visPlayIndex].visPantIzqInc = (VisKF[visPlayIndex + 1].visPantIzq - VisKF[visPlayIndex].visPantIzq) / vis_i_max_steps;
	VisKF[visPlayIndex].visBrazoDerInc = (VisKF[visPlayIndex + 1].visBrazoDer - VisKF[visPlayIndex].visBrazoDer) / vis_i_max_steps;
	VisKF[visPlayIndex].visBrazoIzqInc = (VisKF[visPlayIndex + 1].visBrazoIzq - VisKF[visPlayIndex].visBrazoIzq) / vis_i_max_steps;
	VisKF[visPlayIndex].visCabezaInc = (VisKF[visPlayIndex + 1].visCabeza - VisKF[visPlayIndex].visCabeza) / vis_i_max_steps;
}






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



	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag");

	// Nuevo shader para skybox
	Shader skyboxshader("Shader/skybox.vs", "Shader/skybox.frag");


	//Modelos

	// Modelos del puente
	Model Puente((char*)"Models/Puente/Puente.obj");
	

	// Modelo del perro robot

	//Model perroCuerpo((char*)"Models/perroRobot/cuerpo.obj");
	//Model perroPDIU((char*)"Models/perroRobot/patadelanteraIzUp.obj");
	//Model perroPDID((char*)"Models/perroRobot/patadelanteraIzDown.obj");
	//Model perroPDDU((char*)"Models/perroRobot/patadelanteraDerUp.obj");
	//Model perroPDDD((char*)"Models/perroRobot/patadelanteraDerDown.obj");
	//Model perroPTIU((char*)"Models/perroRobot/patatraseraIzUp.obj");
	//Model perroPTID((char*)"Models/perroRobot/patatraseraIzDown.obj");
	//Model perroPTDU((char*)"Models/perroRobot/patatraseraDerUp.obj");
	//Model perroPTDD((char*)"Models/perroRobot/patatraseraDerDown.obj");


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



	// Modelo del visitante
	Model visCabezaM((char*)"Models/Visitante/cabeza.obj");
	Model visTorso((char*)"Models/Visitante/torso.obj");
	Model visBrazoDerM((char*)"Models/Visitante/brazoDer.obj");
	Model visBrazoIzqM((char*)"Models/Visitante/brazoIzq.obj");
	Model visMusloDerM((char*)"Models/Visitante/musloDer.obj");
	Model visMusloIzqM((char*)"Models/Visitante/musloIzq.obj");
	Model visPantDerM((char*)"Models/Visitante/pantorillaDer.obj");
	Model visPantIzqM((char*)"Models/Visitante/pantorillaIzq.obj");



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
	
	// IDA: 10 pasos para recorrer el pasillo
	//                    posX     rotY    mDer   mIzq   pDer   pIzq   bDer   bIzq   cab
	VisKF[0] = { -250.0f, 0,  0.0f, 0,  12.0f, 0, -12.0f, 0, -5.0f, 0, -2.0f, 0, -10.0f, 0,  10.0f, 0,  0.0f, 0 };
	VisKF[1] = { -220.0f, 0,  0.0f, 0, -12.0f, 0,  12.0f, 0, -2.0f, 0, -5.0f, 0,  10.0f, 0, -10.0f, 0,  0.0f, 0 };
	VisKF[2] = { -185.0f, 0,  0.0f, 0,  12.0f, 0, -12.0f, 0, -5.0f, 0, -2.0f, 0, -10.0f, 0,  10.0f, 0,  0.0f, 0 };
	VisKF[3] = { -150.0f, 0,  0.0f, 0, -12.0f, 0,  12.0f, 0, -2.0f, 0, -5.0f, 0,  10.0f, 0, -10.0f, 0,  0.0f, 0 };
	VisKF[4] = { -115.0f, 0,  0.0f, 0,  12.0f, 0, -12.0f, 0, -5.0f, 0, -2.0f, 0, -10.0f, 0,  10.0f, 0,  0.0f, 0 };
	VisKF[5] = { -80.0f, 0,  0.0f, 0, -12.0f, 0,  12.0f, 0, -2.0f, 0, -5.0f, 0,  10.0f, 0, -10.0f, 0,  0.0f, 0 };
	VisKF[6] = { -45.0f, 0,  0.0f, 0,  12.0f, 0, -12.0f, 0, -5.0f, 0, -2.0f, 0, -10.0f, 0,  10.0f, 0,  0.0f, 0 };
	VisKF[7] = { -10.0f, 0,  0.0f, 0, -12.0f, 0,  12.0f, 0, -2.0f, 0, -5.0f, 0,  10.0f, 0, -10.0f, 0,  0.0f, 0 };
	VisKF[8] = { 30.0f, 0,  0.0f, 0,  12.0f, 0, -12.0f, 0, -5.0f, 0, -2.0f, 0, -10.0f, 0,  10.0f, 0,  0.0f, 0 };
	VisKF[9] = { 80.0f, 0,  0.0f, 0,   0.0f, 0,   0.0f, 0,  0.0f, 0,  0.0f, 0,   0.0f, 0,   0.0f, 0,  0.0f, 0 };
	VisKF[10] = { 80.0f, 0, -75.0f, 0,  0.0f, 0,   0.0f, 0,  0.0f, 0,  0.0f, 0,   0.0f, 0,   0.0f, 0, -15.0f, 0 };
	VisKF[11] = { 80.0f, 0, -75.0f, 0,  0.0f, 0,   0.0f, 0,  0.0f, 0,  0.0f, 0,   0.0f, 0,   0.0f, 0, -15.0f, 0 };
	VisKF[12] = { 80.0f, 0,  75.0f, 0,  0.0f, 0,   0.0f, 0,  0.0f, 0,  0.0f, 0,   0.0f, 0,   0.0f, 0,  15.0f, 0 };
	VisKF[13] = { 80.0f, 0,  75.0f, 0,  0.0f, 0,   0.0f, 0,  0.0f, 0,  0.0f, 0,   0.0f, 0,   0.0f, 0,  15.0f, 0 };
	VisKF[14] = { 80.0f, 0, 180.0f, 0,  0.0f, 0,   0.0f, 0,  0.0f, 0,  0.0f, 0,   0.0f, 0,   0.0f, 0,   0.0f, 0 };
	VisKF[15] = { 40.0f, 0, 180.0f, 0, -12.0f, 0,  12.0f, 0, -2.0f, 0, -5.0f, 0,  10.0f, 0, -10.0f, 0,  0.0f, 0 };
	VisKF[16] = { -5.0f, 0, 180.0f, 0,  12.0f, 0, -12.0f, 0, -5.0f, 0, -2.0f, 0, -10.0f, 0,  10.0f, 0,  0.0f, 0 };
	VisKF[17] = { -50.0f, 0, 180.0f, 0, -12.0f, 0,  12.0f, 0, -2.0f, 0, -5.0f, 0,  10.0f, 0, -10.0f, 0,  0.0f, 0 };
	VisKF[18] = { -100.0f, 0, 180.0f, 0,  12.0f, 0, -12.0f, 0, -5.0f, 0, -2.0f, 0, -10.0f, 0,  10.0f, 0,  0.0f, 0 };
	VisKF[19] = { -155.0f, 0, 180.0f, 0, -12.0f, 0,  12.0f, 0, -2.0f, 0, -5.0f, 0,  10.0f, 0, -10.0f, 0,  0.0f, 0 };
	VisKF[20] = { -205.0f, 0, 180.0f, 0,  12.0f, 0, -12.0f, 0, -5.0f, 0, -2.0f, 0, -10.0f, 0,  10.0f, 0,  0.0f, 0 };
	VisKF[21] = { -250.0f, 0, 360.0f, 0,  12.0f, 0, -12.0f, 0, -5.0f, 0, -2.0f, 0, -10.0f, 0,  10.0f, 0,  0.0f, 0 };



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


		// Perro robot

		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		/*
		perroCuerpo.Draw(lightingShader);
		perroPDIU.Draw(lightingShader);
		perroPDID.Draw(lightingShader);
		perroPDDU.Draw(lightingShader);
		perroPDDD.Draw(lightingShader);
		perroPTIU.Draw(lightingShader);
		perroPTID.Draw(lightingShader);
		perroPTDU.Draw(lightingShader);
		perroPTDD.Draw(lightingShader);*/

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
			
			float sc = 0.005f;
			glm::vec3 offset(0.0f, 0.0f, -3.0f);

			
			glm::vec3 pivCuello(-0.40f, 126.5f, 22.77f);
			glm::vec3 pivHombroDer(-1.10f, 126.0f, 21.2f);
			glm::vec3 pivHombroIzq(-1.02f, 126.0f, 24.5f);
			glm::vec3 pivCaderaDer(-0.12f, 120.0f, 21.46f);
			glm::vec3 pivCaderaIzq(-0.07f, 120.0f, 24.19f);
			glm::vec3 pivRodillaDer(-0.42f, 111.0f, 21.21f);
			glm::vec3 pivRodillaIzq(-0.46f, 107.8f, 24.32f);


			glm::mat4 visBase = glm::mat4(1);
			visBase = glm::translate(visBase, offset);
			visBase = glm::scale(visBase, glm::vec3(sc));
			visBase = glm::translate(visBase, glm::vec3(visPosX, 0.0f, 0.0f));
			visBase = glm::translate(visBase, glm::vec3(-0.40f, 99.93f, 22.77f));
			visBase = glm::rotate(visBase, glm::radians(visRotY + 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			visBase = glm::translate(visBase, glm::vec3(0.40f, -99.93f, -22.77f));

			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(visBase));
			visTorso.Draw(lightingShader);

			// CABEZA 
			glm::mat4 matCab = visBase;
			matCab = glm::translate(matCab, pivCuello);
			matCab = glm::rotate(matCab, glm::radians(visCabeza), glm::vec3(0.0f, 1.0f, 0.0f));
			matCab = glm::translate(matCab, -pivCuello);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(matCab));
			visCabezaM.Draw(lightingShader);

			// BRAZO DERECHO 
			glm::mat4 matBDer = visBase;
			matBDer = glm::translate(matBDer, pivHombroDer);
			matBDer = glm::rotate(matBDer, glm::radians(visBrazoDer), glm::vec3(0.0f, 0.0f, 1.0f));
			matBDer = glm::translate(matBDer, -pivHombroDer);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(matBDer));
			visBrazoDerM.Draw(lightingShader);

			// BRAZO IZQUIERDO 
			glm::mat4 matBIzq = visBase;
			matBIzq = glm::translate(matBIzq, pivHombroIzq);
			matBIzq = glm::rotate(matBIzq, glm::radians(visBrazoIzq), glm::vec3(0.0f, 0.0f, 1.0f));
			matBIzq = glm::translate(matBIzq, -pivHombroIzq);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(matBIzq));
			visBrazoIzqM.Draw(lightingShader);

			// MUSLO DERECHO 
			glm::mat4 matMDer = visBase;
			matMDer = glm::translate(matMDer, pivCaderaDer);
			matMDer = glm::rotate(matMDer, glm::radians(visMusloDer), glm::vec3(0.0f, 0.0f, 1.0f));
			matMDer = glm::translate(matMDer, -pivCaderaDer);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(matMDer));
			visMusloDerM.Draw(lightingShader);

			// PANTORRILLA DERECHA 
			glm::mat4 matPDer = matMDer; // hereda del muslo
			matPDer = glm::translate(matPDer, pivRodillaDer);
			matPDer = glm::rotate(matPDer, glm::radians(visPantDer), glm::vec3(0.0f, 0.0f, 1.0f));
			matPDer = glm::translate(matPDer, -pivRodillaDer);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(matPDer));
			visPantDerM.Draw(lightingShader);

			// MUSLO IZQUIERDO 
			glm::mat4 matMIzq = visBase;
			matMIzq = glm::translate(matMIzq, pivCaderaIzq);
			matMIzq = glm::rotate(matMIzq, glm::radians(visMusloIzq), glm::vec3(0.0f, 0.0f, 1.0f));
			matMIzq = glm::translate(matMIzq, -pivCaderaIzq);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(matMIzq));
			visMusloIzqM.Draw(lightingShader);

			// PANTORRILLA IZQUIERDA 
			glm::mat4 matPIzq = matMIzq; // hereda del muslo
			matPIzq = glm::translate(matPIzq, pivRodillaIzq);
			matPIzq = glm::rotate(matPIzq, glm::radians(visPantIzq), glm::vec3(0.0f, 0.0f, 1.0f));
			matPIzq = glm::translate(matPIzq, -pivRodillaIzq);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(matPIzq));
			visPantIzqM.Draw(lightingShader);
		}
		////////////////////////////////////////////
		mamp1.Draw(lightingShader);
		mamp2.Draw(lightingShader);
		mamp3.Draw(lightingShader);
		mamp4.Draw(lightingShader);
		mamp5.Draw(lightingShader);
		mamp6.Draw(lightingShader);
		mamp7.Draw(lightingShader);
		mamp8.Draw(lightingShader);

		pumagua.Draw(lightingShader);


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
		expositorRotMano = 25.0f * sin(expositorTiempo * 5.0f );
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
			visMusloDer += VisKF[visPlayIndex].visMusloDerInc;
			visMusloIzq += VisKF[visPlayIndex].visMusloIzqInc;
			visPantDer += VisKF[visPlayIndex].visPantDerInc;
			visPantIzq += VisKF[visPlayIndex].visPantIzqInc;
			visBrazoDer += VisKF[visPlayIndex].visBrazoDerInc;
			visBrazoIzq += VisKF[visPlayIndex].visBrazoIzqInc;
			visCabeza += VisKF[visPlayIndex].visCabezaInc;
			vis_i_curr_steps++;
		}
	}

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