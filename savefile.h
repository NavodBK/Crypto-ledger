#ifndef SAVEFILE_H
#define SAVEFILE_H

#include <QFile>
#include<QDir>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMessageBox>


class SaveFile
{
public:
    SaveFile();
    void saveDataFile(QString,QString,QString,QString,QList<QString> ledgerList);
    void DataFileInit();
    QJsonObject readDataFile();

};

#endif // SAVEFILE_H
