#include "getfilepath.h"

GetFilePath::GetFilePath(QObject *parent){
}

QString GetFilePath::getPath() const{
    return stored_path;
}

void GetFilePath::setPath(QString new_path){
    if(new_path != stored_path){
        stored_path = new_path;
    }
    qDebug() << "Registered path: " << new_path;
    emit pathChanged();
}
