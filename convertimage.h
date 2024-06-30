#ifndef CONVERTIMAGE_H
#define CONVERTIMAGE_H

#include <QObject>
#include <QWidget>
#include "processedimage.h"

class ConvertImage: public QObject{
    Q_OBJECT

    public:
        explicit ConvertImage(QObject *parent = nullptr);
        QString encodeImage(QString path);
        QString decodeFiles(QString path);
        Q_INVOKABLE QString performConversion(bool mode, QString path);
};

#endif // CONVERTIMAGE_H
