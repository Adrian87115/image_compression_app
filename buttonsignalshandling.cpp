#include "buttonsignalshandling.h"

ButtonSignalsHandling::ButtonSignalsHandling(QObject *parent): QObject(parent), m_mode(true){}

bool ButtonSignalsHandling::getMode() const{
    return m_mode;
}

void ButtonSignalsHandling::setMode(bool new_mode){
    if(m_mode == new_mode)
        return;
    m_mode = new_mode;
    emit modeChanged();
}
