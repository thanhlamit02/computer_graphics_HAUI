#include "Angel.h"
#include <GL/Glu.h>
typedef vec4 point4;
typedef vec4 color4;
using namespace std;

GLuint program;
GLuint model_loc;
GLuint projection_loc;
GLuint view_loc;

mat4 projection;
mat4 view;
const int numCube = 36;
const int numCylinder = 96;
point4 numberOfCube[8];
color4 colorCube[8];

point4 arrTriangleVertex[numCube + numCylinder];
color4 arrVertexColor[numCube + numCylinder];
vec3 arrNormalVec[numCube + numCylinder];
point4 numberOfCylinder[18];
color4 colorCylinder[10];

point4 Light_Position(0.0, 10.0, 10.0, 1.0);
color4 AmbientLight_Color(1, 1, 1, 1.0);
color4 DiffuseLight_Color(1.0, 1.0, 1.0, 1.0);
color4 SpecularLight_Color(1.0, 1.0, 1.0, 1.0);

color4 VL_color;
color4 VL_colorDiffuse;
color4 VL_colorSpecular;
float Shininess = 1000.0;

color4 Ambient_color;
color4 Diffuse_color;
color4 Specular_color;

vec4 eye(0, 5, -5, 1);
vec4 at(0, 0, 0, 1);
vec4 up(0, 1, 0, 1);

/*
//Make a wheel
float angle = 5, xpos = -1;
void idle() {
	angle += 5;
	if (xpos <= 1) xpos += 0.001;
	else xpos = xpos - 2;
	glutPostRedisplay();
}
*/

void CaculateVertexAndColorCube()
{

	numberOfCube[0] = point4(-0.5, -0.5, 0.5, 1.0);
	numberOfCube[1] = point4(-0.5, 0.5, 0.5, 1.0);
	numberOfCube[2] = point4(0.5, 0.5, 0.5, 1.0);
	numberOfCube[3] = point4(0.5, -0.5, 0.5, 1.0);
	numberOfCube[4] = point4(-0.5, -0.5, -0.5, 1.0);
	numberOfCube[5] = point4(-0.5, 0.5, -0.5, 1.0);
	numberOfCube[6] = point4(0.5, 0.5, -0.5, 1.0);
	numberOfCube[7] = point4(0.5, -0.5, -0.5, 1.0);

	colorCube[0] = color4(0.0, 0.0, 0.0, 1.0);
	colorCube[1] = color4(1.0, 0.0, 0.0, 1.0);
	colorCube[2] = color4(1.0, 1.0, 0.0, 1.0);
	colorCube[3] = color4(0.0, 1.0, 0.0, 1.0);
	colorCube[4] = color4(0.0, 0.0, 1.0, 1.0);
	colorCube[5] = color4(1.0, 0.0, 1.0, 1.0);
	colorCube[7] = color4(0.0, 1.0, 1.0, 1.0);
}

int Index = 0;

void CreateCubeFace(int a, int b, int c, int d)
{
	vec4 u = numberOfCube[b] - numberOfCube[a];
	vec4 v = numberOfCube[c] - numberOfCube[b];
	vec3 normal = normalize(cross(u, v));

	arrNormalVec[Index] = normal; arrVertexColor[Index] = colorCube[a]; arrTriangleVertex[Index] = numberOfCube[a]; Index++;
	arrNormalVec[Index] = normal; arrVertexColor[Index] = colorCube[a]; arrTriangleVertex[Index] = numberOfCube[b]; Index++;
	arrNormalVec[Index] = normal; arrVertexColor[Index] = colorCube[a]; arrTriangleVertex[Index] = numberOfCube[c]; Index++;
	arrNormalVec[Index] = normal; arrVertexColor[Index] = colorCube[a]; arrTriangleVertex[Index] = numberOfCube[a]; Index++;
	arrNormalVec[Index] = normal; arrVertexColor[Index] = colorCube[a]; arrTriangleVertex[Index] = numberOfCube[c]; Index++;
	arrNormalVec[Index] = normal; arrVertexColor[Index] = colorCube[a]; arrTriangleVertex[Index] = numberOfCube[d]; Index++;
}

