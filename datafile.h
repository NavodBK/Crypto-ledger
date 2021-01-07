#ifndef DATAFILE_H
#define DATAFILE_H

#include <QObject>

class DataFile : public QObject
{
    Q_OBJECT
public:
    explicit DataFile(QObject *parent = nullptr);
    void dataFileInit();

signals:

};

#endif // DATAFILE_H
