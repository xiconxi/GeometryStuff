#include "polypartition.h"
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

struct PartitionVertex {
    PartitionVertex(){}
    PartitionVertex(std::uint32_t _id, std::uint32_t __prev, std::uint32_t __next, std::vector<glm::vec2>* ref, PartitionVertex* self_ref):
        _prev(__prev), _next(__next), vidx(_id), _vertex(ref), _offset(self_ref) {
    }

    bool isConvex{true};
    bool isEar{false};

    std::uint32_t vidx;

    glm::vec2& vertex() { return _vertex->at(vidx); }

    PartitionVertex* next(PartitionVertex* p = nullptr){
        if(p) _next = p - _offset;
        return _offset + _next;
    }

    PartitionVertex* prev(PartitionVertex* p = nullptr){
        if(p) _prev = p - _offset;
        return _offset + _prev;
    }

private:
    std::uint32_t _prev, _next;
    std::vector<glm::vec2>* _vertex;
    PartitionVertex* _offset;
};

float Area(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3) {
    return glm::determinant(glm::mat3(glm::vec3(p1, 1.0f), glm::vec3(p2, 1.0f),glm::vec3(p3, 1.0f))) ;
}

bool InTriangle(glm::vec2 pa, glm::vec2 pb, glm::vec2 pc, glm::vec2 p) {
    auto s = (Area(pa, pb, p) > 0.0f) + (Area(pb, pc, p) > 0.0f) + (Area(pc, pa, p) > 0.0f);
    return s == 0 || s == 3;
}

void UpdateVertex(PartitionVertex *v, bool convexChanged = false) {
    if(v->isConvex || convexChanged) {
        auto curr = v->next()->next();
        while(curr != v->prev()) {
            if(InTriangle(v->prev()->vertex(), v->vertex(), v->next()->vertex(), curr->vertex())) {
                v->isEar = false;
                curr->isConvex = false;
                return ;
            }
            curr = curr->next();
        };
        v->isEar = Area(v->prev()->vertex(), v->vertex(), v->next()->vertex()) > 0.0f;
    }
}

int Triangulate_EC(std::vector<glm::vec2> &poly, std::vector<glm::ivec3>  &triangles) {
    if(poly.size() < 3) return 0;

    PartitionVertex vertexs[poly.size()];
    //initialize circular linked list
    for(int i = 0; i < poly.size(); i++)
        vertexs[i] = PartitionVertex(i, i == 0 ? poly.size()-1: i-1, i == poly.size()-1 ? 0: i+1, &poly, vertexs);

    for(int i = 0; i < poly.size(); i++)  UpdateVertex(vertexs + i);

    auto curr = vertexs;
    while(curr->next()->next() != curr){
        bool is_ear = false;
        auto _curr = curr;
        do {
            if( curr->isEar ) {
                is_ear = true; break;
            }
            curr = curr->next();
        }while(_curr != curr );

        if ( !is_ear )  return 0;

        triangles.emplace_back(glm::ivec3(curr->prev()->vidx, curr->vidx, curr->next()->vidx));

        // remove ear from link
        curr->prev()->next(curr->next());
        curr->next()->prev(curr->prev());

        UpdateVertex(curr->prev(), true);
        UpdateVertex(curr->next(), true);

        curr = curr->prev();
    };

    return 1;
}