void CreateCube()
{
	CreateCubeFace(1, 0, 3, 2);
	CreateCubeFace(2, 3, 7, 6);
	CreateCubeFace(3, 0, 4, 7);
	CreateCubeFace(6, 5, 1, 2);
	CreateCubeFace(4, 5, 6, 7);
	CreateCubeFace(5, 4, 0, 1);
}

void CaculateVertexAndColorCylinder()
{
	float top = 0.5;
	float bot = -top;

	numberOfCylinder[0] = point4(0, top, 0, 1);

	for (int i = 1; i <= 8; i++)
	{
		float _radius = 0.5;
		float rad = (i - 1) * 45.0 / 360 * 2 * M_PI;
		float x = _radius * cosf(rad);
		float z = _radius * sinf(rad);
		numberOfCylinder[i * 2 - 1] = point4(x, top, z, 1);
		numberOfCylinder[i * 2] = point4(x, bot, z, 1);
	}

	numberOfCylinder[17] = point4(0, bot, 0, 1);

	colorCylinder[0] = color4(1.0, 0.0, 0.0, 1.0);
	colorCylinder[1] = color4(1.0, 1.0, 0.0, 1.0);
	colorCylinder[2] = color4(0.0, 1.0, 0.0, 1.0);
	colorCylinder[3] = color4(0.0, 0.0, 1.0, 1.0);
	colorCylinder[4] = color4(1.0, 0.0, 1.0, 1.0);
	colorCylinder[5] = color4(1.0, 0.0, 0.0, 1.0);
	colorCylinder[6] = color4(1.0, 1.0, 0.0, 1.0);
	colorCylinder[7] = color4(0.0, 1.0, 0.0, 1.0);
	colorCylinder[8] = color4(0.0, 0.0, 1.0, 1.0);
	colorCylinder[9] = color4(1.0, 0.0, 1.0, 1.0);
}

void CreateSideFaceCylinder(int a, int b, int c, int d, int _color)
{
	vec4 u = numberOfCylinder[b] - numberOfCylinder[a];
	vec4 v = numberOfCylinder[c] - numberOfCylinder[b];
	vec3 _normalVec = normalize(cross(u, v));

	arrNormalVec[Index] = _normalVec; arrVertexColor[Index] = colorCylinder[_color]; arrTriangleVertex[Index] = numberOfCylinder[a]; Index++;
	arrNormalVec[Index] = _normalVec; arrVertexColor[Index] = colorCylinder[_color]; arrTriangleVertex[Index] = numberOfCylinder[b]; Index++;
	arrNormalVec[Index] = _normalVec; arrVertexColor[Index] = colorCylinder[_color]; arrTriangleVertex[Index] = numberOfCylinder[c]; Index++;
	arrNormalVec[Index] = _normalVec; arrVertexColor[Index] = colorCylinder[_color]; arrTriangleVertex[Index] = numberOfCylinder[a]; Index++;
	arrNormalVec[Index] = _normalVec; arrVertexColor[Index] = colorCylinder[_color]; arrTriangleVertex[Index] = numberOfCylinder[c]; Index++;
	arrNormalVec[Index] = _normalVec; arrVertexColor[Index] = colorCylinder[_color]; arrTriangleVertex[Index] = numberOfCylinder[d]; Index++;
}

