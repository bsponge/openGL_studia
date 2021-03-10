
#include <iostream>
#include <stdio.h>
// Nagłówki
#include <GL/glew.h>
#include <SFML/Window.hpp>

#include <cmath>

// Kody shaderów

const GLchar* vertexSource = R"glsl(
#version 150 core
in vec3 position;
in vec3 color;
out vec3 Color;

void main(){
Color = color;
gl_Position = vec4(position, 1.0);
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

int main() {

	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	// Okno renderingu

	sf::Window window(sf::VideoMode(800, 800, 32), "OpenGL", sf::Style::Titlebar | sf::Style::Close, settings);

	// Inicjalizacja GLEW
	glewExperimental = GL_TRUE;
	glewInit();



	// Utworzenie VAO (Vertex Array Object)

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Utworzenie VBO (Vertex Buffer Object)

	// i skopiowanie do niego danych wierzchołkowych

	GLuint vbo;
	glGenBuffers(1, &vbo);

	/*
	GLfloat vertices[] = {
		0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};
	*/
	int vertices_size = 15;
	GLfloat *vertices = new GLfloat[vertices_size * 6];

	float alfa = 0.0f;
	float d_alfa = 2 * M_PI / vertices_size;

	int r = 1;
	for (int i = 0; i < vertices_size * 6; i += 6) {
		vertices[i] = r * cos(alfa);
		vertices[i + 1] = r * sin(alfa);
		vertices[i + 2] = 0.0f;

		vertices[i + 3] = sin(alfa);
		vertices[i + 4] = cos(alfa);
		vertices[i + 5] = tan(alfa);


		alfa += d_alfa;
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices_size * 6 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

	delete vertices;

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



	// Rozpoczęcie pętli zdarzeń

	bool running = true;

	while (running) {
		sf::Event windowEvent;
		while (window.pollEvent(windowEvent)) {
			switch (windowEvent.type) {
				case sf::Event::Closed:
					running = false;
					break;
			}
		}
		// Nadanie scenie koloru czarnego

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Narysowanie trójkąta na podstawie 3 wierzchołków

		glDrawArrays(GL_TRIANGLE_FAN, 0, vertices_size * 3);
		// Wymiana buforów tylni/przedni
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
