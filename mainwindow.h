#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "qtreewidget.h"
#include "scanner.h"
#include <QMainWindow>
#include <QPlainTextEdit>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_startButton_clicked();
    void on_stopButton_clicked();
    void addDirectory(const QString& path);
    void addFile(const QString& path, qint64 size);
    void on_comboBox_activated(int index);



signals:
    void startScanning(const QString& path);
    void stopScanning();

private:
    Ui::MainWindow *ui;
    Scanner* scanner;

    QTreeWidgetItem* currentParent = nullptr;
    QMap<QString, QTreeWidgetItem*> dirItems;
    void updateParentSizes(QTreeWidgetItem* item, qint64 size);
    void populateDrives();
    QString formatSize(qint64 bytes);
    QPlainTextEdit* logWidget;
};
#endif // MAINWINDOW_H