void CreateBotTriangle(int a, int b, int c, int _color)
{
	vec4 u = numberOfCylinder[b] - numberOfCylinder[a];
	vec4 v = numberOfCylinder[c] - numberOfCylinder[b];
	vec3 _normalVec = normalize(cross(u, v));

	arrNormalVec[Index] = _normalVec; arrVertexColor[Index] = colorCylinder[_color]; arrTriangleVertex[Index] = numberOfCylinder[a]; Index++;
	arrNormalVec[Index] = _normalVec; arrVertexColor[Index] = colorCylinder[_color]; arrTriangleVertex[Index] = numberOfCylinder[b]; Index++;
	arrNormalVec[Index] = _normalVec; arrVertexColor[Index] = colorCylinder[_color]; arrTriangleVertex[Index] = numberOfCylinder[c]; Index++;
}

void CreateCylinder()
{
	CreateSideFaceCylinder(2, 1, 3, 4, 0);
	CreateSideFaceCylinder(4, 3, 5, 6, 1);
	CreateSideFaceCylinder(6, 5, 7, 8, 2);
	CreateSideFaceCylinder(8, 7, 9, 10, 3);
	CreateSideFaceCylinder(10, 9, 11, 12, 4);
	CreateSideFaceCylinder(12, 11, 13, 14, 5);
	CreateSideFaceCylinder(14, 13, 15, 16, 6);
	CreateSideFaceCylinder(16, 15, 1, 2, 7);

	CreateBotTriangle(2, 17, 16, 9);
	CreateBotTriangle(16, 17, 14, 9);
	CreateBotTriangle(14, 17, 12, 9);
	CreateBotTriangle(12, 17, 10, 9);
	CreateBotTriangle(10, 17, 8, 9);
	CreateBotTriangle(8, 17, 6, 9);
	CreateBotTriangle(6, 17, 4, 9);
	CreateBotTriangle(4, 17, 2, 9);

	CreateBotTriangle(1, 0, 3, 8);
	CreateBotTriangle(3, 0, 5, 8);
	CreateBotTriangle(5, 0, 7, 8);
	CreateBotTriangle(7, 0, 9, 8);
	CreateBotTriangle(9, 0, 11, 8);
	CreateBotTriangle(11, 0, 13, 8);
	CreateBotTriangle(13, 0, 15, 8);
	CreateBotTriangle(15, 0, 1, 8);

}

void GenerateShapes(void)
{
	CaculateVertexAndColorCube();
	CreateCube();

	CaculateVertexAndColorCylinder();
	CreateCylinder();
}

float ConvertToRGB(float color)
{
	return color / 255;
}

color4 ColorToRGB(int Red, int Green, int Blue)
{
	return color4(ConvertToRGB(Red), ConvertToRGB(Green), ConvertToRGB(Blue), 1.0);
}

color4 RGBtoColor(int Red, int Green, int Blue, int x)
{
	return color4(ConvertToRGB(Red), ConvertToRGB(Green), ConvertToRGB(Blue), ConvertToRGB(x));
}

float DEGtoRAD(float deg)
{
	return deg / 360 * 2 * M_PI;
}

void Mix_Color()
{
	Ambient_color = AmbientLight_Color * VL_color;
	Diffuse_color = DiffuseLight_Color * VL_colorDiffuse;
	Specular_color = SpecularLight_Color * VL_colorSpecular;

	glUniform4fv(glGetUniformLocation(program, "AmbientProduct"), 1, Ambient_color);
	glUniform4fv(glGetUniformLocation(program, "DiffuseProduct"), 1, Diffuse_color);
	glUniform4fv(glGetUniformLocation(program, "SpecularProduct"), 1, Specular_color);
	glUniform4fv(glGetUniformLocation(program, "LightPosition"), 1, Light_Position);
	glUniform1f(glGetUniformLocation(program, "Shininess"), Shininess);
}

void CreateMaterial(color4 Ambient, color4 Diffuse, color4 Specular, float Shininess)
{
	VL_color = Ambient;
	VL_colorSpecular = Specular;
	VL_colorDiffuse = Diffuse;
	Shininess = Shininess;
	Mix_Color();
}

void CreateCube(mat4 _symbol, mat4 matrixPhanCap)
{
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, matrixPhanCap * _symbol);
	glDrawArrays(GL_TRIANGLES, 0, numCube);
}

