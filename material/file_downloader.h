#ifndef FILE_DOWNLOADER_H
#define FILE_DOWNLOADER_H

#include <QObject>
#include <QTimer>
#include <QScopedPointer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFile>

class FileDownloader : public QObject
{
    Q_OBJECT
public:    
    explicit FileDownloader(const QUrl& url, const QString& target, QObject *parent = 0);
    ~FileDownloader();
    void start();
private:
    bool                    m_aborted;
    QNetworkReply*          m_reply;        //!< download reply
    QScopedPointer<QNetworkAccessManager> m_nm;
    QScopedPointer<QFile>                 m_file;
    QUrl                    m_url;
    QString                 m_filepath;
signals:
    /**
     * @brief completed
     * @param code - FileError or NetworkError
     * @param system - 0 for file and 1 for network systems
     */
    void completed(int code, int system);
private slots:
    void on_data_finished();
    void on_data_ready();

};

#endif // FILE_DOWNLOADER_H
