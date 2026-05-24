// Medina Villa Samuel 320249538
// Eric Ramirez 423095203
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


//Other includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "ModeloAnimado.h"
#include "Texture.h"


//Function prototypes
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


//pointlights para el puente
glm::vec3 pointLightPositions[] = {
	glm::vec3(0.0f, 2.0f, 0.0f),
	/*glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(0.0f, 0.0f, 0.0f)*/
};

//vertices para modelar el cubo semitransparente
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

//ANIIMACION DEL PRESENTADOR EN UN STAND
float expositorRotMano = 0.0f;
float expositorRotAntebrazo = 0.0f;
float expositorRotCabeza = 0.0f;
float expositorTiempo = 0.0f;
bool animExpositor = true;

//bool animVisitante = true;
float visPosX = 0.0f;
float visRotY = 0.0f;
float tiempoAnimVisitante = 0.0f;
float poseFijaVisitante = 1.1f;

#define VIS_MAX_FRAMES 10 //keyframe para definir las interpolaciones del animador
int vis_i_max_steps = 1200;
int vis_i_curr_steps = 0;

typedef struct _vis_frame {
	float visPosX;    float visPosXInc;
	float visRotY;    float visRotYInc;
	int maxSteps;
} VIS_FRAME;

VIS_FRAME VisKF[VIS_MAX_FRAMES];
int visPlayIndex = 0;
bool visPlay = false;

void visResetElements(void) {
	visPosX = VisKF[0].visPosX;
	visRotY = VisKF[0].visRotY;
}

void visInterpolation(void) {

	vis_i_max_steps = VisKF[visPlayIndex].maxSteps;

	VisKF[visPlayIndex].visPosXInc = (VisKF[visPlayIndex + 1].visPosX - VisKF[visPlayIndex].visPosX) / vis_i_max_steps;
	VisKF[visPlayIndex].visRotYInc = (VisKF[visPlayIndex + 1].visRotY - VisKF[visPlayIndex].visRotY) / vis_i_max_steps;
}

//ANIMACION DE BRAZO ROBOTICO ENCIMA DE LA MESA 
#define BRAZO_MAX_FRAMES 9
int brazo_i_max_steps = 100;
int brazo_i_curr_steps = 0;

typedef struct _brazo_frame {
	float baseRot;     float baseRotInc;     // rtacion de la base (eje Y)
	float hombroRot;   float hombroRotInc;   //rotacion del hombro
	float codoRot;     float codoRotInc;     //rotacion del brazo/codo
	float garraRot;    float garraRotInc;    //rotacion de la garra (muñeca)
	int maxSteps;
} BRAZO_FRAME;

BRAZO_FRAME BrazoKF[BRAZO_MAX_FRAMES];
int brazoPlayIndex = 0;
bool brazoPlay = false;

//valores actuales aplicados al renderizar
float brazo_baseRot = 0.0f;
float brazo_hombroRot = 0.0f;
float brazo_codoRot = 0.0f;
float brazo_garraRot = 0.0f;

void brazoResetElements(void) {
	brazo_baseRot = BrazoKF[0].baseRot;
	brazo_hombroRot = BrazoKF[0].hombroRot;
	brazo_codoRot = BrazoKF[0].codoRot;
	brazo_garraRot = BrazoKF[0].garraRot;
}

void brazoInterpolation(void) {
	brazo_i_max_steps = BrazoKF[brazoPlayIndex].maxSteps;
	BrazoKF[brazoPlayIndex].baseRotInc = (BrazoKF[brazoPlayIndex + 1].baseRot - BrazoKF[brazoPlayIndex].baseRot) / brazo_i_max_steps;
	BrazoKF[brazoPlayIndex].hombroRotInc = (BrazoKF[brazoPlayIndex + 1].hombroRot - BrazoKF[brazoPlayIndex].hombroRot) / brazo_i_max_steps;
	BrazoKF[brazoPlayIndex].codoRotInc = (BrazoKF[brazoPlayIndex + 1].codoRot - BrazoKF[brazoPlayIndex].codoRot) / brazo_i_max_steps;
	BrazoKF[brazoPlayIndex].garraRotInc = (BrazoKF[brazoPlayIndex + 1].garraRot - BrazoKF[brazoPlayIndex].garraRot) / brazo_i_max_steps;
}

//apuntadores de los modelos de los logos
Model* logoOracle;
Model* logoPG;
Model* logoAmazon;

float tiempoOracle = 0.0f;
float tiempoAmazon = 0.0f;
float tiempoPG = 0.0f;

//variables apra animar las rotaciones de los logos
bool playOracle = false;
bool playAmazon = false;
bool playPG = false;

//variables para manipular al perro robot en el stand de CROFI
float tiempoPerro = 0.0f;
bool playPerro = false;

//variables para manipular rotacion y timeline de la animacion de la camara de seguridad
float tiempoCamera = 0.0f;
float rotCamera = 0.0f;

