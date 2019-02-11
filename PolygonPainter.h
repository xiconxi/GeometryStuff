#ifndef POLYGONPAINTER_H
#define POLYGONPAINTER_H

#include <QQuickPaintedItem>
#include <vector>
#include <array>
#include <QLine>
#include <glm/common.hpp>

class PolygonPainter : public QQuickPaintedItem
{
    Q_OBJECT
public:
    PolygonPainter();

    void paint(QPainter *painter) override;

public slots:
    void addPoint(qreal x, qreal y);

    void inPolygonTest(qreal x, qreal y);

    void pickTriangle(qreal x, qreal y);

    void triangulation();

private:
    int LineInterSectionsLMRTest(glm::vec2 pa, glm::vec2 pb, glm::vec2 pc, glm::vec2 pd) ;
    std::vector<glm::vec2> polygon;
    std::vector<glm::ivec3> triangles;
    std::array<QPointF,3> pickedTri;
};

#endif // POLYGONPAINTER_H
