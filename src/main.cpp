
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <list>
#include <ctime>
#include <sstream>
#include <map>
// Nagłówki
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <SFML/System/Time.hpp>

const GLchar* vertexSource = R"glsl(
#version 150 core
in vec3 position;
in vec2 atexCoord;
in vec3 normal;
out vec2 texCoord;
out vec3 FragPos;
out vec3 outNormal;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main(){
  texCoord = atexCoord;
  
  gl_Position = proj * view * model * vec4(position, 1.0);
  FragPos = vec3(model * vec4(position, 1.0));
}
)glsl";


const GLchar* fragmentSource = R"glsl(
#version 150 core
uniform sampler2D texture1;
uniform vec3 lightPos;
in vec3 FragPos;
in vec3 outNormal;
in vec2 texCoord;
out vec4 Color;

void main()
{
  float ambientStrength = 0.5;
  vec3 ambientlightColor = vec3(1.0, 1.0, 1.0);
  vec4 ambient = ambientStrength * vec4(ambientlightColor, 1.0);
  vec3 difflightColor = vec3(1.0, 1.0, 1.0);
  vec3 norm = normalize(outNormal);
  vec3 lightDir = normalize(lightPos - FragPos);
  float diff = max(dot(norm, lightDir), 0.0);
  vec3 diffuse = diff * difflightColor;
  Color = (ambient + vec4(diffuse, 1.0)) * texture(texture1, texCoord);
}
)glsl";

bool firstMouse = true;
int lastX, lastY;
double yaw = 45.0;
double pitch = 0.0;

glm::vec3 cameraPos = glm::vec3(0.3f, 0.3f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);



void ustawKamereMysz(GLint uniView, sf::Int64 time, sf::Window& window) {
  sf::Vector2i localPosition = sf::Mouse::getPosition(window);
  sf::Vector2i position;
  bool relocation = false;

  if (localPosition.x <= 0) {
    position.x = window.getSize().x - 1;
    position.y = localPosition.y;
    relocation = true;
  }
  if (localPosition.x >= window.getSize().x - 1) {
    position.x = 0;
    position.y = localPosition.y;
    relocation = true;
  }
  if (localPosition.y <= 0) {
    position.y = window.getSize().y;
    position.x = localPosition.x;
    relocation = true;
  }
  if (localPosition.y >= window.getSize().y - 1) {
    position.y = 0;
    position.x = localPosition.x;
    relocation = true;
  }
  if (relocation) {
    sf::Mouse::setPosition(position, window);
    firstMouse = true;
    localPosition = sf::Mouse::getPosition(window);
  }
  
  
  if (firstMouse) {
    lastX = localPosition.x;
    lastY = localPosition.y;
    firstMouse = false;
  }

  double xoffset = localPosition.x - lastX;
  double yoffset = localPosition.y - lastY;
  lastX = localPosition.x;
  lastY = localPosition.y;

  double sensitivity = 0.001;
  double cameraSpeed = 0.003 * time;
  
  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yaw += xoffset * cameraSpeed;
  pitch -= yoffset * cameraSpeed;

  if (pitch > 89.0) {
    pitch = 89.0;
  }
  if (pitch < -89.0) {
    pitch = -89.0;
  }

  glm::vec3 front;
  front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
  front.y = sin(glm::radians(pitch));
  front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
  cameraFront = glm::normalize(front);

  glm::mat4 view;
  view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
  glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
}