// Variables para los folletos volando
float tiempoFolleto = 0.0f;
const int NUM_FOLLETOS = 2;

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

	//importamos modelos de los logos
	logoOracle = new Model((char*)"Models/logos/oracle.obj");
	logoAmazon = new Model((char*)"Models/logos/amazon.obj");
	logoPG = new Model((char*)"Models/logos/pg.obj");

	//importamos shaders
	Shader lightingShader("Shader/lighting.vs", "Shader/lighting.frag");
	Shader lampShader("Shader/lamp.vs", "Shader/lamp.frag"); 
	Shader emissiveShader("Shader/emissive.vs", "Shader/emissive.frag"); //sol y luna

	//shader para skybox
	Shader skyboxshader("Shader/skybox.vs", "Shader/skybox.frag");

	//shader para animacion con huesos desde mixamo
	Shader shaderAnimacion("Shader/anim.vs", "Shader/anim.frag");


	//Modelos
	//modelo del puente
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

	//stands de asociaciones
	Model stand_crofi((char*)"Models/stands_1/crofi.obj");
	Model stand_aero((char*)"Models/stands_1/aero.obj");
	Model stand_siafi((char*)"Models/stands_1/siafi.obj");

	//modelo del presentador
	Model presentadorCuerpo((char*)"Models/Presentador/cuerpo.obj");
	Model presentadorCabeza((char*)"Models/Presentador/cabeza.obj");
	Model presentadorAntebrazo((char*)"Models/Presentador/antebrazo.obj");
	Model presentadorMano((char*)"Models/Presentador/mano.obj");

	//modelo del brazo robotico
	Model brazoBase((char*)"Models/brazoRobot/base.obj");
	Model brazoBase2((char*)"Models/brazoRobot/base2.obj");
	Model brazoHombro((char*)"Models/brazoRobot/hombro.obj");
	Model brazoBrazo((char*)"Models/brazoRobot/brazo.obj");
	Model brazoGarra((char*)"Models/brazoRobot/garra.obj");

	//pumagua
	Model pumagua((char*)"Models/extras/pumagua/pumagua.obj");

	//luna
	Model luna((char*)"Models/extras/luna/luna.obj");

	//sol
	Model sol((char*)"Models/extras/sol/sol.obj");

	//lamparas del techo (en el modelo vienen las 4 en paralelo)
	Model lampara((char*)"Models/extras/lampara/lampara.obj");

	//camara de seguridad
	Model camBase((char*)"Models/extras/camara/cam_base.obj");
	Model camCabeza((char*)"Models/extras/camara/cam_cabeza.obj");

	//modelo del folleto
	Model folletoModel((char*)"Models/folleto/hojaPapel.obj");

	//modelos con animacion
	ModeloAnimado visitante((GLchar*)"Models/Visitante/visitante.fbx");
	ModeloAnimado perroRobot((GLchar*)"Models/perroRobot/perro.fbx");



	//definicion de la caja para skybox 
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


	//cargamos texturas del skybox
	vector  < const GLchar*> faces;
	faces.push_back("Skybox/left.jpg");
	faces.push_back("Skybox/right.jpg");
	faces.push_back("Skybox/top.jpg");
	faces.push_back("Skybox/bottom.jpg");
	faces.push_back("Skybox/front.jpg");
	faces.push_back("Skybox/back.jpg");

	GLuint cubemapTexture = TextureLoading::LoadCubemap(faces);

	//skybox exterior DIA
	vector<const GLchar*> facesExterior;
	facesExterior.push_back("SkyboxExterior/right.jpg");
	facesExterior.push_back("SkyboxExterior/left.jpg");
	facesExterior.push_back("SkyboxExterior/top.jpg");
	facesExterior.push_back("SkyboxExterior/bottom.jpg");
	facesExterior.push_back("SkyboxExterior/front.jpg");
	facesExterior.push_back("SkyboxExterior/back.jpg");

	GLuint cubemapTextureExterior = TextureLoading::LoadCubemap(facesExterior);



	//skybox interior NOCHE
	vector<const GLchar*> facesNoche;
	facesNoche.push_back("SkyboxNoche/left.jpg");
	facesNoche.push_back("SkyboxNoche/right.jpg");
	facesNoche.push_back("SkyboxNoche/top.jpg");
	facesNoche.push_back("SkyboxNoche/bottom.jpg");
	facesNoche.push_back("SkyboxNoche/front.jpg");
	facesNoche.push_back("SkyboxNoche/back.jpg");

	GLuint cubemapTextureNoche = TextureLoading::LoadCubemap(facesNoche);

	//skybox exterior NOCHE
	vector<const GLchar*> facesExteriorNoche;
	facesExteriorNoche.push_back("SkyboxExteriorNoche/right.jpg");
	facesExteriorNoche.push_back("SkyboxExteriorNoche/left.jpg");
	facesExteriorNoche.push_back("SkyboxExteriorNoche/top.jpg");
	facesExteriorNoche.push_back("SkyboxExteriorNoche/bottom.jpg");
	facesExteriorNoche.push_back("SkyboxExteriorNoche/front.jpg");
	facesExteriorNoche.push_back("SkyboxExteriorNoche/back.jpg");

	GLuint cubemapTextureExteriorNoche = TextureLoading::LoadCubemap(facesExteriorNoche);

	//CUBO SEMITRANSPARENTE PARA FUENTES DE LUZ
	//valores de los KeyFrames para la animacion del visitante
	VisKF[0] = { 0.0f, 0,   0.0f, 0,  800 };  //camina al extremo derecho
	VisKF[1] = { 240.0f, 0,   0.0f, 0,   50 };  //gira a ver stand (0 a 90 grados)
	VisKF[2] = { 240.0f, 0,  90.0f, 0,  200 };
	VisKF[3] = { 240.0f, 0,  90.0f, 0,   50 };  //gira para regresar 
	VisKF[4] = { 240.0f, 0, 180.0f, 0, 1600 };  //camina al otro extremo
	VisKF[5] = { -250.0f, 0, 180.0f, 0,   50 };  //gira a ver stand 
	VisKF[6] = { -250.0f, 0, 270.0f, 0,  200 };
	VisKF[7] = { -250.0f, 0, 270.0f, 0,   50 };
	VisKF[8] = { -250.0f, 0, 360.0f, 0,  950 };  //camina de vuelta al centro
	VisKF[9] = { 0.0f, 0, 360.0f, 0,    1 };

	visResetElements();
	visPlay = true;
	visPlayIndex = 0;
	vis_i_curr_steps = 0;
	visInterpolation();


	//valores de los keyframes del brazo robotico
	BrazoKF[0] = { 0.0f, 0,    0.0f, 0,    0.0f, 0,    0.0f, 0,    100 };  //reposo
	BrazoKF[1] = { 45.0f, 0,    0.0f, 0,    0.0f, 0,    0.0f, 0,    120 };  //gira base 45 grados
	BrazoKF[2] = { 45.0f, 0,   30.0f, 0,   10.0f, 0,    5.0f, 0,    150 };  //baja a recoger (garra se balancea ligero)
	BrazoKF[3] = { 45.0f, 0,   30.0f, 0,   10.0f, 0,   -5.0f, 0,     60 };  //pausa: garra se balancea al otro lado
	BrazoKF[4] = { 45.0f, 0,  -10.0f, 0,    -30.0f, 0,    0.0f, 0,    120 };  //levanta 
	BrazoKF[5] = { -45.0f, 0,  -10.0f, 0,    -30.0f, 0,    5.0f, 0,    180 };  //gira al otro lado 
	BrazoKF[6] = { -45.0f, 0,   28.0f, 0,    8.0f, 0,    5.0f, 0,    150 };  //baja a soltar
	BrazoKF[7] = { -45.0f, 0,    0.0f, 0,    0.0f, 0,    0.0f, 0,    120 };
	BrazoKF[8] = { 0.0f, 0,    0.0f, 0,    0.0f, 0,    0.0f, 0,      1 };

	brazoResetElements();
	brazoPlay = true;
	brazoPlayIndex = 0;
	brazo_i_curr_steps = 0;
	brazoInterpolation();

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

		//matrices de camara y proyeccion — disponibles en todo el loop
		glm::mat4 projection = glm::perspective(camera.GetZoom(), (GLfloat)SCREEN_WIDTH / (GLfloat)SCREEN_HEIGHT, 0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();

		//skybox exterior
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



		// Use cooresponding shader when setting uniforms/drawing objects
		lightingShader.Use();

		glUniform1i(glGetUniformLocation(lightingShader.Program, "diffuse"), 0);
		//glUniform1i(glGetUniformLocation(lightingShader.Program, "specular"),1);

		GLint viewPosLoc = glGetUniformLocation(lightingShader.Program, "viewPos");
		glUniform3f(viewPosLoc, camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);



		//Directional light (Sol en dia / Luna en noche)
		if (modoDia) {
			//SOL
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), 1.3f, -1.1f, 1.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.35f, 0.32f, 0.28f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.80f, 0.76f, 0.68f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.45f, 0.45f, 0.40f);

		}
		else {
			//LUNA 
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.direction"), -1.3f, -1.1f, 1.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.ambient"), 0.08f, 0.10f, 0.18f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.diffuse"), 0.20f, 0.25f, 0.40f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "dirLight.specular"), 0.30f, 0.32f, 0.45f);
		}

		//pointLights[0] — apagado en dia (dirLight ya maneja el sol), activo en noche (luna)
		if (modoDia) {
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), -1.3f, 1.7f, -4.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.0f, 0.0f, 0.0f);
		}
		else {
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].position"), 1.3f, 1.7f, -4.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].ambient"), 0.04f, 0.05f, 0.09f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].diffuse"), 0.18f, 0.20f, 0.32f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, "pointLights[0].specular"), 0.12f, 0.15f, 0.25f);
		}
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].linear"), 0.045f); //usamos los valores de linear y quadratic para distance 100
		//dados en la tabla de pointLights vista en clase de teoria (grupo 05)
		glUniform1f(glGetUniformLocation(lightingShader.Program, "pointLights[0].quadratic"), 0.0075f);




		//spotlight para stands
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.position"), camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.direction"), camera.GetFront().x, camera.GetFront().y, camera.GetFront().z);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.ambient"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.diffuse"), 0.0f, 0.0f, 0.0f);
		glUniform3f(glGetUniformLocation(lightingShader.Program, "spotLight.specular"), 0.0f, 0.0f, 0.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.constant"), 1.0f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.linear"), 0.3f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.quadratic"), 0.7f);
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.cutOff"), glm::cos(glm::radians(12.0f)));
		glUniform1f(glGetUniformLocation(lightingShader.Program, "spotLight.outerCutOff"), glm::cos(glm::radians(18.0f)));


		float spot_intensity = modoDia ? 0.6f : 1.0f;
		float lamp_intensity = modoDia ? 0.0f : 1.0f; //lamparas techo solo en noche


		//luces de las lamparas de Oracle

		glm::vec3 oracleSpots[4] = {
			glm::vec3(-1.3260f, 0.6700f, -3.3580f),  //lampara 0 lateral
			glm::vec3(-1.2140f, 0.6700f, -3.3580f),  //lampara 1 lateral
			glm::vec3(-1.4260f, 0.6650f, -3.1680f),  //lampara 2 fondo
			glm::vec3(-1.4180f, 0.6650f, -3.2800f)   //lampara 3 fondo 
		};

		for (int i = 0; i < 4; i++) {
			std::string base = "standSpots[" + std::to_string(i) + "]";

			//posicion
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".position").c_str()),
				oracleSpots[i].x, oracleSpots[i].y, oracleSpots[i].z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".direction").c_str()),
				0.0f, -1.0f, 0.0f);

			//angulos de apertura 
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".cutOff").c_str()),
				glm::cos(glm::radians(8.0f)));
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".outerCutOff").c_str()),
				glm::cos(glm::radians(30.0f)));

			//color de la luz: blanco 
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".ambient").c_str()),
				0.0f, 0.0f, 0.0f); //sin ambiente para evitar que ilumine fuera del cono y dar efecto exagerado del spotlight
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".diffuse").c_str()),
				1.0f * spot_intensity, 1.0f * spot_intensity, 1.0f * spot_intensity);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".specular").c_str()),
				1.0f * spot_intensity, 1.0f * spot_intensity, 1.0f * spot_intensity);

			//atenuacion 
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".constant").c_str()), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".linear").c_str()), 5.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".quadratic").c_str()), 50.0f);


		}

		//luces de la lamparas de Crofi

		glm::vec3 crofiSpots[4] = {
			glm::vec3(-1.4277f, 0.6700f, -2.6347f),
			glm::vec3(-1.4276f, 0.6700f, -2.7459f),
			glm::vec3(-1.3468f, 0.6800f, -2.5339f), 
			glm::vec3(-1.2356f, 0.6800f, -2.5339f) 
		};

		for (int i = 0; i < 4; i++) {
			std::string base = "standSpots[" + std::to_string(i + 4) + "]";

			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".position").c_str()),
				crofiSpots[i].x, crofiSpots[i].y, crofiSpots[i].z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".direction").c_str()),
				0.0f, -1.0f, 0.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".cutOff").c_str()),
				glm::cos(glm::radians(8.0f)));
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".outerCutOff").c_str()),
				glm::cos(glm::radians(30.0f)));
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".ambient").c_str()),
				0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".diffuse").c_str()),
				1.0f * spot_intensity, 1.0f * spot_intensity, 1.0f * spot_intensity);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".specular").c_str()),
				1.0f * spot_intensity, 1.0f * spot_intensity, 1.0f * spot_intensity);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".constant").c_str()), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".linear").c_str()), 5.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".quadratic").c_str()), 50.0f);
		}


		//luces de las lamparas de amazon 

		glm::vec3 amazonSpots[4] = {
			glm::vec3(-0.0290f, 0.6700f, -3.3583f),  //fondo izquierda
			glm::vec3(0.0822f, 0.6700f, -3.3582f),  //fondo derecha
			glm::vec3(-0.1251f, 0.6700f, -3.2806f),  //lateral 1
			glm::vec3(-0.1251f, 0.6700f, -3.1694f)   //lateral 2
		};

		for (int i = 0; i < 4; i++) {
			std::string base = "standSpots[" + std::to_string(i + 8) + "]";

			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".position").c_str()),
				amazonSpots[i].x, amazonSpots[i].y, amazonSpots[i].z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".direction").c_str()),
				0.0f, -1.0f, 0.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".cutOff").c_str()),
				glm::cos(glm::radians(8.0f)));
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".outerCutOff").c_str()),
				glm::cos(glm::radians(30.0f)));
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".ambient").c_str()),
				0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".diffuse").c_str()),
				1.0f * spot_intensity, 1.0f * spot_intensity, 1.0f * spot_intensity);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".specular").c_str()),
				1.0f * spot_intensity, 1.0f * spot_intensity, 1.0f * spot_intensity);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".constant").c_str()), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".linear").c_str()), 5.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".quadratic").c_str()), 50.0f);
		}



		//luces de las lamparas de unam aero design
		glm::vec3 aeroSpots[4] = {
			glm::vec3(-0.1163f, 0.6700f, -2.6347f),
			glm::vec3(-0.0354f, 0.6700f, -2.5339f),
			glm::vec3(0.0758f, 0.6700f, -2.5339f),
			glm::vec3(-0.1162f, 0.6700f, -2.7459f)
		};

		for (int i = 0; i < 4; i++) {
			std::string base = "standSpots[" + std::to_string(i + 12) + "]";

			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".position").c_str()),
				aeroSpots[i].x, aeroSpots[i].y, aeroSpots[i].z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".direction").c_str()),
				0.0f, -1.0f, 0.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".cutOff").c_str()),
				glm::cos(glm::radians(8.0f)));
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".outerCutOff").c_str()),
				glm::cos(glm::radians(30.0f)));
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".ambient").c_str()),
				0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".diffuse").c_str()),
				2.5f * spot_intensity, 2.5f * spot_intensity, 2.5f * spot_intensity);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".specular").c_str()),
				2.5 * spot_intensity, 2.5f * spot_intensity, 2.5f * spot_intensity);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".constant").c_str()), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".linear").c_str()), 5.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".quadratic").c_str()), 50.0f);
		}

		//luces de las lamparas de SIAFI

		glm::vec3 siafiSpots[4] = {
			glm::vec3(0.9964f, 0.6700f, -2.6347f),
			glm::vec3(1.0773f, 0.6700f, -2.5339f),
			glm::vec3(1.1885f, 0.6700f, -2.5339f),
			glm::vec3(0.9965f, 0.6700f, -2.7459f)
		};

		for (int i = 0; i < 4; i++) {
			std::string base = "standSpots[" + std::to_string(i + 16) + "]";

			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".position").c_str()),
				siafiSpots[i].x, siafiSpots[i].y, siafiSpots[i].z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".direction").c_str()),
				0.0f, -1.0f, 0.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".cutOff").c_str()),
				glm::cos(glm::radians(8.0f)));
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".outerCutOff").c_str()),
				glm::cos(glm::radians(30.0f)));
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".ambient").c_str()),
				0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".diffuse").c_str()),
				2.0f * spot_intensity, 2.0f * spot_intensity, 2.0f * spot_intensity);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".specular").c_str()),
				2.0f * spot_intensity, 2.0f * spot_intensity, 2.0f * spot_intensity);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".constant").c_str()), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".linear").c_str()), 5.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".quadratic").c_str()), 50.0f);
		}

		//luces de las lamparas de P&G

		glm::vec3 pagSpots[4] = {
			glm::vec3(1.0241f, 0.6700f, -3.2806f),
			glm::vec3(1.0241f, 0.6700f, -3.1694f),
			glm::vec3(1.1249f, 0.6700f, -3.3615f),
			glm::vec3(1.2361f, 0.6700f, -3.3615f)
		};

		for (int i = 0; i < 4; i++) {
			std::string base = "standSpots[" + std::to_string(i + 20) + "]";

			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".position").c_str()),
				pagSpots[i].x, pagSpots[i].y, pagSpots[i].z);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".direction").c_str()),
				0.0f, -1.0f, 0.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".cutOff").c_str()),
				glm::cos(glm::radians(8.0f)));
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".outerCutOff").c_str()),
				glm::cos(glm::radians(30.0f)));
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".ambient").c_str()),
				0.0f, 0.0f, 0.0f);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".diffuse").c_str()),
				2.0f * spot_intensity, 2.0f * spot_intensity, 2.0f * spot_intensity);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".specular").c_str()),
				2.0f * spot_intensity, 2.0f * spot_intensity, 2.0f * spot_intensity);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".constant").c_str()), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".linear").c_str()), 5.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".quadratic").c_str()), 50.0f);
		}



		//lamparas de techo — Point Lights
		glm::vec3 lamparasTecho[4] = {
			glm::vec3(-1.2815f, 0.35f, -2.9374f),
			glm::vec3(-0.3998f, 0.35f, -2.9370f),
			glm::vec3(0.3810f, 0.35f, -2.9370f),
			glm::vec3(1.1759f, 0.35f, -2.9370f)
		};

		for (int i = 0; i < 4; i++) {
			std::string base = "pointLights[" + std::to_string(i + 1) + "]";

			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".position").c_str()),
				lamparasTecho[i].x, lamparasTecho[i].y, lamparasTecho[i].z);

			//ambiente muy bajo para que las sombras sean visibles
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".ambient").c_str()),
				0.01f * lamp_intensity, 0.009f * lamp_intensity, 0.006f * lamp_intensity);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".diffuse").c_str()),
				0.85f * lamp_intensity, 0.77f * lamp_intensity, 0.55f * lamp_intensity);
			glUniform3f(glGetUniformLocation(lightingShader.Program, (base + ".specular").c_str()),
				0.4f * lamp_intensity, 0.36f * lamp_intensity, 0.25f * lamp_intensity);

			//atenuacion mas agresiva — sombras mas marcadas en stands
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".constant").c_str()), 1.0f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".linear").c_str()), 2.02f);
			glUniform1f(glGetUniformLocation(lightingShader.Program, (base + ".quadratic").c_str()), 3.6f);
		}


		//set material properties
		glUniform1f(glGetUniformLocation(lightingShader.Program, "material.shininess"), 32.0f);

		// Get the uniform locations
		GLint modelLoc = glGetUniformLocation(lightingShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(lightingShader.Program, "view");
		GLint projLoc = glGetUniformLocation(lightingShader.Program, "projection");

		// Pass the matrices to the shader
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glm::mat4 model(1);

		model = glm::mat4(1);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, -3.0f));
		model = glm::scale(model, glm::vec3(0.005f, 0.005f, 0.005f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glUniform1i(glGetUniformLocation(lightingShader.Program, "transparency"), 0);
		Puente.Draw(lightingShader);


		//stands

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


		//DIBUJAMOS AL PRESENTADOR
		glm::mat4 modelPres = glm::mat4(1);
		modelPres = glm::translate(modelPres, glm::vec3(0.0f, 0.0f, -3.0f));
		modelPres = glm::scale(modelPres, glm::vec3(0.005f, 0.005f, 0.005f));

		//cuerpo 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelPres));
		presentadorCuerpo.Draw(lightingShader);

		//antebrazo 
		glm::mat4 antebrazoMat = modelPres;
		antebrazoMat = glm::translate(antebrazoMat, glm::vec3(-16.23f, 124.96f, -13.10f));
		antebrazoMat = glm::rotate(antebrazoMat, glm::radians(expositorRotAntebrazo), glm::vec3(1.0f, 0.0f, 0.0f));
		antebrazoMat = glm::translate(antebrazoMat, glm::vec3(16.23f, -124.96f, 13.10f));
		antebrazoMat = glm::translate(antebrazoMat, glm::vec3(0.3f, -0.1f, -0.62f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(antebrazoMat));
		presentadorAntebrazo.Draw(lightingShader);

		//cabeza 
		glm::mat4 cabezaMat = modelPres;
		cabezaMat = glm::translate(cabezaMat, glm::vec3(-13.19f, 126.10f, -18.28f));
		cabezaMat = glm::rotate(cabezaMat, glm::radians(expositorRotCabeza), glm::vec3(0.0f, 1.0f, 0.0f));
		cabezaMat = glm::translate(cabezaMat, glm::vec3(13.19f, -126.10f, 18.28f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(cabezaMat));
		presentadorCabeza.Draw(lightingShader);

		//mano 
		glm::mat4 manoMat = antebrazoMat;
		manoMat = glm::translate(manoMat, glm::vec3(-13.99f, 129.99f, -9.76f));
		manoMat = glm::rotate(manoMat, glm::radians(expositorRotMano), glm::vec3(0.0f, 1.0f, 0.0f));
		manoMat = glm::translate(manoMat, glm::vec3(13.99f, -129.99f, 9.76f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(manoMat));
		presentadorMano.Draw(lightingShader);

		//BRAZO ROBOTICO
		glm::mat4 modelBrazo = glm::mat4(1);
		modelBrazo = glm::translate(modelBrazo, glm::vec3(0.0f, 0.0f, -3.0f));
		modelBrazo = glm::scale(modelBrazo, glm::vec3(0.005f, 0.005f, 0.005f));

		//pivotes 
		glm::vec3 pivotePedestal = glm::vec3(-95.23f, 117.33f, -48.66f);
		glm::vec3 pivoteHombro = glm::vec3(-96.10f, 117.48f, -49.04f);
		glm::vec3 pivoteCodo = glm::vec3(-95.63f, 124.29f, -50.42f);
		glm::vec3 pivoteGarra = glm::vec3(-95.26f, 122.01f, -45.30f);

		//NIVEL 0: BASE FIJA 
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelBrazo));
		brazoBase.Draw(lightingShader);

		//NIVEL 1: BASE 2
		glm::mat4 mPedestal = modelBrazo;
		mPedestal = glm::translate(mPedestal, pivotePedestal);
		mPedestal = glm::rotate(mPedestal, glm::radians(brazo_baseRot), glm::vec3(0.0f, 1.0f, 0.0f));
		mPedestal = glm::translate(mPedestal, -pivotePedestal);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mPedestal));
		brazoBase2.Draw(lightingShader);

		//NIVEL 2: HOMBRO 
		glm::mat4 mHombro = mPedestal;
		mHombro = glm::translate(mHombro, pivoteHombro);
		mHombro = glm::rotate(mHombro, glm::radians(brazo_hombroRot), glm::vec3(1.0f, 0.0f, 0.0f));
		mHombro = glm::translate(mHombro, -pivoteHombro);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mHombro));
		brazoHombro.Draw(lightingShader);

		//NIVEL 3: BRAZO/CODO 
		glm::mat4 mBrazo = mHombro;
		mBrazo = glm::translate(mBrazo, pivoteCodo);
		mBrazo = glm::rotate(mBrazo, glm::radians(brazo_codoRot), glm::vec3(1.0f, 0.0f, 0.0f));
		mBrazo = glm::translate(mBrazo, -pivoteCodo);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mBrazo));
		brazoBrazo.Draw(lightingShader);

		//NIVEL 4: GARRA 
		glm::mat4 mGarra = mBrazo;
		mGarra = glm::translate(mGarra, pivoteGarra);
		mGarra = glm::rotate(mGarra, glm::radians(brazo_garraRot), glm::vec3(1.0f, 0.0f, 0.0f));
		mGarra = glm::translate(mGarra, -pivoteGarra);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(mGarra));
		brazoGarra.Draw(lightingShader);

		//VISITANTE CAMINANDO
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
			modelVis = glm::translate(modelVis, glm::vec3(visPosX, 0.0f, 0.0f));
			modelVis = glm::rotate(modelVis, glm::radians(visRotY + 180), glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(modelLocAnim, 1, GL_FALSE, glm::value_ptr(modelVis));

			bool hayDesplazamiento = (std::abs(VisKF[visPlayIndex].visPosXInc) > 0.0001f);
			bool hayGiro = (std::abs(VisKF[visPlayIndex].visRotYInc) > 0.0001f);

			if (hayDesplazamiento || hayGiro)
			{
				tiempoAnimVisitante += deltaTime * 0.8f;
				visitante.Draw(shaderAnimacion, tiempoAnimVisitante);
			}
			else
			{

				visitante.Draw(shaderAnimacion, poseFijaVisitante);
			}

			lightingShader.Use();
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));



			//ANIMACION DEL PERRO ROBOT
			shaderAnimacion.Use(); //reactivar shaderAnimacion antes de dibujar el perro
			glUniformMatrix4fv(viewLocAnim, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projLocAnim, 1, GL_FALSE, glm::value_ptr(projection));
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

			glm::vec3 centroOracle(-255.8f, 136.83f, -48.15f); //coordenadas tomadas de la escena en blender
			glm::vec3 centroPG(239.0f, 149.88f, -41.31f);
			glm::vec3 centroAmazon(7.27f, 141.24f, -44.42f);


			glm::mat4 modelOracle = glm::mat4(1.0f);
			modelOracle = glm::translate(modelOracle, glm::vec3(0.0f, 0.0f, -3.0f));
			modelOracle = glm::scale(modelOracle, glm::vec3(0.005f, 0.005f, 0.005f));
			modelOracle = glm::translate(modelOracle, centroOracle); //mueve pivote al centroOracle

			if (playOracle)
				modelOracle = glm::rotate(modelOracle, tiempoOracle, glm::vec3(0.0f, 1.0f, 0.0f));
			modelOracle = glm::translate(modelOracle, -centroOracle); //regresa pivote

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


		//modelos extras
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


		//modelo de lamparas
		glm::mat4 modelLampara = glm::mat4(1.0f);
		modelLampara = glm::translate(modelLampara, glm::vec3(0.0f, 0.0f, -3.0f));
		modelLampara = glm::scale(modelLampara, glm::vec3(0.005f, 0.005f, 0.005f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"),
			1, GL_FALSE, glm::value_ptr(modelLampara));
		lampara.Draw(lightingShader);

		//camara de seguridad en pilar
		glm::vec3 centroCamCabeza(-202.53f, 156.96f, -65.422f);

		//base fija
		glm::mat4 modelCamBase = glm::mat4(1.0f);
		modelCamBase = glm::translate(modelCamBase, glm::vec3(0.0f, 0.0f, -3.0f));
		modelCamBase = glm::scale(modelCamBase, glm::vec3(0.005f, 0.005f, 0.005f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"),
			1, GL_FALSE, glm::value_ptr(modelCamBase));
		camBase.Draw(lightingShader);

		//cabeza giratoria
		glm::mat4 modelCamCabeza = glm::mat4(1.0f);
		modelCamCabeza = glm::translate(modelCamCabeza, glm::vec3(0.0f, 0.0f, -3.0f));
		modelCamCabeza = glm::translate(modelCamCabeza, centroCamCabeza * 0.005f);     //pivot en espacio OpenGL
		modelCamCabeza = glm::rotate(modelCamCabeza, glm::radians(rotCamera), glm::vec3(0.0f, 1.0f, 0.0f));
		modelCamCabeza = glm::translate(modelCamCabeza, -centroCamCabeza * 0.005f);    // regresa pivot
		modelCamCabeza = glm::scale(modelCamCabeza, glm::vec3(0.005f, 0.005f, 0.005f));
		glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"),
			1, GL_FALSE, glm::value_ptr(modelCamCabeza));
		camCabeza.Draw(lightingShader);

		//Folletos volando
		tiempoFolleto += deltaTime;

		//Pila de folletos sobre la mesa del stand P&G
		//varios folletos apilados 
		float pilaPosX = 1.15f;   //X del stand P&G
		float pilaPosY = 0.539f;  //altura de la mesa
		float pilaPosZ = -3.25f;  //Z del stand P&G

		for (int k = 0; k < 8; k++) {
			glm::mat4 modelPila = glm::mat4(1.0f);
			modelPila = glm::translate(modelPila, glm::vec3(
				pilaPosX + (k % 3) * 0.015f - 0.015f,           //ligero desplazamiento en X
				pilaPosY + k * 0.001f,                          //cada folleto un poco mas arriba
				pilaPosZ + ((k * 7) % 5) * 0.008f - 0.02f       //ligero desplazamiento en Z
			));
			modelPila = glm::rotate(modelPila, glm::radians((float)(k * 13)), glm::vec3(0.0f, 1.0f, 0.0f)); //rotaciones desordenadas
			modelPila = glm::scale(modelPila, glm::vec3(0.0008f, 0.0008f, 0.0008f));

			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelPila));
			folletoModel.Draw(lightingShader);
		}

		//Folletos saliendo desde el stand P&G
		for (int k = 0; k < NUM_FOLLETOS; k++) {
			float t = tiempoFolleto + k * 4.0f;
			float ciclo = fmod(t, 8.0f);

			
			
			float fasevuelo = 4.0f;
			float progreso;
			bool enReposo = false;

			if (ciclo < fasevuelo) {
				progreso = ciclo / fasevuelo;       
			}
			else {
				progreso = 1.0f;                   
				enReposo = true;
			}

			//trayectoria: salen de la pila, vuelan hacia el frente y caen al piso
			float posX = pilaPosX + sin(progreso * 3.1416f) * 0.15f;
			float posY = pilaPosY + sin(progreso * 3.1416f) * 0.25f - progreso * 0.0350f;
			float posZ = pilaPosZ + progreso * 0.55f;

			//rotaciones: animadas mientras vuela, congeladas al aterrizar
			float rotY, rotZ, rotX;

			if (enReposo) {
				//pose fija en el piso: plano y con leve inclinacion para que se vea natural
				rotY = (float)(k * 37);              //cada folleto cae con orientacion distinta
				rotZ = (float)((k * 13) % 20 - 10);  //leve inclinacion lateral
				rotX = 0.0f;                         //plano sobre el piso
			}
			else {
				rotY = t * 60.0f;
				rotZ = sin(t * 1.5f) * 25.0f;
				rotX = cos(t * 1.2f) * 20.0f;
			}

			glm::mat4 modelFolleto = glm::mat4(1.0f);
			modelFolleto = glm::translate(modelFolleto, glm::vec3(posX, posY, posZ));
			modelFolleto = glm::rotate(modelFolleto, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
			modelFolleto = glm::rotate(modelFolleto, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
			modelFolleto = glm::rotate(modelFolleto, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
			modelFolleto = glm::scale(modelFolleto, glm::vec3(0.001f, 0.0010f, 0.001f));

			glUniformMatrix4fv(glGetUniformLocation(lightingShader.Program, "model"), 1, GL_FALSE, glm::value_ptr(modelFolleto));
			folletoModel.Draw(lightingShader);
		}


		//dibujamos los modelos del sol y la luna usando el shader de emissive para que brillen por si mismos
		//dibujo del sol (solo modo dia)
		if (modoDia) {
			emissiveShader.Use();
			glUniformMatrix4fv(glGetUniformLocation(emissiveShader.Program, "view"),
				1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(emissiveShader.Program, "projection"),
				1, GL_FALSE, glm::value_ptr(projection));
			glUniform1f(glGetUniformLocation(emissiveShader.Program, "brightness"), 2.5f); // brillo extra
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(-1.3f, 1.7f, -4.0f));
			model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
			glUniformMatrix4fv(glGetUniformLocation(emissiveShader.Program, "model"),
				1, GL_FALSE, glm::value_ptr(model));
			sol.Draw(emissiveShader);
		}

		//dibujo de la luna (solo modo noche)
		if (!modoDia) {
			emissiveShader.Use();
			glUniformMatrix4fv(glGetUniformLocation(emissiveShader.Program, "view"),
				1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(emissiveShader.Program, "projection"),
				1, GL_FALSE, glm::value_ptr(projection));
			glUniform1f(glGetUniformLocation(emissiveShader.Program, "brightness"), 1.0f); // luna mas tenue
			model = glm::mat4(1);
			model = glm::translate(model, glm::vec3(1.3f, 1.7f, -4.0f));
			model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
			glUniformMatrix4fv(glGetUniformLocation(emissiveShader.Program, "model"),
				1, GL_FALSE, glm::value_ptr(model));
			luna.Draw(emissiveShader);
		}

		// Also draw the lamp object, again binding the appropriate shader
		lampShader.Use();
		// Get location objects for the matrices on the lamp shader (these could be different on a different shader)
		modelLoc = glGetUniformLocation(lampShader.Program, "model");
		viewLoc = glGetUniformLocation(lampShader.Program, "view");
		projLoc = glGetUniformLocation(lampShader.Program, "projection");

		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

		glBindVertexArray(0);

		//dibujo del skybox

		glDepthFunc(GL_LEQUAL);
		skyboxshader.Use();
		glUniform1i(glGetUniformLocation(skyboxshader.Program, "skybox"), 1);
		view = camera.GetViewMatrix();
		glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		//modelo para posicionar y escalar la caja alrededor del puente
		model = glm::mat4(1);
		model = glm::translate(model, glm::vec3(-0.0413f, 0.6800f, -2.9475f)); //centro del puente
		model = glm::scale(model, glm::vec3(1.4619f, 0.1850f, 0.4875f));    //imagenes al tama�o del puente
		glUniformMatrix4fv(glGetUniformLocation(skyboxshader.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));

		glBindVertexArray(skyboxVAO);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, modoDia ? cubemapTexture : cubemapTextureNoche);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
		glDepthFunc(GL_LESS);

		//swap the screen buffers
		glfwSwapBuffers(window);
	}

	//se borra el buffer el skybox

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

	//tecla E para controlar la animacion del presentador 
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

	//pausar/reanudar animaci�n del brazo rob�tico
	if (keys[GLFW_KEY_B])
	{
		brazoPlay = !brazoPlay;
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

	//Tecla F para controlar la animacion de los folletos volando
	
	
	if (key == GLFW_KEY_F && action == GLFW_PRESS)
	{
		tiempoFolleto = 0.0f;
		printf("Animacion Folletos: REINICIADA\n");
	}

}
void Animation() {
	//ANIMACION PRESENTADOR 
	if (animExpositor)
	{
		expositorTiempo += deltaTime;
		expositorRotMano = 25.0f * sin(expositorTiempo * 5.0f);
		expositorRotCabeza = 15.0f * sin(expositorTiempo * 1.5f);
		expositorRotAntebrazo = 20.0f * sin(expositorTiempo * 5.0f);
	}


	//ANIMACION VISITANTE POR KEYFRAMES
	if (visPlay) {
		if (vis_i_curr_steps >= vis_i_max_steps) {
			visPlayIndex++;

			if (visPlayIndex >= VIS_MAX_FRAMES - 1) {
				visPlayIndex = 0;
				vis_i_curr_steps = 0;
				visPosX = 0.0f;
				visRotY = 0.0f;
				visInterpolation();
			}
			else {
				vis_i_curr_steps = 0;
				visInterpolation();
			}
		}
		else {
			visPosX += VisKF[visPlayIndex].visPosXInc;
			visRotY += VisKF[visPlayIndex].visRotYInc;
			vis_i_curr_steps++;
		}
	}

	//ANIMACIONES DE ROTACION DE LOGOS
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


	//ANIMACIÓN BRAZO ROBÓTICO POR KEYFRAMES
	if (brazoPlay) {
		if (brazo_i_curr_steps >= brazo_i_max_steps) {
			brazoPlayIndex++;
			if (brazoPlayIndex >= BRAZO_MAX_FRAMES - 1) {
				brazoPlayIndex = 0;
				brazo_i_curr_steps = 0;
				brazoResetElements();
				brazoInterpolation();
			}
			else {
				brazo_i_curr_steps = 0;
				brazoInterpolation();
			}
		}
		else {
			brazo_baseRot += BrazoKF[brazoPlayIndex].baseRotInc;
			brazo_hombroRot += BrazoKF[brazoPlayIndex].hombroRotInc;
			brazo_codoRot += BrazoKF[brazoPlayIndex].codoRotInc;
			brazo_garraRot += BrazoKF[brazoPlayIndex].garraRotInc;
			brazo_i_curr_steps++;
		}
	}

	//camara de seguridad — gira automaticamente de lado a lado cada 10 segundos
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