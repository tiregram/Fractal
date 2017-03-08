// Project "Frac"
import QtQuick 2.4
import QtQuick.Controls 2.1
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
        property int start_x;
        property int start_y;

        property double  start_pos_x;
        property double start_pos_y;

        property double zoom ;

        onClicked: {
            indicator.running = true;
            imag.source= "image://fract/" + xtx.value + "/" + ytx.value + "/" + zoom + "/" + position.priv_x +"/" +position.priv_y
        }

        onWheel: {
            if (wheel.angleDelta.y > 0)
            {
                zoom = zoom*1.03+1;
                zoomsb.value = zoom;
            }
            else
            {
                zoom = zoom*0.97;
                zoomsb.value = zoom;
            }

            indicator.running = true;
            imag.source= "image://fract/" + xtx.value + "/" + ytx.value + "/" + zoom + "/" + position.priv_x +"/" +position.priv_y

        }

        onPressed:{
            start_x = mouse.x
            start_y = mouse.y
            start_pos_x = position.priv_x
            start_pos_y = position.priv_y

        }
        onPositionChanged:{
            position.priv_x = + (mouse.x - start_x)/zoom*10 + start_pos_x
            position.priv_y = + (mouse.y - start_y)/zoom*10 + start_pos_y

            indicator.running = true;
            imag.source= "image://fract/" + xtx.value + "/" + ytx.value + "/" + zoom + "/" + position.priv_x +"/" +position.priv_y

        }
        onReleased:{

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

    RowLayout {
        id: layout

        SpinBox {
            id: xtx
            from: 0
            value: 1000
            to: 1000 * 1000
            stepSize: 100

            property int decimals: 3
            property real realValue: value / 1000

            validator: DoubleValidator {
                bottom: Math.min(xtx.from, xtx.to)
                top:  Math.max(xtx.from, xtx.to)
            }

            textFromValue: function(value, locale) {
                return Number(value / 100).toLocaleString(locale, 'f', xtx.decimals)
            }

            valueFromText: function(text, locale) {
                return Number.fromLocaleString(locale, text) * 100
            }
        }

        SpinBox {
            id:ytx
            from: 0
            value: 1000
            to: 1000 * 1000
            stepSize: 100

            property int decimals: 3
            property real realValue: value / 1000

            validator: DoubleValidator {
                bottom: Math.min(ytx.from, ytx.to)
                top:  Math.max(ytx.from, ytx.to)
            }

            textFromValue: function(value, locale) {
                return Number(value / 100).toLocaleString(locale, 'f', ytx.decimals)
            }

            valueFromText: function(text, locale) {
                return Number.fromLocaleString(locale, text) * 100
            }

        }

        SpinBox {
            id:zoomsb
            from: 0
            value: 1000
            to: 1000 * 1000
            stepSize: 10

            property int decimals: 3
            property real realValue: value / 1000

            validator: DoubleValidator {
                bottom: Math.min(zoomsb.from, zoomsb.to)
                top:  Math.max(zoomsb.from, zoomsb.to)
            }

            textFromValue: function(value, locale) {
                return Number(value / 100).toLocaleString(locale, 'f', zoomsb.decimals)
            }

            valueFromText: function(text, locale) {
                return Number.fromLocaleString(locale, text) * 100
            }
        }

        Text{
            id:position

            property real priv_x:0
            property real priv_y:0

            text:"x:"+priv_x+"\t y:"+priv_y

        }

        Switch
        {
            id: mandel_julia

        }

        Label
        {
            id: mandel_julia_label
            text:"julia"

        }

    }
}
