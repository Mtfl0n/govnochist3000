#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QThread>
#include <QFileDialog>
#include "scanner.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), scanner(nullptr) {
    ui->setupUi(this);
    populateDrives();




    scanner = new Scanner();
    connect(this, &MainWindow::startScanning, scanner, &Scanner::startScan);
    connect(this, &MainWindow::stopScanning, scanner, &Scanner::stopScan);
    connect(scanner, &Scanner::foundDirectory, this, &MainWindow::addDirectory);
    connect(scanner, &Scanner::foundFile, this, &MainWindow::addFile);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete scanner;
}


void MainWindow::on_startButton_clicked() {
    ui->treeWidget->clear();// очищеает дерево
    dirItems.clear();// очищеает мапу [путь к директории - указатель на элемент дерева ]
    QString path = ui->comboBox->currentText();
    if (path.isEmpty() || path == "Выбрать папку...") return;

    QTreeWidgetItem* rootItem = new QTreeWidgetItem(ui->treeWidget);
    rootItem->setText(0, path);
    dirItems[path] = rootItem;


    emit startScanning(path);
}



void MainWindow::on_stopButton_clicked()
{
    if(scanner){
        qDebug() << "Stop button clicked";
        emit stopScanning();

    }
}

void MainWindow::on_comboBox_activated(int index) {
    QString selectedPath = ui->comboBox->itemText(index);
    if (selectedPath == "Выбрать папку...") {
        QString dir = QFileDialog::getExistingDirectory(this, "Выберите папку", QDir::homePath());
        if (!dir.isEmpty()) {
            if (ui->comboBox->findText(dir) == -1) {
                ui->comboBox->insertItem(ui->comboBox->count() - 1, dir);
            }
            ui->comboBox->setCurrentText(dir);
        }
    }
}
void MainWindow::updateParentSizes(QTreeWidgetItem* item, qint64 size) {
    while (item) {
        qint64 currentSize = item->data(1, Qt::UserRole).toLongLong() + size;
        item->setData(1, Qt::UserRole, currentSize);
        item->setText(1, formatSize(currentSize));
        item = item->parent();
    }
}

void MainWindow::populateDrives() {
    ui->comboBox->clear();
    QFileInfoList drives = QDir::drives();
    for (const QFileInfo& drive : std::as_const(drives)) {
        ui->comboBox->addItem(drive.absoluteFilePath());
    }
    ui->comboBox->addItem("Выбрать папку..."); 
}

void MainWindow::addDirectory(const QString& path) {
    QFileInfo info(path);
    QString parentPath = info.dir().absolutePath();

    QTreeWidgetItem* parentItem = dirItems.value(parentPath);
    if (!parentItem) parentItem = dirItems.value(path);


    QTreeWidgetItem* item = new QTreeWidgetItem(parentItem);
    item->setText(0, info.fileName());
    item->setChildIndicatorPolicy(QTreeWidgetItem::ShowIndicator); 
    dirItems[path] = item;
}


void MainWindow::addFile(const QString& path, qint64 size) {
    QFileInfo info(path);
    QString dirPath = info.dir().absolutePath();
    QTreeWidgetItem* parentItem = dirItems.value(dirPath);
    if(!parentItem)return;
    QTreeWidgetItem* item = new QTreeWidgetItem(parentItem);
    item->setText(0, info.fileName());
    item->setText(1, formatSize(size));

    updateParentSizes(parentItem, size);
}

QString MainWindow::formatSize(qint64 bytes) {
    constexpr qint64 KB = 1024;
    constexpr qint64 MB = KB * 1024;
    constexpr qint64 GB = MB * 1024;

    if (bytes >= GB) return QString::number(bytes / (double)GB, 'f', 2) + " GB";
    if (bytes >= MB) return QString::number(bytes / (double)MB, 'f', 2) + " MB";
    if (bytes >= KB) return QString::number(bytes / (double)KB, 'f', 2) + " KB";
    return QString::number(bytes) + " B";
}


