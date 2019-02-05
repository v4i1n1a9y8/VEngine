#include<iostream>
#include"vWindow.h"
#include"vContext.h"
#include"vEvent.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

int main(int argc, char** argv) {
	v::Window window("lol",800,600,SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE);
	v::Context context(window,3,3);

	GLuint VAO;
	glGenVertexArrays(1,&VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	GLfloat vertices[] = {
		-0.5f,-0.5f,0.0f,
		-0.5f,0.5f,0.0f,
		0.5f,-0.5f,0.0f,
	};
	GLuint VBO2;
	glGenBuffers(1, &VBO2);
	GLfloat vertices2[] = {
		-0.5f,0.5f,0.0f,
		0.5f,0.5f,0.0f,
		0.5f,-0.5f,0.0f,
	};
	GLuint EBO;
	glGenBuffers(1, &EBO);
	GLuint indices[] = {
		0,1,2
	};

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, VBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//glBindVertexArray(VAO);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	GLuint vs, fs;
	vs = glCreateShader(GL_VERTEX_SHADER);
	fs = glCreateShader(GL_FRAGMENT_SHADER);
	const char* vsource =
R"(#version 330 core
layout(location=0) in vec3 vert;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
void main() {
	gl_Position = projection*view*model*vec4(vert,1);
}
)";
	const char* fsource =
R"(#version 330 core
out vec4 FragColor;
void main() {
	FragColor = vec4(1,0,0,1);
}
)";
	glShaderSource(vs, 1, &vsource, nullptr);
	glCompileShader(vs);
	glShaderSource(fs, 1, &fsource, nullptr);
	glCompileShader(fs);
	GLuint program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	glm::vec3 camPos = glm::vec3(0, 0, -3);
	glm::vec3 camDir = glm::vec3(0, 0, 1);
	glm::vec3 camUp = glm::vec3(0, 1, 0);
	
	glm::mat4 model = glm::mat4(1.0f);

	v::Event::AddKeyEvent(SDLK_ESCAPE,nullptr,nullptr, 
		[]() {v::Event::running = false; }
	);

	int width = 800, height = 600;
	v::Event::onResize.push_back(
		[&width,&height](int w,int h) {
		width = w;
		height = h;
		}
	);
	float speed = 2.0f / 60.0f;
	v::Event::AddKeyEvent(SDLK_w, [&speed, &camPos, &camDir] {camPos += glm::normalize(camDir)*speed; }, nullptr, nullptr); 
	v::Event::AddKeyEvent(SDLK_s, [&speed, &camPos, &camDir] {camPos +=	glm::normalize(-camDir)*speed; }, nullptr, nullptr);
	v::Event::AddKeyEvent(SDLK_a, [&speed, &camPos, &camDir,&camUp] {camPos +=glm::normalize(-glm::cross(camDir, camUp))*speed; }, nullptr, nullptr);
	v::Event::AddKeyEvent(SDLK_d, [&speed, &camPos, &camDir, &camUp] {camPos += glm::normalize(glm::cross(camDir, camUp))*speed; }, nullptr, nullptr);
	v::Event::AddKeyEvent(SDLK_e, [&speed, &camPos, &camUp] {camPos +=glm::normalize(camUp)*speed; }, nullptr, nullptr);
	v::Event::AddKeyEvent(SDLK_q, [&speed, &camPos, &camUp] {camPos +=glm::normalize(-camUp)*speed; }, nullptr, nullptr);
	
	v::Event::onMouseMove.push_back([&camDir](v::Event::MouseMotionEvent mme) {
		glm::mat4 rot = glm::mat4(1.0f);
		float speed = 5.0f / 60.0f;
		rot = glm::rotate(rot, glm::radians(-mme.rx*speed), glm::vec3(0, 1, 0));
		rot = glm::rotate(rot, glm::radians(-mme.ry*speed), glm::cross(camDir,glm::vec3(0, 1, 0)));
		camDir = glm::vec3(rot*glm::vec4(camDir.x, camDir.y, camDir.z, 1.0f));
	});
	SDL_SetRelativeMouseMode(SDL_TRUE);

	SDL_GL_SetSwapInterval(1);
	v::log((char*)glGetString(GL_VERSION));
	while (v::Event::running) {
		v::Event::Poll();
		v::log((const char*)glewGetErrorString(glGetError()));
		glClearColor(1, 1, 1, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glUseProgram(program);
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)width/ (float)height, 0.1f, 100.0f);
		glm::mat4 view = glm::lookAt(
			camPos,
			camPos + camDir,
			camUp
		);
		glUniformMatrix4fv(glGetUniformLocation(program, "projection"),1,GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(glGetUniformLocation(program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glBindVertexArray(VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ARRAY_BUFFER, VBO2);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		window.Update();
		v::log(std::to_string(width).c_str());
		v::log(std::to_string(height).c_str());
		glViewport(0, 0, width, height);
	}
	return 0;
}