#include "VideoManager.h"
#include "Renderer.h"

#include <SDL.h>

#ifdef HAVE_GLES
#include <GLES/gl.h>
#include "eglport.h"
#endif

using namespace jvgs::math;

using namespace std;

namespace jvgs
{
    namespace video
    {
        VideoManager::VideoManager()
        {
            SDL_InitSubSystem(SDL_INIT_VIDEO);
#ifndef HAVE_GLES
            flags = SDL_DOUBLEBUF | SDL_HWSURFACE | SDL_HWACCEL | SDL_OPENGL;
#else
            flags = SDL_DOUBLEBUF | SDL_SWSURFACE;

            if (!EGL_Open()) exit(1);
#endif
        }

        VideoManager::~VideoManager()
        {
#ifdef HAVE_GLES
            EGL_Destroy();
#endif
            SDL_QuitSubSystem(SDL_INIT_VIDEO);
        }

        VideoManager *VideoManager::getInstance()
        {
            static VideoManager instance;
            return &instance;
        }

        void VideoManager::setVideoMode(std::string title)
        {
#ifndef HAVE_GLES
            SDL_Rect **modes = SDL_ListModes(NULL, flags | SDL_FULLSCREEN);

            /* Auto-select video mode. */
            size = Vector2D(800, 600);
            if(modes!=NULL) {
                size = Vector2D(modes[0]->w, modes[0]->h);
            }
#else
            size = Vector2D(800, 480);
#endif

            SDL_SetVideoMode(800, 480, 0,
                    flags | SDL_FULLSCREEN);
#ifdef HAVE_GLES
            EGL_Init();
#endif
            SDL_ShowCursor(0);
            SDL_WM_SetCaption(title.c_str(), NULL);

            setVideoDefaults();
        }

        void VideoManager::setVideoMode(const Vector2D &size,
                                        std::string title)
        {
            SDL_WM_SetCaption(title.c_str(), NULL);
            setVideoMode(size);
        }

        void VideoManager::setVideoMode(const Vector2D &size)
        {
            this->size = Vector2D((float)(int) size.getX(),
                    (float)(int) size.getY());

            SDL_SetVideoMode((int) size.getX(), (int) size.getY(), 0, flags);
            SDL_ShowCursor(0);
            
            setVideoDefaults();
        }

        void VideoManager::setVideoDefaults()
        {
            glViewport(0, 0, (GLsizei) size.getX(), (GLsizei) size.getY());

            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glOrtho(0.0f, (GLfloat) size.getX(), (GLfloat) size.getY(),
                    0.0f, -1.0f, 1.0f);

            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();

            glEnable(GL_TEXTURE_2D);
            glBindTexture(GL_TEXTURE_2D, 0);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glEnable(GL_LINE_SMOOTH);
            glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

            setClearColor(Color(1.0f, 1.0f, 1.0f));
            setColor(Color(0.0f, 0.0f, 0.0f));

            glLineWidth(1.5f);
        }

        const Vector2D &VideoManager::getSize() const
        {
            return size;
        }

        void VideoManager::clear() const
        {
            glClear(GL_COLOR_BUFFER_BIT);
        }

        void VideoManager::flip() const
        {
#ifndef HAVE_GLES
            SDL_GL_SwapBuffers();
#else
            EGL_SwapBuffers();
#endif
        }

        void VideoManager::identity() const
        {
            glLoadIdentity();
        }

        void VideoManager::push() const
        {
            glPushMatrix();
        }

        void VideoManager::pop() const
        {
            glPopMatrix();
        }

        void VideoManager::translate(const Vector2D &vector) const
        {
            glTranslatef(vector.getX(), vector.getY(), 0.0f);
        }

        void VideoManager::scale(const Vector2D &scale) const
        {
            glScalef(scale.getX(), scale.getY(), 1.0f);
        }

        void VideoManager::rotate(const float &degrees) const
        {
            glRotatef(degrees, 0.0f, 0.0f, 1.0f);
        }

        void VideoManager::transform(const AffineTransformationMatrix &matrix)
                const
        {
            float *glMatrix = new float[16];

            /* Our AffineTransformationMatrix
             *     / a b c \
             *     | d e f |
             *     \ 0 0 1 /
             * becomes a matrix we can use with OpenGL:
             *     / a b 0 c \
             *     | d e 0 f |
             *     | 0 0 1 0 |
             *     \ 0 0 0 1 /
             */

            /* Intialize to 0. */
            for(int i = 0; i < 16; i++)
                glMatrix[i] = 0.0f;
            
            for(int row = 0; row < matrix.getHeight(); row++) {
                for(int column = 0; column < matrix.getWidth(); column++) {
                    if(column < 2 && row < 2) {
                        glMatrix[column * 4 + row] =
                                matrix.getValue(row, column);
                    }
                }
            }

            glMatrix[2 * 4 + 2] = 1.0f;
            glMatrix[3 * 4 + 3] = 1.0f;

            glMatrix[3 * 4 + 0] = matrix.getValue(0, 2);
            glMatrix[3 * 4 + 1] = matrix.getValue(1, 2);

            glMultMatrixf(glMatrix);
            delete[] glMatrix;
        }

        void VideoManager::setColor(const Color &color)
        {
            this->color = color;
            glColor4f(color.getRed(), color.getGreen(),
                      color.getBlue(), color.getAlpha());
        }

        const Color &VideoManager::getColor() const
        {
            return color;
        }

        void VideoManager::setClearColor(const Color &clearColor)
        {
            this->clearColor = clearColor;
            glClearColor(clearColor.getRed(), clearColor.getGreen(),
                         clearColor.getBlue(), clearColor.getAlpha());
        }

        const Color &VideoManager::getClearColor() const
        {
            return clearColor;
        }

        void VideoManager::invert()
        {
            Color tmp = color;
            setColor(clearColor);
            setClearColor(tmp);
        }
    };
};
