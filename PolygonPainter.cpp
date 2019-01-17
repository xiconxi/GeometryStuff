#include "PolygonPainter.h"
#include <QPen>
#include <QPainter>
#include <iostream>

PolygonPainter::PolygonPainter() {

}

void PolygonPainter::paint(QPainter *painter) {
    QPen pen(QColor(0, 0, 255, 80));
    pen.setWidth(1);
    painter->setPen(pen);

    QBrush brush(QColor(255, 0, 0, 80));
    painter->setBrush(brush);
    painter->setRenderHint(QPainter::Antialiasing);
    painter->drawConvexPolygon(&points[0], points.size());
    std::cout << " rendering " << points.size() << std::endl;
}

void PolygonPainter::addPoint(qreal x, qreal y){
    // 测试新加入的点是否导致自相交
    points.emplace_back(QPointF(x,y));
    update();
}

void PolygonPainter::inPolygonTest(qreal x, qreal y) {
    std::cout << "in test: " << x << ' ' << y << std::endl;
}
