#ifndef GETFILEPATH_H
#define GETFILEPATH_H

#include <QObject>
#include <QDebug>

class GetFilePath: public QObject{
    Q_OBJECT
    Q_PROPERTY(QString stored_path READ getPath WRITE setPath NOTIFY pathChanged)

    public:
        explicit GetFilePath(QObject *parent = nullptr);
        QString getPath() const; // apparently this has to be a constant
        Q_INVOKABLE void setPath(QString newPath); // so for some fucking unknown reason it will not work without Q_INVOKABLE, even though in tutorials it is not added

    signals:
        void pathChanged();

    private:
        QString stored_path;
};

#endif // GETFILEPATH_H
