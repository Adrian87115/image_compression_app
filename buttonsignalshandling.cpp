#include "buttonsignalshandling.h"

ButtonSignalsHandling::ButtonSignalsHandling(QObject *parent): QObject(parent), mode(true){}

bool ButtonSignalsHandling::getMode() const{
    return mode;
}

void ButtonSignalsHandling::setMode(bool new_mode){
    if(mode == new_mode)
        return;
    mode = new_mode;
    emit modeChanged();
}

QString ButtonSignalsHandling::callConversion(QString path){
    ConvertImage temp;
    QString result = temp.performConversion(mode, path);
    return result;
}
