#include "polypartition.h"
#include <glm/geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

struct PartitionVertex {
    PartitionVertex(){}
    PartitionVertex(std::uint32_t _id, std::uint32_t __prev, std::uint32_t __next, std::vector<glm::vec2>* ref, PartitionVertex* self_ref):
        _prev(__prev), _next(__next), vidx(_id), _vertex(ref), _offset(self_ref) {}
    bool isActive{true};
    bool isConvex{false};
    bool isEar{false};
    float angle;

    std::uint32_t vidx;

    glm::vec2& vertex() { return _vertex->at(vidx); }

    PartitionVertex* next(PartitionVertex* p = nullptr){
        if(p) _next = p - _offset;
        return _offset + _next;
    }

    PartitionVertex* prev(PartitionVertex* p = nullptr){
        if(p) _next = p - _offset;
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

void UpdateVertex(PartitionVertex *v, PartitionVertex *vertices, long length) {
    v->isConvex = Area(v->prev()->vertex(), v->vertex(), v->next()->vertex()) > 0.0f;
    v->angle    = glm::dot(v->prev()->vertex() - v->vertex(), v->next()->vertex() - v->vertex());

    if(v->isConvex) {
        v->isEar = true;
        for(int i = 0; i < length; i++) {
            if(v->vidx == i || v->next()->vidx == i || v->prev()->vidx == i ) continue;
            if(InTriangle(v->prev()->vertex(), v->vertex(), v->next()->vertex(), vertices->vertex())) {
                v->isEar = false;
                break;
            }
        }
    } else {
        v->isEar = false;
    }
}

int Triangulate_EC(std::vector<glm::vec2> &poly, std::vector<glm::ivec3>  &triangles) {
    if(poly.size() < 3) return 0;

    //initialize circular linked list
    PartitionVertex vertexs[poly.size()];
    for(int i = 0; i < poly.size(); i++)
        vertexs[i] = PartitionVertex(i, i == 0 ? poly.size()-1: i-1, i == poly.size()-1 ? 0: i+1, &poly, vertexs);

    for(int i = 0; i < poly.size(); i++)
        UpdateVertex(vertexs + i, vertexs, poly.size());

    for(int i = 0; i < poly.size() - 3; i++) {
        PartitionVertex* ear = nullptr;
        for( int j = 0; j < poly.size(); j++) {
            if( !vertexs[j].isActive || !vertexs[j].isEar) continue;
            if( !ear ) {
                ear = vertexs + j;
            }else
                ear = vertexs[j].angle > ear->angle ? &vertexs[j]: ear;

        }
        if (!ear) return 0;

        triangles.emplace_back(glm::ivec3(ear->prev()->vidx, ear->vidx, ear->next()->vidx));

        ear->isActive = false;
        ear->prev()->next(ear->next());
        ear->next()->prev(ear->prev());

        if( i == poly.size() - 4) break;

        UpdateVertex(ear->prev(), vertexs, poly.size());
        UpdateVertex(ear->next(), vertexs, poly.size());
    }

    for(int i = 0; i < poly.size(); i++) {
        if(!vertexs[i].isActive) continue;
        triangles.emplace_back(glm::ivec3(vertexs[i].prev()->vidx, i, vertexs[i].next()->vidx));
        break;
    }

    return 1;

}