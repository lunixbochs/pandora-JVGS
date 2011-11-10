#include "DisplayListWrapper.h"

PglList *curList = NULL;
GLuint curIdx;
PglList::iterator iter;

std::vector<PglList*> lists;
std::vector<int> freeLists;
std::map<int, GLuint> vbos;

GLuint pglGenLists(int number) {
    int place;

    if (number == 0 && freeLists.size() > 0) {
        place = *freeLists.end();
        freeLists.pop_back();
        return place;
    }

    place = lists.size();
    for (int i = 0; i < number; i++) {
        PglList *l = new PglList;
        lists.push_back(l);
    }
    return place;
}

void pglDeleteList(GLuint list) {
    if (lists[list]->empty()) return;

    for (iter = lists[list]->begin(); iter != lists[list]->end(); iter++) {
        delete &(*iter);
    }
    delete lists[list];
}

void pglNewList(GLuint list) {
    pglDeleteList(list);

    curList = lists[list] = new PglList;
    curIdx = list;
}

void pglWriteList(std::vector<GLfloat> *verts) {
    if (curList == NULL) return;

    std::vector<GLfloat> *copy = new std::vector<GLfloat>;
    copy = verts;

    curList->push_back(*copy);
}

void pglEndList() {
    if (curList == NULL) return;

#ifdef future
    GLuint vbo;
    glGenBuffers(1, &vbo);
#endif

    curList = NULL;
    return;
}

void pglCallList(GLuint list) {
    if (list >= lists.size()) return;
    if (lists[list]->empty()) return;

    glEnableClientState(GL_VERTEX_ARRAY);

    for (iter = lists[list]->begin(); iter != lists[list]->end(); iter++) {
        std::vector<GLfloat> l = *iter;
        glVertexPointer(2, GL_FLOAT, 0, &l[0]);
        glDrawArrays(GL_LINE_STRIP, 0, iter->size()/2);
    }
}

void pglDeleteLists(GLuint list, int number) {
    bool end = false;
    if (list + number == lists.size() - 1) {
        end = true;
    }

    if (list <= curIdx && curIdx < list + number - 1) {
        curList = NULL;
    }

    // empty the lists
    for (GLuint i = list; i < list + number - 1; i++) {
        pglDeleteList(i);

        if (!end)
            freeLists.push_back(i);
    }

    if (end) {
        lists.erase(lists.end()-number, lists.end());
    }
}

