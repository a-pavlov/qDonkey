#include "file_downloader.h"
#include <QStringList>
#include <QDebug>
#include <QApplication>
#ifdef Q_WS_WIN
#include <windows.h>
#endif

FileDownloader::FileDownloader(const QUrl& url, const QString& filepath, QObject *parent) :
    QObject(parent),
    m_aborted(false),
    m_reply(NULL),
    m_url(url),
    m_filepath(filepath)
{
    m_nm.reset(new QNetworkAccessManager);
    qDebug() << "download from " << url << " to " << filepath;
}

FileDownloader::~FileDownloader()
{
    if (m_reply) m_reply->abort();
}

void FileDownloader::start()
{
    Q_ASSERT(!m_aborted);

    bool hasPlace = !QFile::exists(m_filepath);
    if (!hasPlace) {
        if (QFile::remove(m_filepath)) hasPlace = true;
    }

    if (!hasPlace)
    {
        qDebug() << "unable to remove file " << m_filepath;
        m_aborted = true;
        emit completed(-1, 0);
        return;
    }

    m_file.reset(new QFile(m_filepath));

    // check file
    if (!m_file->open(QIODevice::WriteOnly))
    {
        qDebug() << "can't create file " << m_filepath;
        m_aborted = true;
        emit completed(m_file->error(), 0);
        return;
    }

    //QList<QSslCertificate> cert = QSslCertificate::fromPath(QLatin1String("server-certificate.pem"));
    //QSslError error(QSslError::SelfSignedCertificate, cert.at(0));
    //QList<QSslError> expectedSslErrors;
    //expectedSslErrors.append(error);

    m_reply = m_nm->get(QNetworkRequest(m_url));
    connect(m_reply, SIGNAL(readyRead()), SLOT(on_data_ready()));
    connect(m_reply, SIGNAL(finished()), SLOT(on_data_finished()));
}

void FileDownloader::on_data_ready()
{
    qDebug() << "receive data size: " << m_reply->size();

    if (!m_aborted && m_file)
    {
        if (m_file->write(m_reply->readAll()) == -1)
        {
            qDebug() << "error on write data in update file";
            m_aborted = true;
            emit completed(m_file->error(), 0);
            m_reply->close();
            m_file->remove();
        }
    }
}

void FileDownloader::on_data_finished()
{
    if (m_aborted) return;

    qDebug() << "data finished";
    // read last part
    on_data_ready();
    m_file->flush();
    if (m_reply->error()) m_file->remove();
    m_file.reset();
    emit completed(m_reply->error(), 1);
    m_reply->deleteLater();
    m_reply = NULL;
}

