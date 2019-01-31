#include "PolygonPainter.h"
#include <QPen>
#include <QPainter>
#include <iostream>
#include <glm/common.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/mat2x2.hpp>
#include <stack>
#include <polypartition.h>

bool toLeftTest(QPointF pa, QPointF pb, QPointF pc) {
   auto v =  glm::determinant(glm::mat3(glm::vec3(pa.x(), pa.y(), 1.0f),glm::vec3(pb.x(), pb.y(), 1.0f),glm::vec3(pc.x(), pc.y(), 1.0f)));
   return v > 0.0f;
}

bool inTriangleTest(QPointF pa, QPointF pb, QPointF pc, QPointF pd) {
    return toLeftTest(pa, pb, pd) &&  toLeftTest(pb, pc, pd)  &&  toLeftTest(pc, pa, pd) ;
}

/// pa*i + pb*(1-i) = pc*j + pd*(1-j)
/// i*(pa-pb) + j*(pd-pc) = pd - pb ==> (i j)T*(
/// \brief LineInterSectionsTest: 测试两条线段是否相交
/// \return
///
bool LineInterSectionsTest(glm::vec2 pa, glm::vec2 pb, glm::vec2 pc, glm::vec2 pd) {
    using std::abs;
    if( abs(glm::determinant(glm::mat2(pb-pa, pc-pd))) < 1e-5 ) return false;
    else {
        auto ij = (glm::inverse(glm::mat2(pb-pa, pc-pd))*(pc-pa));
        return !( ij.x < 1e-5  || ij.x > 1 || ij.y > 1 || ij.y < 1e-5 );
    }
}

int PolygonPainter::LineInterSectionsLMRTest(glm::vec2 pa, glm::vec2 pb, glm::vec2 pc, glm::vec2 pd) {
    using std::abs;
    if( abs(glm::determinant(glm::mat2(pb-pa, pc-pd))) < 1e-5 ) return 0;
    else {
        auto ij = (glm::inverse(glm::mat2(pb-pa, pc-pd))*(pc-pa));
        if(ij[0]> 1e-7 && ij[0] < 1.0) {
            auto p = pa+(pb-pa)*ij[0];
            return (ij[1] > 0.0 ? 1:-1);
        }
    }
    return 0;
}

PolygonPainter::PolygonPainter(): polygon{
    { 142.801 , 73.8398},
    { 434.074 , 75.6797},
    { 425.086 , 353.238},
    { 200.023 , 399.766},
    { 24.3633 , 393.906},
    { 86.6914 , 250.723},
    { 237.379 , 267.215},
    { 379.113 , 114.508},
    { 159.141 , 209.871},
    { 32.5547 , 188.449},
    { 54.5352 , 54.418} }
{
    Triangulation();
}

void PolygonPainter::paint(QPainter *painter) {
    QPen pen(QColor(0, 0, 255, 80));
    pen.setWidth(1.4);
    painter->setPen(pen);

    painter->setBrush(QBrush(QColor(255, 0, 0, 80)));
    painter->setRenderHint(QPainter::Antialiasing);

    std::vector<QPointF> Qpoly(polygon.size());
    for(int i = 0; i < polygon.size(); i++)
        Qpoly[i] = QPointF(polygon[i][0], polygon[i][1]);

    painter->drawConvexPolygon(&Qpoly[0], polygon.size());

    QBrush brush(QColor(255, 124, 0, 80));
    painter->setBrush(brush);
    painter->setRenderHint(QPainter::Antialiasing);
    for(int i = 0; i < triangles.size(); i++) {
        painter->drawConvexPolygon(&triangles[i][0], 3);
    }
}

void PolygonPainter::addPoint(qreal x, qreal y){
    // 测试新加入的点是否导致自相交
//    bool is_interctions = false;
//    if(polygon.size() ) {
//        glm::vec2 pc(std::prev(polygon.end())->x(), std::prev(polygon.end())->y()), pd(x, y);
//        for(int j = 1; j < polygon.size()-1; j++ )
//            is_interctions += LineInterSectionsTest(glm::vec2(polygon[j-1].x(),polygon[j-1].y()), glm::vec2(polygon[j].x(),polygon[j].y()), pc, pd);
//        pc = glm::vec2(polygon.begin()->x(), polygon.begin()->y());
//        for(int j = 2; j < polygon.size(); j++ )
//            is_interctions += LineInterSectionsTest(glm::vec2(polygon[j-1].x(),polygon[j-1].y()), glm::vec2(polygon[j].x(),polygon[j].y()), pc, pd);
//    }

//    if(is_interctions) return ;
//    polygon.emplace_back(QPointF(x,y));
//    update();
}

void PolygonPainter::inPolygonTest(qreal x, qreal y) {
//    glm::vec2 pc(x, y), pd(x-60, y-60);
//    std::pair<int, int> odd_even{0, 0};
//    int ret = LineInterSectionsLMRTest(glm::vec2(polygon[0].x(),polygon[0].y()), glm::vec2(std::prev(polygon.end())->x(), std::prev(polygon.end())->y()), pc, pd);
//    odd_even.first += (ret == -1);
//    odd_even.second += (ret == 1);
//    for(int j = 1; j < polygon.size(); j++) {
//        ret = LineInterSectionsLMRTest(glm::vec2(polygon[j-1].x(),polygon[j-1].y()), glm::vec2(polygon[j].x(),polygon[j].y()), pc, pd);
//        odd_even.first += (ret == -1);
//        odd_even.second += (ret == 1);
//    }
//    if(odd_even.first%2 == 0 && odd_even.second%2 == 0)
//        std::cout << "outside: " << std::endl;
//    else
//        std::cout << " inside: " << std::endl;
//    update();
}

void PolygonPainter::Triangulation() {
    std::vector<glm::ivec3> _triangles;
    Triangulate_EC(polygon, _triangles);
    for(auto tri : _triangles) {
        std::array<QPointF,3> _tri;
        for(int i = 0; i < 3; i++)
            _tri[i] = QPointF(polygon[tri[i]].x, polygon[tri[i]].y);
        triangles.emplace_back(_tri);
    }
//    std::list<int> list;
//    std::vector<int> next(polygon.size());
//    for(int i = 0; i < polygon.size(); i++) {
//        list.emplace_back(i);
//        next[i] = (i + 1)%polygon.size();
//    }

//    list.sort([this](int e1, int e2){
//        return polygon[e1].x() < polygon[e2].x();
//    });
//    std::stack< int > s;
//    s.push(*list.begin());
//    s.push(*std::next(list.begin()));
//    for(int i = 2; i < polygon.size(); i++) {
//    }
}
