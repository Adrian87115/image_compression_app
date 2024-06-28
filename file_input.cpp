#include "file_input.h"

FileInput::FileInput(QObject *parent): QObject(parent){
}

QString FileInput::getPath(){
    return file_path;
}

void FileInput::setPath(QString f_path){
    if(file_path != f_path){
        file_path = f_path;
        emit pathChanged(file_path);
    }
}
