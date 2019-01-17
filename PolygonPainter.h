#ifndef POLYGONPAINTER_H
#define POLYGONPAINTER_H

#include <QQuickPaintedItem>
#include <vector>
#include <array>
#include <QLine>

class PolygonPainter : public QQuickPaintedItem
{
    Q_OBJECT
public:
    PolygonPainter();

    void paint(QPainter *painter) override;

public slots:
    void addPoint(qreal x, qreal y);

    void inPolygonTest(qreal x, qreal y);

private:
    std::vector<QPointF> points;
};

#endif // POLYGONPAINTER_H
