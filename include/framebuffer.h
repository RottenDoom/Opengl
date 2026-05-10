/*
 * File: framebuffer.h
 * Created: 2026-05-10
 * Description: Framebuffer class implementation for post-processing effects
* 
* MIT License
* 
* Copyright (c) 2026 Aditya Yadav
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE
*/

#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <glad/glad.h>
#include <assert.h>

#include "logger.h"

class Framebuffer
{
private:
        GLuint m_fbo{0};
        GLuint m_rbo{0};
        GLuint m_texture{0};

        uint32_t m_width{0};
        uint32_t m_height{0};

        GLint    m_colorInternalFormat{GL_RGB};
        GLenum   m_colorFormat{GL_RGB};
        GLenum   m_colorType{GL_UNSIGNED_BYTE};
public:

        void Init(const uint32_t width, const uint32_t height, 
                GLint colorInternalFormat = GL_RGB,
                GLenum colorFormat        = GL_RGB,
                GLenum colorType          = GL_UNSIGNED_BYTE) 
        {
                assert(width != 0 && height != 0);
                m_width = width;
                m_height = height;
                m_colorInternalFormat = colorInternalFormat;
                m_colorFormat         = colorFormat;
                m_colorType           = colorType;

                // generate framebuffer
                glGenFramebuffers(1, &m_fbo);
                glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

                // generate texture
                glGenTextures(1, &m_texture);
                glBindTexture(GL_TEXTURE_2D, m_texture);
                glTexImage2D(GL_TEXTURE_2D, 0, colorInternalFormat, width, height, 0, colorFormat, colorType, NULL);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glBindTexture(GL_TEXTURE_2D, 0);

                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texture, 0);

                if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                        Logger::Warn("Framebuffer::Init:  Framebuffer is not complete!");
                
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void Bind() 
        {
                glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        }

        void Unbind() 
        {
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        void FramebufferResize(const uint32_t width, const uint32_t height) 
        {
                glViewport(0, 0, width, height);
        }

        void AttachDepthStencil()
        {
                assert(m_fbo != 0 && m_width != 0 && m_height != 0);

                glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

                glGenRenderbuffers(1, &m_rbo);
                glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
                                m_width, m_height);
                glBindRenderbuffer(GL_RENDERBUFFER, 0);

                glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                                        GL_RENDERBUFFER, m_rbo);

                if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                Logger::Warn("Framebuffer::AttachDepthStencil: not complete!");

                glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }


        void Resize(uint32_t newWidth, uint32_t newHeight)
        {
                assert(m_fbo != 0);
                assert(newWidth != 0 && newHeight != 0);

                m_width  = newWidth;
                m_height = newHeight;

                // reallocate color texture storage
                glBindTexture(GL_TEXTURE_2D, m_texture);
                glTexImage2D(GL_TEXTURE_2D, 0, m_colorInternalFormat,
                        m_width, m_height, 0,
                        m_colorFormat, m_colorType, nullptr);
                glBindTexture(GL_TEXTURE_2D, 0);

                // reallocate renderbuffer storage if it exists
                if (m_rbo)
                {
                        glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
                        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8,
                                                m_width, m_height);
                        glBindRenderbuffer(GL_RENDERBUFFER, 0);
                }

                glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
                if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
                        Logger::Warn("Framebuffer::Resize: not complete after resize!");
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        GLuint   GetTexture() const { return m_texture; }
        uint32_t GetWidth()   const { return m_width;   }
        uint32_t GetHeight()  const { return m_height;  }

        void Destroy()
        {
                if (m_rbo)     { glDeleteRenderbuffers(1, &m_rbo);  m_rbo     = 0; }
                if (m_texture) { glDeleteTextures(1, &m_texture);   m_texture = 0; }
                if (m_fbo)     { glDeleteFramebuffers(1, &m_fbo);   m_fbo     = 0; }
        }
};


#endif // FRAMEBUFFER_H