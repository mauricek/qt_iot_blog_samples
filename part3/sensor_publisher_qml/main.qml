import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3
import qt.dds.sensorinformation 1.0

ApplicationWindow {
    visible: true
    width: 640
    height: 480
    title: qsTr("Sensor Publisher - DDS")

    property bool animate: false

    function moveValue(currentValue, stepSize)
    {
        return currentValue + 2 * stepSize * (Math.random() - 0.5)
    }

    SensorInformation {
        id: sensor
        ambientTemperature: ambientSlider.value
        objectTemperature: objectSlider.value
        light: lightSlider.value
        humidity: humiditySlider.value
        accelerometerX: accelXSlider.value
        accelerometerY: accelYSlider.value
        accelerometerZ: accelZSlider.value
    }

    Timer {
        interval: 500
        running: animate
        repeat: true
        onTriggered: {
            ambientSlider.value = moveValue(ambientSlider.value, 0.5)
            lightSlider.value = moveValue(lightSlider.value, 50)
            humiditySlider.value = moveValue(humiditySlider.value, 2)
            accelXSlider.value = moveValue(accelXSlider.value, 3)
            accelYSlider.value = moveValue(accelYSlider.value, 3)
            accelZSlider.value = moveValue(accelZSlider.value, 3)
        }
    }

    GridLayout {
        anchors.fill: parent
        anchors.margins: 20
        columns: 2

        Label { text: qsTr("ID:") }

        RowLayout {
            Label {
                id: idLabel
                text: sensor.identifier
            }
            Button {
                id: animateButton
                text: (animate ? "Stop" : "Start") + " Animation"
                onClicked: animate = !animate
                Layout.fillWidth: true
            }
        }

        Label { text: qsTr("Ambient Temperature:") }

        Slider {
            id: ambientSlider
            stepSize: 0.1
            to: 50
            from: -20
            value: 0.5
            Layout.fillWidth: true
        }

        Label { text: qsTr("Object Temperature:") }

        Slider {
            id: objectSlider
            enabled: false
            value: ambientSlider.value
            to: 50
            stepSize: 0.1
            from: -20
            Layout.fillWidth: true

            Behavior on value { NumberAnimation { duration: 1500 } }
        }

        Label { text: qsTr("Light:") }

        Slider {
            id: lightSlider
            stepSize: 0.5
            to: 1000
            from: 0
            value: 600
            Layout.fillWidth: true
        }

        Label { text: qsTr("Humidity:") }

        Slider {
            id: humiditySlider
            stepSize: 0.1
            to: 100
            from: 0
            value: 70
            Layout.fillWidth: true
        }

        Label { text: qsTr("Acceleration:") }

        ColumnLayout {
            Slider {
                id: accelXSlider
                stepSize: 0.01
                to: 10
                from: -10
                value: 0
                Layout.fillWidth: true
            }
            Slider {
                id: accelYSlider
                stepSize: 0.01
                to: 10
                from: -10
                value: 0
                Layout.fillWidth: true
            }
            Slider {
                id: accelZSlider
                stepSize: 0.01
                to: 10
                from: -10
                value: 0
                Layout.fillWidth: true
            }
            Layout.fillWidth: true
        }
    }
}
