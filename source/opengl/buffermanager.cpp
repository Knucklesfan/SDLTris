#include "buffermanager.h"
#include <iostream>

buffermanager::buffermanager(int w, int h,bool alpha):width(w),height(h) {
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer); //handles running the game at the set resolution

	glGenTextures(1, &renderTexture.id); //generates a texture to render to
	renderTexture.w = w;
	renderTexture.h = h;
	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, renderTexture.id);

	// Give an empty image to OpenGL ( the last "0" )
	if(alpha) { //cant believe we have to do this, but some shaders dont like to play fair...
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGBA, width, height, 0,GL_RGBA, GL_UNSIGNED_BYTE, 0);
	}
	else {
		glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, width, height, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
	}
	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    GLuint depthrenderbuffer;
    glGenRenderbuffers(1, &depthrenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);


	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderTexture.id, 0);
	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

	// Always check that our framebuffer is ok
	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        throw("buffer error");
    }

	//begin renderplane
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//end renderplane

}
void buffermanager::enable() {
		glGetIntegerv(GL_FRAMEBUFFER_BINDING, &previousFB); //store previous framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glViewport(0,0,width,height); // Activate and render at texture size.
		glColorMask(1, 1, 1, 1);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClearDepth(1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void buffermanager::disable(int WINDOW_WIDTH, int WINDOW_HEIGHT, bool restore) {
		if(restore) {
			glBindFramebuffer(GL_FRAMEBUFFER, previousFB); //restore previous framebuffer
		}
		else {
			glBindFramebuffer(GL_FRAMEBUFFER, 0); //restore previous framebuffer
		}
		glViewport(0,0,WINDOW_WIDTH,WINDOW_HEIGHT); //Restore old frame buffer, we're done here.
		// glMatrixMode(GL_PROJECTION);
}
void buffermanager::render(shader* shad,int WINDOW_WIDTH, int WINDOW_HEIGHT, bool aspect) {
    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, renderTexture.id);
    shad->activate();
	if(aspect) {
		glm::mat4 bgmtrans = glm::mat4(1.0f); //the actual transform of the model itself
		float currentaspect = (float)WINDOW_WIDTH/(float)WINDOW_HEIGHT;
		float intendedaspect = (float)width/(float)height;
		float outputwidth = WINDOW_WIDTH;
		float outputheight = WINDOW_HEIGHT;
		if(intendedaspect > currentaspect) {
			outputheight = outputwidth / intendedaspect;
		}
		else {
			outputwidth = outputheight * intendedaspect;
		}
		bgmtrans = glm::scale(bgmtrans, glm::vec3(outputwidth/WINDOW_WIDTH,outputheight/WINDOW_HEIGHT,1.0f));
		shad->setVector("model",glm::value_ptr(bgmtrans));
	}
	else {
		glm::mat4 modelmatrix = glm::mat4(1.0f);
		shad->setVector("model",glm::value_ptr(modelmatrix));
	}

    shad->setInt("texture1",0);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}