void CreateCylinder(mat4 _symbol, mat4 _matrix)
{
	glUniformMatrix4fv(model_loc, 1, GL_TRUE, _matrix * _symbol);
	glDrawArrays(GL_TRIANGLES, 36, numCylinder);
}

void GenerateGPUBuffer(void)
{
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(arrTriangleVertex) + sizeof(arrVertexColor) + sizeof(arrNormalVec), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(arrTriangleVertex), arrTriangleVertex);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(arrTriangleVertex), sizeof(arrVertexColor), arrVertexColor);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(arrTriangleVertex) + sizeof(arrVertexColor), sizeof(arrNormalVec), arrNormalVec);
}

void SetupShader(void)
{
	program = InitShader("vshader1.glsl", "fshader1.glsl");
	glUseProgram(program);

	GLuint loc_vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(loc_vPosition);
	glVertexAttribPointer(loc_vPosition, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	GLuint loc_vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(loc_vColor);
	glVertexAttribPointer(loc_vColor, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(arrTriangleVertex)));

	GLuint loc_vNormal = glGetAttribLocation(program, "vNormal");
	glEnableVertexAttribArray(loc_vNormal);
	glVertexAttribPointer(loc_vNormal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(arrTriangleVertex) + sizeof(arrVertexColor)));

	model_loc = glGetUniformLocation(program, "Model");
	projection_loc = glGetUniformLocation(program, "Projection");
	view_loc = glGetUniformLocation(program, "View");

	glEnable(GL_DEPTH_TEST);
	glClearColor(33.0 / 255, 114.0 / 255, 160.0 / 255, 1.0);
}

mat4 matrixPhanCap;
color4 vl; //Màu vật liệu
color4 kt; //Màu khuếch tán
color4 pxg; //Mầu phản xạ gương
mat4 symbol; //Matrix vật thể

// Biến dùng để di chuyển
float DiChuyenX = 0;
float DiChuyenZ = 0;
float XeXoay = 0;
float BanhTruocXoay = 0;
float BanhXeXoay = 0;
float MoThungXe = 0;
float MoCuaXeTrai = 0;
float MoCuaXePhai = 0;

/*Hàm tạo bánh xe sau gồm bánh xe trái, bánh xe phải và trục xe sau*/
void BanhSau()
{
	vl = ColorToRGB(30, 20, 30);
	kt = ColorToRGB(35, 30, 40);
	pxg = ColorToRGB(40, 40, 50);
	CreateMaterial(vl, kt, pxg, 1000);

	// Bánh trái sau
	symbol = Translate(0, 0, -1.4) * Scale(1, 1, 0.3) * RotateX(90);
	CreateCylinder(symbol, matrixPhanCap);

	// Bánh phải sau
	symbol = Translate(0, 0, 1.4) * Scale(1, 1, 0.3) * RotateX(90);
	CreateCylinder(symbol, matrixPhanCap);

	//Trục bánh xe
	vl = ColorToRGB(82, 135, 190);
	kt = ColorToRGB(75, 120, 180);
	pxg = ColorToRGB(70, 110, 170);
	CreateMaterial(vl, kt, pxg, 1000);
	symbol = Scale(0.2, 0.2, 3.2) * RotateX(90);
	CreateCylinder(symbol, matrixPhanCap);
}

/*Hàm tạo bánh trước*/
void BanhTruoc()
{
	vl = ColorToRGB(30, 20, 30);
	kt = ColorToRGB(35, 30, 40);
	pxg = ColorToRGB(40, 40, 50);
	CreateMaterial(vl, kt, pxg, 1000);

	symbol = Scale(1, 1, 0.3) * RotateX(90);
	CreateCylinder(symbol, matrixPhanCap);
}

