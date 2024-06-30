#ifndef BUTTONSIGNALSHANDLING_H
#define BUTTONSIGNALSHANDLING_H

#include <QObject>
#include "convertimage.h"

class ButtonSignalsHandling: public QObject{
    Q_OBJECT
    Q_PROPERTY(bool mode READ getMode WRITE setMode NOTIFY modeChanged)

    public:
        explicit ButtonSignalsHandling(QObject *parent = nullptr);
        bool getMode() const;
        Q_INVOKABLE void setMode(bool newMode);
        QString callConversion(QString path);

    signals:
        void modeChanged();

    private:
        bool mode; // true - img, false - txt
};

#endif // BUTTONSIGNALSHANDLING_H
