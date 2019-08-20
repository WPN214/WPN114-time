#include "qml_plugin.hpp"
#include <source/time.hpp>
#include <QQmlEngine>
#include <qqml.h>

//-------------------------------------------------------------------------------------------------
void
qml_plugin::registerTypes(const char *uri)
//-------------------------------------------------------------------------------------------------
{
    Q_UNUSED(uri)

    qmlRegisterType<TimeNode, 1>
    ("WPN114.Time", 1, 1, "Node");

    qmlRegisterType<Loop, 1>
    ("WPN114.Time", 1, 0, "Loop");

    qmlRegisterType<Automation, 1>
    ("WPN114.Time", 1, 0, "Automation");
}