/*Hàm tạo thân xe*/
void ThanXe()
{
	vl = ColorToRGB(10, 100, 220);
	kt = ColorToRGB(30, 120, 200);
	pxg = ColorToRGB(50, 130, 180);
	CreateMaterial(vl, kt, pxg, 1000);

	//Mặt dưới đầu xe
	symbol = Translate(3.05, -0.95, 0) * Scale(2.1, 0.1, 3);
	CreateCube(symbol, matrixPhanCap);

	//Mặt sau đầu xe
	symbol = Translate(2, 0, 0) * Scale(0.1, 2, 3);
	CreateCube(symbol, matrixPhanCap);

	//Mặt trước đầu xe
	symbol = Translate(3.95, -0.4, 0) * Scale(0.1, 1.1, 3);
	CreateCube(symbol, matrixPhanCap);

	//Đèn xe
	vl = ColorToRGB(1, 133, 36);
	kt = ColorToRGB(50, 120, 200);
	pxg = ColorToRGB(60, 130, 180);
	CreateMaterial(vl, kt, pxg, 1000);

	//Đèn bên trái
	symbol = Translate(3.95, -0.4, -1) * Scale(0.3, 0.2, 0.5);
	CreateCube(symbol, matrixPhanCap);
	//Đèn bên phải
	symbol = Translate(3.95, -0.4, 1) * Scale(0.3, 0.2, 0.5);
	CreateCube(symbol, matrixPhanCap);

	//Mặt trên đầu xe
	symbol = Translate(2.55, 0.95, 0) * Scale(1.2, 0.1, 3);
	CreateCube(symbol, matrixPhanCap);

	//Thanh chéo trước đầu xe bên trái
	symbol = Translate(3.53, 0.54, -1.4) * RotateZ(-45) * Scale(1.2, 0.1, 0.2);
	CreateCube(symbol, matrixPhanCap);

	//Thanh chéo trước đầu xe bên trái
	symbol = Translate(3.53, 0.54, 1.4) * RotateZ(-45) * Scale(1.2, 0.1, 0.2);
	CreateCube(symbol, matrixPhanCap);

	vl = ColorToRGB(200, 80, 80);
	kt = ColorToRGB(190, 90, 100);
	pxg = ColorToRGB(180, 100, 120);
	CreateMaterial(vl, kt, pxg, 1000);

	// Thanh dưới thùng xe bên trái
	symbol = Translate(-1, -0.95, -1.25) * Scale(6, 0.1, 0.5);
	CreateCube(symbol, matrixPhanCap);

	//Thanh dưới thùng xe ở giữa
	symbol = Translate(-1, -0.95, 0) * Scale(6, 0.1, 0.5);
	CreateCube(symbol, matrixPhanCap);

	// Thanh dưới thùng xe bên phải
	symbol = Translate(-1, -0.95, 1.25) * Scale(6, 0.1, 0.5);
	CreateCube(symbol, matrixPhanCap);

	//Thanh dưới thùng xe phía sau
	symbol = Translate(-3.75, -0.95, 0) * Scale(0.5, 0.1, 3);
	CreateCube(symbol, matrixPhanCap);

	//Thanh dưới thùng xe phía trước
	symbol = Translate(1.5, -0.95, 0) * Scale(1, 0.1, 3);
	CreateCube(symbol, matrixPhanCap);

}

void CuaXe()
{
	vl = ColorToRGB(175, 100, 0);
	kt = ColorToRGB(160, 120, 20);
	pxg = ColorToRGB(150, 140, 40);
	CreateMaterial(vl, kt, pxg, 1000);

	symbol = Translate(-0.925, 0, 0) * Scale(1.85, 1, 0.1);
	CreateCube(symbol, matrixPhanCap);

	
}

void TayNamCua()
{
	vl = ColorToRGB(60, 100, 0);
	kt = ColorToRGB(70, 120, 20);
	pxg = ColorToRGB(80, 140, 40);
	CreateMaterial(vl, kt, pxg, 1000);

	//Tay nắm cửa
	symbol = Translate(-1.6, 0.155, 0.1) * Scale(0.2, 0.1, 0.02);
	CreateCube(symbol, matrixPhanCap);
}

