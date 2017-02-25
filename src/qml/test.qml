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
        onClicked: {
                   img.source= "image://fract/yellow"

        }
    }
        

    Image {
        id:img
        anchors.centerIn: parent ;
        sourceSize.width: root.width    
        sourceSize.height: root.height

        onStatusChanged: {
            if(status == Image.Ready)
                indicator.running = false;
        }

        BusyIndicator {
            id: indicator
            anchors.centerIn: parent
            running: false
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                indicator.running = true;
                img.source = "image://fract/500/500/?seed=" + Math.random(1000)
            }
        }
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