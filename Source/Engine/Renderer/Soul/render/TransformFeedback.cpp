/*
 * Soul: a small object-oriented OpenGL Rendering Kernel.
 */
/*
 * Main authors: jiangsi,tianyu.
 */

#include "Soul/render/TransformFeedback.h"

#include <cassert>

#include <GL/glew.h>

#include "Soul/render/FrameBuffer.h"

namespace Soul
{

GLenum getMeshMode(MeshMode m);

ptr<FrameBuffer> TRANSFORMFEEDBACK_FRAMEBUFFER;

TransformFeedback::TransformFeedback() : Object("TransformFeedback")
{
    glGenTransformFeedbacks(1, &id);
}

TransformFeedback::TransformFeedback(bool main) : Object("TransformFeedback")
{
    if (main) {
        id = 0;
    } else {
        glGenTransformFeedbacks(1, &id);
    }
}

TransformFeedback::~TransformFeedback()
{
    reset();
    if (id != 0) {
        glDeleteTransformFeedbacks(1, &id);
    }
}

ptr<TransformFeedback> TransformFeedback::getDefault()
{
    if (DEFAULT == NULL) {
        DEFAULT = new TransformFeedback(true);
    }
    return DEFAULT;
}

void TransformFeedback::reset()
{
    GLint n;
    glGetIntegerv(GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS, &n);
    bind(id);
    for (GLuint i = 0; i < GLuint(n); ++i) {
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, i, 0);
    }
    assert(FrameBuffer::getError() == GL_NO_ERROR);
}

void TransformFeedback::setVertexBuffer(int index, ptr<GPUBuffer> b)
{
    bind(id);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, index, b->getId());
    b.cast<Buffer>()->dirty();
}

void TransformFeedback::setVertexBuffer(int index, ptr<GPUBuffer> b, GLuint offset, GLuint size)
{
    bind(id);
    glBindBufferRange(GL_TRANSFORM_FEEDBACK_BUFFER, index, b->getId(), GLintptr(offset), GLsizeiptr(size));
    b.cast<Buffer>()->dirty();
}

void TransformFeedback::begin(ptr<FrameBuffer> fb, ptr<Program> transform, MeshMode m, ptr<TransformFeedback> tfb, bool rasterize)
{
    assert(m == POINTS || m == LINES || m == TRIANGLES);
    TRANSFORMFEEDBACK_FRAMEBUFFER = fb;
    TRANSFORM = transform;
    MODE = m;
    TRANSFORMFEEDBACK_FRAMEBUFFER->set();
    TRANSFORM->set();
    bind(tfb->id);
    glBeginTransformFeedback(getMeshMode(m));
    if (!rasterize) {
        glEnable(GL_RASTERIZER_DISCARD);
    }
}

void TransformFeedback::transform(const MeshBuffers &mesh, GLint first, GLsizei count, GLsizei primCount, GLint base)
{
    TRANSFORMFEEDBACK_FRAMEBUFFER->set();
    TRANSFORM->set();
    TRANSFORMFEEDBACK_FRAMEBUFFER->beginConditionalRender();
    mesh.draw(MODE, first, count, primCount, base);
    TRANSFORMFEEDBACK_FRAMEBUFFER->endConditionalRender();
}

void TransformFeedback::multiTransform(const MeshBuffers &mesh, GLint *firsts, GLsizei *counts, GLsizei primCount, GLint* bases)
{
    TRANSFORMFEEDBACK_FRAMEBUFFER->set();
    TRANSFORM->set();
    TRANSFORMFEEDBACK_FRAMEBUFFER->beginConditionalRender();
    mesh.multiDraw(MODE, firsts, counts, primCount, bases);
    TRANSFORMFEEDBACK_FRAMEBUFFER->endConditionalRender();
}

void TransformFeedback::transformIndirect(const MeshBuffers &mesh, const Buffer &buf)
{
    TRANSFORMFEEDBACK_FRAMEBUFFER->set();
    TRANSFORM->set();
    TRANSFORMFEEDBACK_FRAMEBUFFER->beginConditionalRender();
    mesh.drawIndirect(MODE, buf);
    TRANSFORMFEEDBACK_FRAMEBUFFER->endConditionalRender();
}

void TransformFeedback::transformFeedback(const MeshBuffers &mesh, const TransformFeedback &tfb, int stream)
{
    assert(tfb.id != 0);
    TRANSFORMFEEDBACK_FRAMEBUFFER->set();
    TRANSFORM->set();
    TRANSFORMFEEDBACK_FRAMEBUFFER->beginConditionalRender();
    mesh.drawFeedback(MODE, tfb.id, stream);
    TRANSFORMFEEDBACK_FRAMEBUFFER->endConditionalRender();
}

void TransformFeedback::pause()
{
    glPauseTransformFeedback();
}

void TransformFeedback::resume(ptr<TransformFeedback> tfb)
{
    TRANSFORMFEEDBACK_FRAMEBUFFER->set();
    TRANSFORM->set();
    bind(tfb->id);
    glResumeTransformFeedback();
}

void TransformFeedback::end()
{
    glDisable(GL_RASTERIZER_DISCARD);
    glEndTransformFeedback();
    TRANSFORMFEEDBACK_FRAMEBUFFER = NULL;
    TRANSFORM = NULL;
}

void TransformFeedback::bind(GLuint id)
{
    GLint v;
    glGetIntegerv(GL_MAJOR_VERSION, &v);
    if (v >= 4) {
        glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, id);
    }
}

static_ptr<TransformFeedback> TransformFeedback::DEFAULT;

ptr<Program> TransformFeedback::TRANSFORM = NULL;

MeshMode TransformFeedback::MODE;

}
