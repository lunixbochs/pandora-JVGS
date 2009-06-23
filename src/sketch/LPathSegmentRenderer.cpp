#include "LPathSegmentRenderer.h"
#include "PathSegment.h"
#include "PathRenderer.h"

#include <iostream>
using namespace std;

#include "../math/Vector2D.h"
using namespace jvgs::math;

using namespace jvgs::video;

namespace jvgs
{
    namespace sketch
    {
        LPathSegmentRenderer::LPathSegmentRenderer(PathRenderer *pathRenderer):
                PathSegmentRenderer(pathRenderer)
        {
        }

        LPathSegmentRenderer::~LPathSegmentRenderer()
        {
        }

        void LPathSegmentRenderer::vectors(Renderer *renderer,
                PathSegment *segment, bool fill)
        {
            PathRenderer *pathRenderer = getPathRenderer();

            renderer->vector(pathRenderer->getCurrentPoint());
            for(int i = 0; i + 1 < segment->getNumberOfArguments(); i += 2) {
                Vector2D vector(segment->getArgument(i),
                                segment->getArgument(i + 1));

                if(segment->isRelativeCommand())
                    vector += pathRenderer->getCurrentPoint();
                
                /* Save the last vector. */
                renderer->vector(vector);
                pathRenderer->setCurrentPoint(vector);
            }
        }
    }
}
