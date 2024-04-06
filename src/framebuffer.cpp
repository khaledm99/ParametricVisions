#include "framebuffer.h"
#include <iostream>

Framebuffer::Framebuffer()
{}

void Framebuffer::setup(int width, int height)
{
    glGenFramebuffers(1, &fb);
    glBindFramebuffer(GL_FRAMEBUFFER, fb);
    glViewport(0,0,width,height);

    glGenTextures(1, &texColBuf);
    glBindTexture(GL_TEXTURE_2D, texColBuf);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0); // unbind texture after setting up

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColBuf, 0);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER, 0); // unbind renderbuffer after setting up
    
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cout<< "ERROR: Framebuffer is not complete" << std::endl;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);   // unbind our framebuffer after setting up

}
void Framebuffer::bind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, fb);
}

void Framebuffer::unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

unsigned int const Framebuffer::getFramebuffer()
{
    return fb;
}
unsigned int const Framebuffer::getTextureBuffer()
{
    return texColBuf;
}
unsigned int const Framebuffer::getRenderbuffer()
{
    return rbo;
}
