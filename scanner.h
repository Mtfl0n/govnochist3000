#ifndef SCANNER_H
#define SCANNER_H

#include <QObject>
#include <QDir>
#include <QThread>
#include <QString>
#include <QThreadPool>
#include <QRunnable>
#include <atomic>
#include <QMutex>


class ScanTask : public QRunnable{
public:
    ScanTask(const QString& path, std::atomic<bool>& isStopped, QObject* emitter)
        : path(path), isStopped(isStopped), emitter(emitter){}
    void run() override{
        QDir dir(path);
        auto entries = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
        for (const QFileInfo& info : std::as_const(entries)) {
            if(isStopped)return;
            if(info.isDir()){
                QMetaObject::invokeMethod(emitter, "foundDirectory", Qt::QueuedConnection, Q_ARG(QString, info.absoluteFilePath()));
                QThreadPool::globalInstance()->start(new ScanTask(info.absoluteFilePath(), isStopped, emitter));

            }else {
                QMetaObject::invokeMethod(emitter, "foundFile", Qt::QueuedConnection,
                                          Q_ARG(QString, info.absoluteFilePath()),
                                          Q_ARG(qint64, info.size()));
            }
        }
    }


private:
    QString path;
    std::atomic<bool>& isStopped;
    QObject* emitter;
};

class Scanner : public QObject{
    Q_OBJECT

public slots:
    void startScan(const QString& path){
        isStopped = false;
        QThreadPool::globalInstance()->start(new ScanTask(path, isStopped, this));
    }
    void stopScan(){
        isStopped.store(true);
        QThreadPool::globalInstance()->waitForDone();
    }
signals:
    void foundDirectory(const QString& path);
    void foundFile(const QString& path, qint64 size);
private:
    std::atomic<bool> isStopped = false;
};


#endif // SCANNER_Hx
