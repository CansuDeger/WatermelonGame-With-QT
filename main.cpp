// CENNET CANSU DEGER
//23100011023
#include "WatermelonGame.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);


    Q_INIT_RESOURCE(resources);

    WatermelonGame w;
    w.show();

    return a.exec();
}
