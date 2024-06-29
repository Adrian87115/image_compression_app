#ifndef BUTTONSIGNALSHANDLING_H
#define BUTTONSIGNALSHANDLING_H

#include <QObject>

class ButtonSignalsHandling: public QObject{
    Q_OBJECT
    Q_PROPERTY(bool mode READ getMode WRITE setMode NOTIFY modeChanged)

    public:
        explicit ButtonSignalsHandling(QObject *parent = nullptr);
        bool getMode() const;
        Q_INVOKABLE void setMode(bool newMode);

    signals:
        void modeChanged();

    private:
        bool m_mode;
};

#endif // BUTTONSIGNALSHANDLING_H
