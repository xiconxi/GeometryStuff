import QtQuick 2.10
import QtQuick.Window 2.10
import GeometryLabs 1.0

Window {
    visible: true
    width: 900
    height: 480
    title: qsTr("Hello World")
    PolygonPainter{
        anchors.fill: parent
        MouseArea{
            anchors.fill: parent
            property bool onTest: false
            onClicked: {
                if(!onTest)
                    parent.addPoint(mouseX, mouseY)
                else {
                    parent.pickTriangle(mouseX, mouseY);
                }
            }
            onDoubleClicked: {
                onTest = true;
                parent.triangulation();
            }
        }
    }
}
