#ifndef FILE_INPUT_H
#define FILE_INPUT_H

#include <QObject>
#include <QUrl>
#include <QQuickItem>
#include <QMimeData>

class FileInput: public QObject{
    Q_OBJECT
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged)

    public:
        explicit FileInput(QObject* parent = nullptr);
        QString getPath();
        void setPath(QString f_path);

    signals:
        void pathChanged(QString f_path);

    private:
        QString file_path;
};

#endif // FILE_INPUT_H
