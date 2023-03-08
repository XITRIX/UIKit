//
//  Renderer.cpp
//  SDLTest
//
//  Created by Даниил Виноградов on 01.03.2023.
//

#include <Renderer/Renderer.hpp>
#include <UIRenderer/UIRenderer.hpp>
#include <UIApplication/UIApplication.hpp>

#ifdef SDL_GPU_DISABLE_GLES
#include <glew.h>
#define NANOVG_GL3_IMPLEMENTATION
#elif SDL_GPU_DISABLE_OPENGL
#define NANOVG_GLES2_IMPLEMENTATION

#if __APPLE__
#include <OpenGLES/ES2/glext.h>
#else
#include <GLES2/gl2.h>
#endif
#endif

#include <nanovg_gl.h>
#include <nanovg_gl_utils.h>

#include <Utils/Utils.hpp>

namespace UIKit {

Renderer* Renderer::_shared = nullptr;

Renderer* Renderer::shared() {
    if (!_shared) { _shared = new Renderer(); }
    return _shared;
}

Renderer::Renderer() {

//    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 1);

#ifdef SDL_GPU_DISABLE_GLES
    _vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
#elif SDL_GPU_DISABLE_OPENGL
    _vg = nvgCreateGLES2(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
#endif

    auto path = Utils::resourcePath + "Fonts/switch_font.ttf";
    printf("Path: -> %s\n", path.c_str());
    primaryFont = nvgCreateFont(_vg, "regular", path.c_str());
    nvgFontFaceId(_vg, primaryFont);
}

Renderer::~Renderer() {
#ifdef SDL_GPU_DISABLE_GLES
    nvgDeleteGL3(_vg);
#elif SDL_GPU_DISABLE_OPENGL
    nvgDeleteGLES2(_vg);
#endif
}

// Create a GPU_Image from a NanoVG Framebuffer
void Renderer::draw(std::function<void(NVGcontext*)> draw) {
    auto renderer = GPU_GetActiveTarget();
    bool needHFlip = renderer != UIRenderer::main()->rawPointer;

    auto size = renderer->base_h / renderer->h;
    GPU_FlushBlitBuffer(); // IMPORTANT: run GPU_FlushBlitBuffer before nvgBeginFrame
    nvgBeginFrame(_vg, renderer->w, renderer->h, size); // Do your normal NanoVG stuff
//    nvgScale(_vg, size, size);

    auto currentMatrix = NXTransform3D(GPU_GetCurrentMatrix());
    if (needHFlip) {
        currentMatrix = NXTransform3D::scaleBy(1, -1, 1) * NXTransform3D::translationBy(0, -renderer->h, 0) * currentMatrix;
    }
    auto currentAffineMatrix = NXTransform3DGetAffineTransform(currentMatrix);
    nvgTransform(_vg, currentAffineMatrix.m11, currentAffineMatrix.m12, currentAffineMatrix.m21, currentAffineMatrix.m22, currentAffineMatrix.tX, currentAffineMatrix.tY);

    draw(_vg);

    nvgEndFrame(_vg); // Finish our NanoVG pass
    GPU_ResetRendererState();
//    GPU_SetCoordinateMode(false);
}

std::shared_ptr<CGImage> Renderer::drawFBO(Size size, float scale, std::function<void(NVGcontext*)> draw) {
    auto fbo = std::make_shared<CGImage>(size * scale);
    GPU_LoadTarget(fbo->pointee);
    GPU_SetVirtualResolution(fbo->pointee->target, size.width, size.height);

    auto currentTarget = GPU_GetActiveTarget();
    GPU_SetActiveTarget(fbo->pointee->target);

    glViewport(0, 0, size.width * scale, size.height * scale);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

    nvgBeginFrame(_vg, size.width * scale, size.height * scale, scale);
    nvgScale(_vg, scale, scale);

    auto matrix = NXAffineTransform::scaleBy(1, -1) * NXAffineTransform::translationBy(0, -size.height);
    nvgTransform(_vg, matrix.m11, matrix.m12, matrix.m21, matrix.m22, matrix.tX, matrix.tY);

    draw(_vg); // call the drawing function that was passed as parameter
    nvgEndFrame(_vg);
    /* nvgluBindFramebuffer(0); // official documentation says to unbind, but I haven't had issues not doing it */
    GPU_ResetRendererState();

    GPU_SetActiveTarget(currentTarget);
    return fbo;


//    NVGLUframebuffer* fb = nvgluCreateFramebuffer(_vg, size.width, size.height, NVG_IMAGE_NODELETE); // IMPORTANT: NVG_IMAGE_NODELETE allows us to run nvgluDeleteFramebuffer without freeing the GPU_Image data
//    nvgluBindFramebuffer(fb);
//    glViewport(0, 0, size.width, size.height);
//    glClearColor(0, 0, 0, 0);
//    glClear(GL_COLOR_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
//    nvgBeginFrame(_vg, size.width, size.height, scale);
//    draw(_vg); // call the drawing function that was passed as parameter
//    nvgEndFrame(_vg);
//    /* nvgluBindFramebuffer(0); // official documentation says to unbind, but I haven't had issues not doing it */
//    GPU_ResetRendererState(); // not calling GPU_ResetRendererState can cause problems with SDL_gpu depending on your order of operations
//    // IMPORTANT: don't run nvgluDeleteFramebuffer, GPU_CreateImageUsingTexture takes the handle
//    GPU_Image* return_image = GPU_CreateImageUsingTexture(fb->texture, false); // should take_ownership be true?
//    if (!return_image) {
//        printf("%s\n", GPU_PopErrorCode().details);
//    }
//    nvgluDeleteFramebuffer(fb);
//
//    GPU_Image a;
//
//    return std::make_shared<CGImage>(return_image);
}

}
