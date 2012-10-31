/*
 * (C) Copyright 2012 Kuban State University (http://kubsu.ru/),
 *     Faculty of Physics and Technology, Physics and Information Systems Chair
 *
 * All rights reserved. This file is part of StatistiQ, a program
 * for getting various statistical information from measurement data.
 *
 * Developed by Ivan Novikov (http://github.com/NIA)
 * under the direction of Leontiy Grigorjan
 *
 */

#ifndef READER_H
#define READER_H
// TODO: rename to io.h or smth like this

#include <QObject>
#include <QFile>
#include <QVector>
#include <QDateTime>
#include <QTextStream>

class Reader : public QObject
{
    Q_OBJECT
public:
    class Parser {
    public:
        Parser(QTextStream & in);

        QList<double> parsedData;
        QList<int> skippedLines;
        QList<QString> skippedStrings;
    };

    // Tries to open the file, retrieve data from it and
    // parse it into the vector of double. If something went
    // wrong, isValid() will return false. Otherwise data can
    // be retrieved via data() method.
    explicit Reader(QObject *parent, QString fileName);
    QList<double> data() { return parsedData; }

    QString errorMessage() { return error; }
    QList<QString> warningMessages() { return warnings; }
    bool isValid() { return error.isEmpty(); }

    QString shortFileName() { return fileName; }
    QDateTime modifiedAt() { return lastModified; }

    QString formatReport();
    QString formatFileInfo();

    static QString formatFileInfo(QString fileName, QDateTime lastModified);
    static QString toShortFileName(QString filePath);

private:
    QFile * file;
    QString fileName;
    QDateTime lastModified;

    QList<double> parsedData;
    QString error;
    QList<QString> warnings;

    // Initializing steps:
    void openFile(QString fileName);
    void readFile();
};

// TODO: merge writing report here
class Writer : public QObject {
    Q_OBJECT
public:
    explicit Writer(QObject * parent, QString fileName);
    void write(QList<double> data);
    void close();
    ~Writer();

    bool isValid() { return error.isEmpty() && file != NULL; }
    QString errorMessage() { return error; }

private:
    QFile * file;
    QString error;
};

#endif // READER_H
