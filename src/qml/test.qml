// Project "Frac"
import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2


Item {
    id: root
    
    anchors.fill: parent
    
    MouseArea {
        
        width:root.width
        height:root.height
        onMouseXChanged: xtx.value=mouseX
        onMouseYChanged: ytx.value=mouseY
        property int priv_x;
        property int priv_y;
        property int zoom ;

        onClicked: {
            indicator.running = true;
            imag.source= "image://fract/" + 0 + "/" + 0.69 + "/" + zoom + "/" + priv_x +    "/" + priv_y+Math.random(100);
        }

        onWheel:
        {
            zoom = zoom*1.1+1;
        }

    }

    Image {
        id:imag
        anchors.centerIn: parent ;
        sourceSize.width: root.width    
        sourceSize.height: root.height

        onStatusChanged: {
            if(status == Image.Ready)
                indicator.running = false;
        }

    }
    BusyIndicator {
        id: indicator
        anchors.centerIn: parent
        running: false
    }


    ColumnLayout {
        
        anchors.margins: spacing
        RowLayout {
            
            SpinBox {
                id: xtx
                maximumValue: 10000
                
            }
            
            SpinBox {
                id:ytx
                maximumValue: 10000
            }
        }
    }
}