/*Hàm tạo thùng xe*/
void ThungXe()
{
	vl = ColorToRGB(150, 130, 30);
	kt = ColorToRGB(140, 120, 20);
	pxg = ColorToRGB(130, 110, 10);
	CreateMaterial(vl, kt, pxg, 1000);

	//Mặt dưới
	symbol = Translate(2.75, 0.1, 0) * Scale(5.6, 0.2, 3.6);
	CreateCube(symbol, matrixPhanCap);

	//Mặt trái
	symbol = Translate(2.75, 1, 1.65) * Scale(5.5, 2, 0.1);
	CreateCube(symbol, matrixPhanCap);

	// Mặt phải
	symbol = Translate(2.75, 1, -1.65) * Scale(5.5, 2, 0.1);
	CreateCube(symbol, matrixPhanCap);

	//Mặt trước
	symbol = Translate(5.5, 1, 0) * Scale(0.1, 2, 3.6);
	CreateCube(symbol, matrixPhanCap);

	symbol = Translate(2.75, 1.9, 1.7) * Scale(5.5, 0.2, 0.2);
	CreateCube(symbol, matrixPhanCap);

	symbol = Translate(0, 1, 1.7) * Scale(0.1, 2, 0.2);
	CreateCube(symbol, matrixPhanCap);

	symbol = Translate(2.75, 1.9, -1.7) * Scale(5.5, 0.2, 0.2);
	CreateCube(symbol, matrixPhanCap);

	symbol = Translate(0, 1, -1.7) * Scale(0.1, 2, 0.2);
	CreateCube(symbol, matrixPhanCap);

}

/*Hàm tạo cửa thùng xe*/
void CuaThungXe()
{
	vl = ColorToRGB(150, 110, 10);
	kt = ColorToRGB(130, 110, 10);
	pxg = ColorToRGB(130, 110, 10);
	CreateMaterial(vl, kt, pxg, 1000);

	symbol = Translate(0, -0.9, 0) * Scale(0.1, 1.8, 3.2);
	CreateCube(symbol, matrixPhanCap);
}

/*Hàm tạo vật thể xe tải*/
void truckOpenGL()
{
	matrixPhanCap = mat4();
	matrixPhanCap = matrixPhanCap * Translate(DiChuyenX, 1, DiChuyenZ) * RotateY(XeXoay);
	ThanXe();
	mat4 MatrixTam = matrixPhanCap;
	matrixPhanCap = matrixPhanCap * Translate(-4, -0.85, 0) * RotateZ(MoThungXe);
	ThungXe();

	matrixPhanCap = matrixPhanCap * Translate(0, 2, 0) * RotateZ(-MoThungXe);
	CuaThungXe();

	matrixPhanCap = MatrixTam * Translate(-2, -1.5, 0) * RotateZ(BanhXeXoay);
	BanhSau();

	matrixPhanCap = MatrixTam * Translate(3.9, -0.4, -1.45) * RotateY(-MoCuaXeTrai);
	CuaXe();

	matrixPhanCap = MatrixTam * Translate(3.9, -0.4, -1.65) * RotateY(-MoCuaXeTrai);
	TayNamCua();

	matrixPhanCap = MatrixTam * Translate(3.9, -0.4, 1.45) * RotateY(MoCuaXePhai);
	CuaXe();

	matrixPhanCap = MatrixTam * Translate(3.9, -0.4, 1.45) * RotateY(MoCuaXePhai);
	TayNamCua();

	matrixPhanCap = MatrixTam * Translate(2.5, -1.5, -1.4) * RotateY(BanhTruocXoay) * RotateZ(BanhXeXoay);
	BanhTruoc();

	matrixPhanCap = MatrixTam * Translate(2.5, -1.5, 1.4) * RotateY(BanhTruocXoay) * RotateZ(BanhXeXoay);
	BanhTruoc();
}

