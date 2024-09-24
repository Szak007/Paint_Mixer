#include "Paint_Mixer.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{



    QApplication a(argc, argv);
    a.setApplicationName("Paint Mixer");
    a.setOrganizationName("Jacek Chrz¹szczyk");
    a.setApplicationVersion("1.0.0");
    QFile file("styles.qss"); 
    if (file.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(file.readAll());
        a.setStyleSheet(styleSheet);
    }
    QIcon icon("main_menu_icon_16x16.png");
    
    a.setWindowIcon(icon);
    Paint_Mixer w;
    w.setWindowTitle(a.applicationName());
    w.show();
    return a.exec();
}
