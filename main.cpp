#include <QApplication>
#include <QStyleFactory>
#include <QFile>
#include "gui.h"

int main(int argc, char *argv[])
{
    QApplication::setStyle(QStyleFactory::create(QStringLiteral("Fusion")));
    QApplication a(argc, argv);

    QFile stylesheet(":/style.qss");
    stylesheet.open(QFile::ReadOnly);
    QString setSheet = QLatin1String(stylesheet.readAll());
    a.setStyleSheet(setSheet);

    GUI w;
    w.show();

    return a.exec();
}