void DisplayModels(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	matrixPhanCap = mat4();
	truckOpenGL();

	at = vec4(0, 0, 0, 0);
	eye = vec4(-1, 1, 8, 0);
	view = LookAt(eye, at, up);
	glUniformMatrix4fv(view_loc, 1, GL_TRUE, view);
	projection = Frustum(-0.5, 0.5, -0.5, 0.5, 0.5, 50);
	glUniformMatrix4fv(projection_loc, 1, GL_TRUE, projection);
	glViewport(0, 0, 1000, 1000);
	glutPostRedisplay();
	glutSwapBuffers();
}

void KeyboardControl(unsigned char key, int x, int y)
{
	switch (key) {
	case 033:
		exit(1);
		break;

	case 'i':
		XeXoay += BanhTruocXoay * 0.3 / 7.5;
		BanhXeXoay += (0.3 / (0.5 * 2 * M_PI)) * 360;
		DiChuyenX += cosf(DEGtoRAD(XeXoay)) * 0.3;
		DiChuyenZ += -sinf(DEGtoRAD(XeXoay)) * 0.3;
		break;
	case 'k':
		XeXoay -= BanhTruocXoay * 0.3 / 7.5;
		BanhXeXoay -= (0.3 / (0.5 * 2 * M_PI)) * 360;
		DiChuyenX -= cosf(DEGtoRAD(XeXoay)) * 0.3;
		DiChuyenZ -= -sinf(DEGtoRAD(XeXoay)) * 0.3;
		break;
	case 'j':
		BanhTruocXoay += 5;
		if (BanhTruocXoay >= 30)
		{
			BanhTruocXoay = 30;
		}
		break;
	case 'l':
		BanhTruocXoay -= 5;
		if (BanhTruocXoay <= -30)
		{
			BanhTruocXoay = -30;
		}
		break;
	case 'm':
		MoThungXe += 5;
		if (MoThungXe >= 50)
		{
			MoThungXe = 50;
		}
		break;
	case 'M':
		MoThungXe -= 5;
		if (MoThungXe <= 0)
		{
			MoThungXe = 0;
		}
		break;
	case 'o':
		MoCuaXeTrai += 5;
		if (MoCuaXeTrai >= 50)
		{
			MoCuaXeTrai = 50;
		}
		break;
	case 'O':
		MoCuaXeTrai -= 5;
		if (MoCuaXeTrai <= 0)
		{
			MoCuaXeTrai = 0;
		}
		break;
	case 'p':
		MoCuaXePhai += 5;
		if (MoCuaXePhai >= 50)
		{
			MoCuaXePhai = 50;
		}
		break;
	case 'P':
		MoCuaXePhai -= 5;
		if (MoCuaXePhai <= 0)
		{
			MoCuaXePhai = 0;
		}
		break;
	}
}

/*
void display() {
	//Make a wheel
	//glClearColor(1, 1, 1, 1.0);
	//glClearColor(GL_COLOR_BUFFER_BIT);
	glClearColor(0, 1, 1, 1.0);
	glClearColor(0, 0 ,0, 0.6);
	glPushMatrix();
	glTranslatef(xpos, 0, 0);
	glRotatef(angle, 0, 0, 0.5);
	glColor3f(1, 1, 1);
	glScalef(0.1, 0.1, 0.1);
	glutWireTorus(0.5, 2.0, 30, 30);
	glPopMatrix();

	glutSwapBuffers();
}
*/

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("truckOpenGL");

	glutDisplayFunc(DisplayModels);
	//glutIdleFunc(idle);

	glewInit();

	GenerateShapes();
	GenerateGPUBuffer();
	SetupShader();

	glutDisplayFunc(DisplayModels);
	glutKeyboardFunc(KeyboardControl);

	glutMainLoop();
	return 0;

}