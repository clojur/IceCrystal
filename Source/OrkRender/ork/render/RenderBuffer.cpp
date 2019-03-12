/*
 * Ork: a small object-oriented OpenGL Rendering Kernel.
 * Website : http://ork.gforge.inria.fr/
 * Copyright (c) 2008-2015 INRIA - LJK (CNRS - Grenoble University)
 * All rights reserved.
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of source code must retain the above copyright notice, 
 * this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice, 
 * this list of conditions and the following disclaimer in the documentation 
 * and/or other materials provided with the distribution.
 * 
 * 3. Neither the name of the copyright holder nor the names of its contributors 
 * may be used to endorse or promote products derived from this software without 
 * specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND 
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, 
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE 
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED 
 * OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */
/*
 * Ork is distributed under the BSD3 Licence. 
 * For any assistance, feedback and remarks, you can check out the 
 * mailing list on the project page : 
 * http://ork.gforge.inria.fr/
 */
/*
 * Main authors: Eric Bruneton, Antoine Begault, Guillaume Piolat.
 */

#include "ork/render/RenderBuffer.h"

#include <cassert>

#include <GL/glew.h>

#include "ork/render/FrameBuffer.h"

namespace ork
{

RenderBuffer::RenderBuffer(RenderBufferFormat type, int width, int height, int samples) : Object("RenderBuffer")
{
    int format = 0;
    switch (type) {
    case R8:
        format = GL_R8;
        break;
    case R8_SNORM:
        format = GL_R8_SNORM;
        break;
    case R16:
        format = GL_R16;
        break;
    case R16_SNORM:
        format = GL_R16_SNORM;
        break;
    case RG8:
        format = GL_RG8;
        break;
    case RG8_SNORM:
        format = GL_RG8_SNORM;
        break;
    case RG16:
        format = GL_RG16;
        break;
    case RG16_SNORM:
        format = GL_RG16_SNORM;
        break;
    case R3_G3_B2:
        format = GL_R3_G3_B2;
        break;
    case RGB4:
        format = GL_RGB4;
        break;
    case RGB5:
        format = GL_RGB5;
        break;
    case RGB8:
        format = GL_RGB8;
        break;
    case RGB8_SNORM:
        format = GL_RGB8_SNORM;
        break;
    case RGB10:
        format = GL_RGB10;
        break;
    case RGB12:
        format = GL_RGB12;
        break;
    case RGB16:
        format = GL_RGB16;
        break;
    case RGB16_SNORM:
        format = GL_RGB16_SNORM;
        break;
    case RGBA2:
        format = GL_RGBA2;
        break;
    case RGBA4:
        format = GL_RGBA4;
        break;
    case RGB5_A1:
        format = GL_RGB5_A1;
        break;
    case RGBA8:
        format = GL_RGBA8;
        break;
    case RGBA8_SNORM:
        format = GL_RGBA8_SNORM;
        break;
    case RGB10_A2:
        format = GL_RGB10_A2;
        break;
    case RGB10_A2UI:
        format = GL_RGB10_A2UI;
        break;
    case RGBA12:
        format = GL_RGBA12;
        break;
    case RGBA16:
        format = GL_RGBA16;
        break;
    case RGBA16_SNORM:
        format = GL_RGBA16_SNORM;
        break;
    case SRGB8:
        format = GL_SRGB8;
        break;
    case SRGB8_ALPHA8:
        format = GL_SRGB8_ALPHA8;
        break;
    case R16F:
        format = GL_R16F;
        break;
    case RG16F:
        format = GL_RG16F;
        break;
    case RGB16F:
        format = GL_RGB16F;
        break;
    case RGBA16F:
        format = GL_RGBA16F;
        break;
    case R32F:
        format = GL_R32F;
        break;
    case RG32F:
        format = GL_RG32F;
        break;
    case RGB32F:
        format = GL_RGB32F;
        break;
    case RGBA32F:
        format = GL_RGBA32F;
        break;
    case R11F_G11F_B10F:
        format = GL_R11F_G11F_B10F;
        break;
    case RGB9_E5:
        format = GL_RGB9_E5;
        break;
    case R8I:
        format = GL_R8I;
        break;
    case R8UI:
        format = GL_R8UI;
        break;
    case R16I:
        format = GL_R16I;
        break;
    case R16UI:
        format = GL_R16UI;
        break;
    case R32I:
        format = GL_R32I;
        break;
    case R32UI:
        format = GL_R32UI;
        break;
    case RG8I:
        format = GL_RG8I;
        break;
    case RG8UI:
        format = GL_RG8UI;
        break;
    case RG16I:
        format = GL_RG16I;
        break;
    case RG16UI:
        format = GL_RG16UI;
        break;
    case RG32I:
        format = GL_RG32I;
        break;
    case RG32UI:
        format = GL_RG32UI;
        break;
    case RGB8I:
        format = GL_RGB8I;
        break;
    case RGB8UI:
        format = GL_RGB8UI;
        break;
    case RGB16I:
        format = GL_RGB16I;
        break;
    case RGB16UI:
        format = GL_RGB16UI;
        break;
    case RGB32I:
        format = GL_RGB32I;
        break;
    case RGB32UI:
        format = GL_RGB32UI;
        break;
    case RGBA8I:
        format = GL_RGBA8I;
        break;
    case RGBA8UI:
        format = GL_RGBA8UI;
        break;
    case RGBA16I:
        format = GL_RGBA16I;
        break;
    case RGBA16UI:
        format = GL_RGBA16UI;
        break;
    case RGBA32I:
        format = GL_RGBA32I;
        break;
    case RGBA32UI:
        format = GL_RGBA32UI;
        break;
    case DEPTH_COMPONENT:
        format = GL_DEPTH_COMPONENT;
        break;
    case DEPTH_COMPONENT16:
        format = GL_DEPTH_COMPONENT16;
        break;
    case DEPTH_COMPONENT24:
        format = GL_DEPTH_COMPONENT24;
        break;
    case DEPTH_COMPONENT32:
        format = GL_DEPTH_COMPONENT32;
        break;
    case DEPTH_COMPONENT32F:
        format = GL_DEPTH_COMPONENT32F;
        break;
    case DEPTH24_STENCIL8:
        format = GL_DEPTH24_STENCIL8;
        break;
    case DEPTH32F_STENCIL8:
        format = GL_DEPTH32F_STENCIL8;
        break;
    case STENCIL_INDEX:
        format = GL_STENCIL_INDEX;
        break;
    case STENCIL_INDEX1:
        format = GL_STENCIL_INDEX1;
        break;
    case STENCIL_INDEX4:
        format = GL_STENCIL_INDEX4;
        break;
    case STENCIL_INDEX8:
        format = GL_STENCIL_INDEX8;
        break;
    case STENCIL_INDEX16:
        format = GL_STENCIL_INDEX16;
        break;
    default:
        assert(false); // unsupported format
        break;
    }
    glGenRenderbuffers(1, &bufferId);
    glBindRenderbuffer(GL_RENDERBUFFER, bufferId);
    if (samples == 0) {
        glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
    } else {
        glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, format, width, height);
    }
    assert(FrameBuffer::getError() == GL_NO_ERROR);
}

RenderBuffer::~RenderBuffer()
{
    glDeleteRenderbuffers(1, &bufferId);
    assert(FrameBuffer::getError() == GL_NO_ERROR);
}

GLuint RenderBuffer::getId() const
{
    return bufferId;
}

}
