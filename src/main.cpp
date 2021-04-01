
#include <iostream>
#include <stdio.h>
// Nagłówki
#include <GL/glew.h>
#include <SFML/Window.hpp>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SFML/System/Time.hpp>

// Kody shaderów

const GLchar* vertexSource = R"glsl(
#version 150 core
in vec3 position;
in vec3 color;
uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;
out vec3 Color;

void main(){
Color = color;
gl_Position = proj * view * model * vec4(position, 1.0);
}
)glsl";



const GLchar* fragmentSource = R"glsl(
#version 150 core
in vec3 Color;
out vec4 outColor;

void main()
{
outColor = vec4(Color, 1.0);
}
)glsl";

void setCamera(glm::mat4 view, sf::Time time, sf::Window window) {
  
}


int main() {

  sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	// Okno renderingu

	sf::Window window(sf::VideoMode(800, 800, 32), "OpenGL", sf::Style::Titlebar | sf::Style::Close, settings);

	// Inicjalizacja GLEW
	glewExperimental = GL_TRUE;
	glewInit();


  window.setMouseCursorGrabbed(true);
  window.setMouseCursorVisible(false);

  window.setFramerateLimit(60);

	// Utworzenie VAO (Vertex Array Object)

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Utworzenie VBO (Vertex Buffer Object)

	// i skopiowanie do niego danych wierzchołkowych

	GLuint vbo;
	glGenBuffers(1, &vbo);

  float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f
  };

  int vertices_size = sizeof(vertices);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices_size, vertices, GL_STATIC_DRAW);

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

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));


  glm::mat4 model = glm::mat4(1.0f);
  model = glm::rotate(model, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

  GLint uniTrans = glGetUniformLocation(shaderProgram, "model");
  glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(model));

  glm::mat4 view;

  glm::vec3 cameraPos = glm::vec3(0.3f, 0.3f, 3.0f);
  glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
  glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

  view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

  GLint uniView = glGetUniformLocation(shaderProgram, "view");
  glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

  glm::mat4 proj = glm::perspective(glm::radians(45.0f), 800.0f / 800.0f, 0.06f, 100.0f);
  GLint uniProj = glGetUniformLocation(shaderProgram, "proj");

  glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

  float cameraSpeed = 0.06f;

  sf::Clock clock;
  sf::Time time;


  // Rozpoczęcie pętli zdarzeń

	bool running = true;

  glEnable(GL_DEPTH_TEST);
  
  while (running) {
    sf::Event windowEvent;
		while (window.pollEvent(windowEvent)) {
			switch (windowEvent.type) {
				case sf::Event::Closed:
          running = false;
          break;
        case sf::Event::MouseMoved:
          std::cout << "mouse moved" << std::endl;
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
              cameraPos.x -= cameraSpeed;
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
              cameraPos.x += cameraSpeed;
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
          }
          break;
      }
    } 
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, vertices_size);
		window.display();
	}

	// Kasowanie programu i czyszczenie buforów

	glDeleteProgram(shaderProgram);
	glDeleteShader(fragmentShader);
	glDeleteShader(vertexShader);
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);

	// Zamknięcie okna renderingu
	window.close();
	return 0;
}
