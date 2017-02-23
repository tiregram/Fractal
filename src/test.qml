// Project "Frac"
import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Layouts 1.1


Item {
    id: root
    
    anchors.fill: parent
    
    
    MouseArea {
        
        width:root.width
        height:root.height
        onMouseXChanged: xtx.value=mouseX
        onMouseYChanged: ytx.value=mouseY
    }
    
    Rectangle {
        id: im
        color: "#008888"
        width:root.width
        height:root.height
    }
    Rectangle {
        id:cursor
        width:100
        height:100
        color: "#000099"
        
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