void StereoProjection(GLuint shaderProgram_,float _left, float _right, float _bottom, float _top, float _near, float _far, float _zero_plane, float _dist, float _eye)
{
  //    Perform the perspective projection for one eye's subfield.
  //    The projection is in the direction of the negative z-axis.
  //            _left=-6.0;
  //            _right=6.0;
  //            _bottom=-4.8;
  //             _top=4.8;
  //    [default: -6.0, 6.0, -4.8, 4.8]
  //    left, right, bottom, top = the coordinate range, in the plane of zero parallax setting,
  //         which will be displayed on the screen.
  //         The ratio between (right-left) and (top-bottom) should equal the aspect
  //    ratio of the display.


  //                  _near=6.0;
  //                  _far=-20.0;
  //    [default: 6.0, -6.0]
  //    near, far = the z-coordinate values of the clipping planes.

  //                  _zero_plane=0.0;
  //    [default: 0.0]
  //    zero_plane = the z-coordinate of the plane of zero parallax setting.

  //    [default: 14.5]
  //                     _dist=10.5;
  //   dist = the distance from the center of projection to the plane of zero parallax.

  //    [default: -0.3]
  //                 _eye=-0.3;
  //    eye = half the eye separation; positive for the right eye subfield,
  //    negative for the left eye subfield.

  float   _dx = _right - _left;
  float   _dy = _top - _bottom;

  float   _xmid = (_right + _left) / 2.0;
  float   _ymid = (_top + _bottom) / 2.0;

  float   _clip_near = _dist + _zero_plane - _near;
  float   _clip_far = _dist + _zero_plane - _far;

  float  _n_over_d = _clip_near / _dist;

  float   _topw = _n_over_d * _dy / 2.0;
  float   _bottomw = -_topw;
  float   _rightw = _n_over_d * (_dx / 2.0 - _eye);
  float   _leftw = _n_over_d * (-_dx / 2.0 - _eye);

  // Create a fustrum, and shift it off axis
  glm::mat4 proj=glm::frustum(_leftw, _rightw, _bottomw, _topw, _clip_near, _clip_far);

  proj = glm::translate(proj, glm::vec3(-_xmid - _eye, -_ymid, 0));

  GLint uniProj = glGetUniformLocation(shaderProgram_, "proj");
  glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
}

unsigned int vertices_size;

int loadVertices(GLuint vbo, GLuint eb) {
  std::list<float> vertices;
  std::list<unsigned int> indices;

  std::ifstream file("/home/js/cpp/grafika/src/test.obj");
  std::string line;
  if (file.is_open()) {
    while (std::getline(file, line)) {
      if (line[0] == 'v') {
        std::string::size_type s;
        line = line.substr(1, line.length()-1);
        double d = std::stof(line, &s);
        vertices.push_back(d);
        line = line.substr(s);
        d = std::stof(line, &s);
        vertices.push_back(d);
        d = std::stof(line, &s);
        line = line.substr(s);
        d = std::stof(line, &s);
        vertices.push_back(d);
      } else if (line[0] == 'f') {
        std::string::size_type s;
        line = line.substr(1, line.length()-1);
        unsigned int i = std::stoul(line, &s);
        indices.push_back(--i);
        line = line.substr(s);
        i = std::stoul(line, &s);
        indices.push_back(--i);
        line = line.substr(s);
        i = std::stoul(line, &s);
        indices.push_back(--i);
        line = line.substr(s);
      }
    }
    file.close();
  } else {
    std::cout << "loadVertices ERROR" << std::endl;
  }
  float* ver = new float[vertices.size()];
  unsigned int* ind = new unsigned int[indices.size()];
  int i = 0;
  for (float const &k : vertices) {
    ver[i++] = k;
  }
  i = 0;
  for (int const &k : indices) {
    ind[i++] = k;
  }
  std::cout << "vertex size: " << vertices.size() << std::endl;
  std::cout << "indices size: " << indices.size() << std::endl;
  
  int indices_size = indices.size();
  vertices_size = vertices.size();

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), ver, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eb);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), ind, GL_STATIC_DRAW);

  delete [] ver;
  delete [] ind;

  return indices_size;
}




