#include "ListManager.h"

namespace jvgs
{
    namespace video
    {
        ListManager::ListManager()
        {
        }

        ListManager::~ListManager()
        {
        }

        ListManager *ListManager::getInstance()
        {
            static ListManager instance;
            return &instance;
        }

        ListManager::List ListManager::createLists(int number) const
        {
#ifndef HAVE_GLES
            List list = glGenLists(number);
            return list;
#else
            return pglGenLists(number);
#endif
        }

        void ListManager::beginList(const List &list) const
        {
#ifndef HAVE_GLES
            glNewList(list, GL_COMPILE);
#else
            pglNewList(list);
#endif
        }

        void ListManager::endList() const
        {
#ifndef HAVE_GLES
            glEndList();
#else
            pglEndList();
#endif
        }

        void ListManager::callList(const List &list) const
        {
#ifndef HAVE_GLES
            glCallList(list);
#else
            pglCallList(list);
#endif
        }

        void ListManager::callLists(const List &base, GLubyte *lists,
                int number) const
        {
#ifndef HAVE_GLES
            glListBase(base);
            glCallLists(number, GL_UNSIGNED_BYTE, lists);
#else
            for (int i = 0; i < number; i++) {
                lists++;
                pglCallList(GLuint(lists)+base);
            }
#endif
        }

        void ListManager::deleteLists(const List &list, int number) const
        {
#ifndef HAVE_GLES
            glDeleteLists(list, number);
#else
            pglDeleteLists(list, number);
#endif
        }
    }
}
