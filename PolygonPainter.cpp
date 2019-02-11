#include "PolygonPainter.h"
#include <QPen>
#include <QPainter>
#include <iostream>
#include <glm/common.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/mat2x2.hpp>
#include <stack>
#include <polypartition.h>
#include <thread>
using glm::vec2;

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
        return  !( ij.x < 1e-5  || ij.x > 1 || ij.y > 1 || ij.y < 1e-5 );
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

PolygonPainter::PolygonPainter()
//    : polygon{
//    { 142.801 , 73.8398},
//    { 434.074 , 75.6797},
//    { 425.086 , 353.238},
//    { 200.023 , 399.766},
//    { 24.3633 , 393.906},
//    { 86.6914 , 250.723},
//    { 237.379 , 267.215},
//    { 379.113 , 114.508},
//    { 159.141 , 209.871},
//    { 32.5547 , 188.449},
//    { 54.5352 , 54.418} }
{
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

    pen.setWidth(3.4);
    pen.setColor(QColor(255, 0, 0, 80));
    painter->setPen(pen);
    painter->setBrush(QBrush(QColor(255, 124, 0, 0)));
    painter->setRenderHint(QPainter::Antialiasing);

    std::array<QPointF,3> _tri;
    for(auto tri : triangles) {
        for(int i = 0; i < 3; i++)
            _tri[i] = QPointF(polygon[tri[i]].x, polygon[tri[i]].y);
        painter->drawConvexPolygon(&_tri[0], 3);
    }

    painter->setBrush(QBrush(QColor(255, 124, 0, 255)));
    painter->drawConvexPolygon(&pickedTri[0], 3);
}

void PolygonPainter::addPoint(qreal x, qreal y){
    // 测试新加入的点是否导致自相交
    bool is_interctions = false;
    if(polygon.size() ) {
        glm::vec2 pc = *std::prev(polygon.end()), pd(x, y);
        for(int j = 1; j < polygon.size()-1; j++ )
            is_interctions += LineInterSectionsTest(polygon[j-1], polygon[j], pc, pd);
        pc = *polygon.begin();
        for(int j = 2; j < polygon.size(); j++ )
            is_interctions += LineInterSectionsTest(polygon[j-1], polygon[j], pc, pd);
    }

    if(is_interctions) return ;
    polygon.emplace_back(glm::vec2(x,y));
    update();
}

void PolygonPainter::inPolygonTest(qreal x, qreal y) {
    glm::vec2 pc(x, y), pd(x-60, y-60);
    std::pair<int, int> odd_even{0, 0};
    int ret = LineInterSectionsLMRTest(polygon[0], *std::prev(polygon.end()), pc, pd);
    odd_even.first += (ret == -1);
    odd_even.second += (ret == 1);
    for(int j = 1; j < polygon.size(); j++) {
        ret = LineInterSectionsLMRTest( polygon[j-1], polygon[j], pc, pd);
        odd_even.first += (ret == -1);
        odd_even.second += (ret == 1);
    }
    if(odd_even.first%2 == 0 && odd_even.second%2 == 0)
        std::cout << "outside: " << std::endl;
    else
        std::cout << " inside: " << std::endl;
    update();
}

void PolygonPainter::triangulation() {
    triangles.clear();
    Triangulate_EC(polygon, triangles);
    std::cout << "triangles: " << triangles.size() << std::endl;
    update();
}

void PolygonPainter::pickTriangle(qreal x, qreal y) {
    glm::vec2 p(x, y);
    for(auto tri: triangles) {
        if( InTriangle(polygon[tri[0]], polygon[tri[1]], polygon[tri[2]], p) ){
            for(int i = 0; i < 3; i++)
                pickedTri[i] = QPointF(polygon[tri[i]].x, polygon[tri[i]].y);
        }
    }
    update();
}
