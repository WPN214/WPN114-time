import QtQuick 2.12
import QtQuick.Window 2.12
import WPN114.Time 1.1 as Time

Window
{
    visible: true
    width: 640
    height: 480
    title: qsTr("simple-example")

    Time.Node
    {
        id: startpoint
        wait: sec(10)
        duration: sec(10)
        onStart: console.log("node 1 running")
        onEnd: console.log("node 1 ended")

        Time.Node
        {
            // this will trigger whenever startpoint ends
            wait: sec(5)
            duration: Time.Node.Infinite
            onStart: console.log("node 2 running")
        }
    }
}
