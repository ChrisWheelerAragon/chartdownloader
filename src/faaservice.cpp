/*  FAA Service
    Copyright (C) 2017 Jasem Mutlaq (mutlaqja@ikarustech.com)

    This application is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

*/

#include <QDate>
#include <QUrl>
#include <QXmlSimpleReader>

#include "faaservice.h"
#include "filedownloader.h"
#include "mycontenthandler.h"

FAAService::FAAService(QObject *parent) : ServiceProvider(parent)
{

}

FAAService::~FAAService()
{
    delete (downloadJob);
}

bool FAAService::canDownload(const QString &airportID)
{
    return (airportID.startsWith("K", Qt::CaseInsensitive) ||
            // Alaska
            airportID.startsWith("PA", Qt::CaseInsensitive) ||
            airportID.startsWith("PF", Qt::CaseInsensitive) ||
            airportID.startsWith("PO", Qt::CaseInsensitive) ||
            airportID.startsWith("PP", Qt::CaseInsensitive) ||
            // Hawaii
            airportID.startsWith("PH", Qt::CaseInsensitive));
}

bool FAAService::startDownload(const QString &airportID, bool getAirport, bool getMinimums, bool getSID, bool getSTAR, bool getApproach)
{
    m_airportID  = airportID;

    this->getAirport = getAirport;
    this->getMin     = getMinimums;
    this->getSID     = getSID;
    this->getStar    = getSTAR;
    this->getApproach= getApproach;

    charts.clear();
    currentPage=1;

    downloadPage();

    return true;
}

void FAAService::downloadPage()
{
    // Get current cycle from 1-week in the future
    //QDate currentDate = QDate::currentDate().addDays(7);
    QDate currentDate = QDate::currentDate();
    QString cycle = currentDate.toString("yyMM");
    // Special case
    if (currentDate.month() == 12 && currentDate.day() >= 7)
        cycle = currentDate.toString("yy13");
    QUrl url = QUrl(QString("https://www.faa.gov/air_traffic/flight_info/aeronav/digital_products/dtpp/search/results/?cycle=%1&ident=%2&page=%3").arg(cycle,m_airportID,QString::number(currentPage)));

    if (downloadJob)
    {
        downloadJob->disconnect(this);
        downloadJob->deleteLater();
    }

    downloadJob = new FileDownloader();

    connect(downloadJob, SIGNAL(downloaded()), this, SLOT(processDownloadSucess()));
    connect(downloadJob, SIGNAL(error(QString)), this, SLOT(processDownloadError(QString)));

    //QString output  = KSPaths::writableLocation(QStandardPaths::GenericDataLocation) + "catalog.min.json";
    //downloadJob->setDownloadedFileURL(QUrl::fromLocalFile(output));

    state = PARSE_HTML;

    downloadJob->get(url);
}

void FAAService::processDownloadSucess()
{
    if (state == PARSE_HTML)
    {
        QXmlSimpleReader xmlReader;
        QXmlInputSource *source = new QXmlInputSource();
        QString data = downloadJob->downloadedData().simplified();

        bool getNextPage=false;
        if (data.contains("Next &gt;</a>"))
            getNextPage = true;

        int openingBody = data.indexOf("<tbody>");
        int closingBody = data.indexOf("</tbody>");

        int diff = closingBody - openingBody + strlen("</tbody>");
        data = data.mid(openingBody, diff);

        //qDebug() << data;

        //HTMLParser *parser = new HTMLParser();
        //parser->parse(data);

        source->setData(data);

        MyContentHandler *handler = new MyContentHandler(m_airportID, getAirport, getMin, getSID, getStar, getApproach);
        xmlReader.setContentHandler(handler);
        xmlReader.setErrorHandler(handler);

        bool ok = xmlReader.parse(source);

        if (ok == false)
        {
            emit parseError(QString("Parsing failed %1.").arg(handler->errorString()));
            state = IDLE;
            return;
        }

        charts.append(handler->charts);

        delete (handler);
        delete (source);

        if (getNextPage)
        {
            currentPage++;
            downloadPage();
        }
        else
            emit parseComplete();

    }
}

void FAAService::processDownloadError(const QString &errorString)
{
    qCritical() << "Download Error:" << errorString;
    downloadJob->deleteLater();
    emit parseError(errorString);
}

void FAAService::stopDownload()
{
    qDebug() << QTime::currentTime().toString("HH:mm:ss.zzz") << "FAA service cancel download job";
    downloadJob->disconnect();
    downloadJob->cancel();
}