bool LoadModelOBJNormalsCoord(int& punkty_, const char* filename, int buffer, std::map<std::string, std::vector<std::pair<int ,int>>>& map)
{
	int vert_num = 0;
	int triangles = 0;
	int normals = 0;
	int coord_num = 0;
  int textures_num = 0;

	std::ifstream myReadFile;
	myReadFile.open(filename);
	std::string output;
	if (myReadFile.is_open()) {
		while (!myReadFile.eof()) {
			myReadFile >> output;
			if (output == "v") vert_num++;
			if (output == "f") triangles++;
			if (output == "vn") normals++;
			if (output == "vt") coord_num++;
      if (output == "usemt") textures_num++;
		}
	}

	myReadFile.close();
	myReadFile.open(filename);


	float **vert;
	vert = new float *[vert_num]; //przydzielenie pamięci na w wierszy

	for (int i = 0; i < vert_num; i++)
		vert[i] = new float[3];


	int **trian;
	trian = new int *[triangles]; //przydzielenie pamięci na w wierszy

	for (int i = 0; i < triangles; i++)
		trian[i] = new int[9];

	float **norm;
	norm = new float *[normals]; //przydzielenie pamięci na w wierszy

	for (int i = 0; i < normals; i++)
		norm[i] = new float[3];

	float **coord;
	coord = new float *[coord_num]; //przydzielenie pamięci na w wierszy

	for (int i = 0; i < coord_num; i++)
		coord[i] = new float[2];

	int licz_vert = 0;
	int licz_triang = 0;
	int licz_normals = 0;
  int licz_coord = 0;

  std::string lastMtl = "";
  int mtlCounter = 0;
  int begin = 0;

  while (!myReadFile.eof()) {
    output = "";
    myReadFile >> output;
    if (output == "vn") {
      myReadFile >> norm[licz_normals][0];
      myReadFile >> norm[licz_normals][1];
      myReadFile >> norm[licz_normals][2];
      licz_normals++; 
    }
    if (output == "v") {
      myReadFile >> vert[licz_vert][0];
      myReadFile >> vert[licz_vert][1];
      myReadFile >> vert[licz_vert][2];
      licz_vert++; 
    }
    if (output == "vt") { 
      myReadFile >> coord[licz_coord][0];
      myReadFile >> coord[licz_coord][1];
      licz_coord++; 
    }

    if (output == "f") {

      mtlCounter += 3;
      for (int i = 0; i < 9; i += 3)
      {
        std::string s;
        myReadFile >> s;
				std::stringstream ss(s);

				std::vector <std::string> el;
				std::string item;


				while (getline(ss, item, '/')) {
					el.push_back(item);
				}
				trian[licz_triang][i] = std::stoi(el[0]);
				trian[licz_triang][i + 1] = std::stoi(el[1]);
				trian[licz_triang][i + 2] = std::stoi(el[2]);
			}
		licz_triang++;
		}

    if (output == "usemtl") {
      if (lastMtl != "") {
        map["/home/js/cpp/grafika/src/" + lastMtl].push_back(std::pair<int, int>(begin, mtlCounter));
        begin += mtlCounter;
        mtlCounter = 0;
      }
      std::string file_name = std::string(filename);
      myReadFile >> output;
      std::size_t pos = file_name.find(".obj");
      std::ifstream mtlFile;
      mtlFile.open(file_name.substr(0, pos) + ".mtl");
      if (mtlFile.is_open()) {
        std::string line;
        std::string name;
        while (!mtlFile.eof()) {
          mtlFile >> name;
          if (name == "newmtl") {
            mtlFile >> name;
            if (name == output) {
              for (int i = 0; i < 9; ++i) {
                std::getline(mtlFile, line);
              }
              mtlFile >> output;
              mtlFile >> output;
              lastMtl = output;
              if (map.find("/home/js/cpp/grafika/src/" + output) == map.end()) {
                map["/home/js/cpp/grafika/src/" + output] = std::vector<std::pair<int, int>>();  
              } 
              break;
            }
          }
        }
        mtlFile.close();
      }
    }
	}

  map["/home/js/cpp/grafika/src/" + lastMtl].push_back(std::pair<int, int>(begin, mtlCounter));
	GLfloat *vertices = new GLfloat[triangles * 24];

	int vert_current = 0;

	for (int i = 0; i < triangles; i++)
	{
		vertices[vert_current] = vert[trian[i][0] - 1][0];
		vertices[vert_current + 1] = vert[trian[i][0] - 1][1];
		vertices[vert_current + 2] = vert[trian[i][0] - 1][2];
		vertices[vert_current + 3] = norm[trian[i][2] - 1][0];
		vertices[vert_current + 4] = norm[trian[i][2] - 1][1];
		vertices[vert_current + 5] = norm[trian[i][2] - 1][2];
		vertices[vert_current + 6] = coord[trian[i][1] - 1][0];
		vertices[vert_current + 7] = coord[trian[i][1] - 1][1];

		vertices[vert_current + 8] = vert[trian[i][3] - 1][0];
		vertices[vert_current + 9] = vert[trian[i][3] - 1][1];
		vertices[vert_current + 10] = vert[trian[i][3] - 1][2];
		vertices[vert_current + 11] = norm[trian[i][5] - 1][0];
		vertices[vert_current + 12] = norm[trian[i][5] - 1][1];
		vertices[vert_current + 13] = norm[trian[i][5] - 1][2];
		vertices[vert_current + 14] = coord[trian[i][4] - 1][0];
		vertices[vert_current + 15] = coord[trian[i][4] - 1][1];

		vertices[vert_current + 16] = vert[trian[i][6] - 1][0];
		vertices[vert_current + 17] = vert[trian[i][6] - 1][1];
		vertices[vert_current + 18] = vert[trian[i][6] - 1][2];
		vertices[vert_current + 19] = norm[trian[i][8] - 1][0];
		vertices[vert_current + 20] = norm[trian[i][8] - 1][1];
		vertices[vert_current + 21] = norm[trian[i][8] - 1][2];
		vertices[vert_current + 22] = coord[trian[i][7] - 1][0];
		vertices[vert_current + 23] = coord[trian[i][7] - 1][1];

		vert_current += 24;
	}

	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat)*triangles * 24, vertices, GL_STATIC_DRAW);

	punkty_ = triangles * 3;

	delete vertices;

	for (int i = 0; i < vert_num; i++)
		delete[] vert[i];
	delete[] vert;

	for (int i = 0; i < triangles; i++)
		delete[] trian[i];
	delete[] trian;

	for (int i = 0; i < normals; i++)
		delete[] norm[i];
	delete[] norm;

	for (int i = 0; i < coord_num; i++)
		delete[] coord[i];
	delete[] coord;

	return 0;
}


