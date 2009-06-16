#ifndef JVGS_SKETCH_RECTANGLERENDERER_H
#define JVGS_SKETCH_RECTANGLERENDERER_H

#include "PrimitiveRenderer.h"

namespace jvgs
{
    namespace sketch
    {
        class Rectangle;

        class RectangleRenderer: public PrimitiveRenderer
        {
            private:
                Rectangle *rectangle;

            public:
                RectangleRenderer(Rectangle *rectangle);
                virtual ~RectangleRenderer();

                virtual void fill(video::Renderer *renderer);
                virtual void stroke(video::Renderer *renderer);
                virtual void vectors(video::Renderer *renderer) const;
        };
    }
}

#endif