int main() {

  sf::ContextSettings settings;
  settings.depthBits = 24;
  settings.stencilBits = 8;
  // Okno renderingu

  sf::Window window(sf::VideoMode(1300, 900, 32), "OpenGL", sf::Style::Titlebar | sf::Style::Close, settings);

	// Inicjalizacja GLEW
	glewExperimental = GL_TRUE;
	glewInit();

  window.setMouseCursorGrabbed(true);
  window.setMouseCursorVisible(false);
  window.setFramerateLimit(60);

  int indices_size = 0;


  GLuint vao;
  glGenVertexArrays(1, &vao);

  GLuint vbo;
  glGenBuffers(1, &vbo);

  std::map<std::string, std::vector<std::pair<int ,int>>> map;

  bool b = LoadModelOBJNormalsCoord(indices_size, "/home/js/cpp/grafika/src/trzy.obj", vbo, map);

  int textures_size = map.size();

  // Utworzenie i skompilowanie shadera wierzchołków
  GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertexShader, 1, &vertexSource, NULL);
  glCompileShader(vertexShader);

  char infoLog[512];

  GLint status;
  glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
  if (!status) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR " << status << std::endl;
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION::FAILED\n" << infoLog << std::endl;
	} else {
		std::cout << "Compilation vertexShader OK" << std::endl;
	}

	// Utworzenie i skompilowanie shadera fragmentów

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	if (!status) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR " << status << std::endl;
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION::FAILED\n" << infoLog << std::endl;
	} else {
		std::cout << "Compilation fragmentShader OK" << std::endl;
	}

	// Zlinkowanie obu shaderów w jeden wspólny program

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	//glBindFragDataLocation(shaderProgram, 0, "outColor");
	
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// Specifikacja formatu danych wierzchołkowych

  glBindVertexArray(vao);

  GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
  GLint normAttrib = glGetAttribLocation(shaderProgram, "normal");
  GLint texCoord = glGetAttribLocation(shaderProgram, "atexCoord");
  glEnableVertexAttribArray(posAttrib);
  glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), (void*)0);
  glEnableVertexAttribArray(normAttrib);
  glVertexAttribPointer(normAttrib, 3, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), (void*)(sizeof(GL_FLOAT)*3));
  glEnableVertexAttribArray(texCoord);
  glVertexAttribPointer(texCoord, 2, GL_FLOAT, GL_FALSE, 8*sizeof(GL_FLOAT), (void*)(sizeof(GL_FLOAT)*6));

  glm::vec3 lightPos(10.0f, 10.0f, 10.0f);
  GLint uniLightPos = glGetUniformLocation(shaderProgram, "lightPos");
  glUniform3fv(uniLightPos, 1, glm::value_ptr(lightPos));

  //================================== TEXTURES
  std::vector<std::string> keys;
  for (const auto& [key, value] : map) {
    keys.push_back(key);
  }
  GLint tex_pos = glGetUniformLocation(shaderProgram, "texture1");
  glUniform1i(tex_pos, 0);
  GLuint *textures = new GLuint[textures_size];
  glGenTextures(textures_size, textures);
  for (int i = 0; i < textures_size; i++) {
    glBindTexture(GL_TEXTURE_2D, textures[i]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load(keys[i].c_str(), &width, &height, &nrChannels, 0);
    //std::cout << "widht: " << width << " " << "height: " << height << " nrChannels: " << nrChannels <<  std::endl;
    if (data) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
      glGenerateMipmap(GL_TEXTURE_2D);
    } else {
      std::cout << "Failed to load texture: " << keys[i] << std::endl;
    }
    stbi_image_free(data);

    glUniform1i(tex_pos, 0);
  }


  glm::mat4 model = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 0.0f, 1.0f));

  GLint uniTrans = glGetUniformLocation(shaderProgram, "model");
  glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(model));

  glm::mat4 view;

  
  view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

  GLint uniView = glGetUniformLocation(shaderProgram, "view");
  glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

  glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.06f, 100.0f);
  GLint uniProj = glGetUniformLocation(shaderProgram, "proj");

  glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

  sf::Clock clock;
  sf::Time time;


  // Rozpoczęcie pętli zdarzeń

	bool running = true;

  glEnable(GL_DEPTH_TEST);
  int licznik = 0;


  int mode = 2;
  float zero_plane = 0.0;
  float dist = 13;
  float eye = 0.05;



  
  while (running) {
    time = clock.restart();
    licznik++;
    float cameraSpeed = 0.00005f * time.asMicroseconds();
    float ffps = 1000000 / time.asMicroseconds();
    if (licznik > ffps) {
      window.setTitle(std::to_string(ffps));
      licznik = 0;
    }
    sf::Event windowEvent;
		while (window.pollEvent(windowEvent)) {
			switch (windowEvent.type) {
				case sf::Event::Closed:
          running = false;
          break;
        case sf::Event::MouseMoved:
          ustawKamereMysz(uniView, time.asMicroseconds(), window);
          break;
        case sf::Event::KeyPressed:
          switch (windowEvent.key.code) {
            case sf::Keyboard::Escape:
              running = false;
              break;
            case sf::Keyboard::W:
              cameraPos += cameraSpeed * cameraFront;
              view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
              uniView = glGetUniformLocation(shaderProgram, "view");
              glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
              break;
            case sf::Keyboard::A:
              cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
              view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
              uniView = glGetUniformLocation(shaderProgram, "view");
              glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
              break;
            case sf::Keyboard::S:
              cameraPos -= cameraSpeed * cameraFront;
              view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
              uniView = glGetUniformLocation(shaderProgram, "view");
              glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
              break;
            case sf::Keyboard::D:
              cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
              view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
              uniView = glGetUniformLocation(shaderProgram, "view");
              glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
              break;
            case sf::Keyboard::Space:
              cameraPos.y += cameraSpeed;
              view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
              uniView = glGetUniformLocation(shaderProgram, "view");
              glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
              break;
            case sf::Keyboard::Z:
              cameraPos.y -= cameraSpeed;
              view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
              uniView = glGetUniformLocation(shaderProgram, "view");
              glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
              break; 
            case sf::Keyboard::U:
              zero_plane += 0.5;
              break;
            case sf::Keyboard::I:
              dist += 0.1;
              break;
            case sf::Keyboard::O:
              eye += 0.1;
              break;
            case sf::Keyboard::J:
              zero_plane -= 0.5;
              break;
            case sf::Keyboard::K:
              dist -= 0.1;
              break;
            case sf::Keyboard::L:
              eye -= 0.1;
              break;
            case sf::Keyboard::Num1:
              mode = 0;
              break;
            case sf::Keyboard::Num2:
              mode = 1;
              break;
            case sf::Keyboard::Num3:
              mode = 2;
              break;
          }
          break;
      }
    } 


    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

    switch (mode) {
      case 0:
        glViewport(0, 0, window.getSize().x, window.getSize().y);
        glDrawBuffer(GL_BACK_LEFT);
        StereoProjection(shaderProgram, -6, 6, -4.8, 4.8, 12.99, -100, zero_plane, dist, -eye);
        glColorMask(true, false, false, false);
        for (int i = 0; i < textures_size; ++i) {
          glActiveTexture(GL_TEXTURE0);
          glBindTexture(GL_TEXTURE_2D, textures[i]);
          glUniform1i(tex_pos, 0);
          for (const auto& v : map[keys[i]]) {
            glDrawArrays(GL_TRIANGLES, v.first, v.second);
          }
        }
        glClear(GL_DEPTH_BUFFER_BIT);
        glDrawBuffer(GL_BACK_RIGHT);
        StereoProjection(shaderProgram, -6, 6, -4.8, 4.8, 12.99, -100, zero_plane, dist, eye);
        glColorMask(false, false, true, false);
        for (int i = 0; i < textures_size; ++i) {
          glActiveTexture(GL_TEXTURE0);
          glBindTexture(GL_TEXTURE_2D, textures[i]);
          glUniform1i(tex_pos, 0);
          for (const auto& v : map[keys[i]]) {
            glDrawArrays(GL_TRIANGLES, v.first, v.second);
          }
        }
        glColorMask(true, true, true, true);
        break;
      case 1:
        glViewport(0, 0, window.getSize().x/2, window.getSize().y);
        glDrawBuffer(GL_BACK_LEFT);
        StereoProjection(shaderProgram, -6, 6, -4.8, 4.8, 12.99, -100, zero_plane, dist, -eye);
        for (int i = 0; i < textures_size; ++i) {
          glActiveTexture(GL_TEXTURE0);
          glBindTexture(GL_TEXTURE_2D, textures[i]);
          glUniform1i(tex_pos, 0);
          for (const auto& v : map[keys[i]]) {
            glDrawArrays(GL_TRIANGLES, v.first, v.second);
          }
        }
        glViewport(window.getSize().x/2, 0, window.getSize().x/2, window.getSize().y);
        glClear(GL_DEPTH_BUFFER_BIT);
        glDrawBuffer(GL_BACK_RIGHT);
        StereoProjection(shaderProgram, -6, 6, -4.8, 4.8, 12.99, -100, zero_plane, dist, eye);
        for (int i = 0; i < textures_size; ++i) {
          glActiveTexture(GL_TEXTURE0);
          glBindTexture(GL_TEXTURE_2D, textures[i]);
          glUniform1i(tex_pos, 0);
          for (const auto& v : map[keys[i]]) {
            glDrawArrays(GL_TRIANGLES, v.first, v.second);
          }
        }
        break;
      case 2:
        glViewport(0, 0, window.getSize().x, window.getSize().y);

        for (int i = 0; i < textures_size; ++i) {
          glActiveTexture(GL_TEXTURE0);
          glBindTexture(GL_TEXTURE_2D, textures[i]);
          glUniform1i(tex_pos, 0);
          for (const auto& v : map[keys[i]]) {
            glDrawArrays(GL_TRIANGLES, v.first, v.second);
          }
        }
        break;
    }
    window.display();
  }

  // Kasowanie programu i czyszczenie buforów

	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
}
