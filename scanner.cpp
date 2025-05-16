// #include "scanner.h"

// void Scanner::startScan(const QString& path) {
//     isStopped = false;
//     scanDirectory(path);
// }

// void Scanner::scanDirectory(const QString& path){
//     QStack<QString> dirStack;
//     dirStack.push(path);

//     for(;!dirStack.isEmpty() && !isStopped;)
//     {
//         QString currentPath = dirStack.pop();
//         QDir dir(currentPath);

//         auto entries = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);

//         if(entries.isEmpty())continue;
//         for(const QFileInfo& info : std::as_const(entries))
//         {
//             if(isStopped){
//                 qDebug() << "Сканирование остановилось в : " << currentPath;
//                 return;
//             }
//             if(info.isDir())
//             {
//                 emit foundDirectory(info.absoluteFilePath());
//                 dirStack.push(info.absoluteFilePath());
//             }else if(info.isFile()){
//                 emit foundFile(info.absoluteFilePath(), info.size());
//             }
//         }
//     }
//     if (isStopped) {
//         qDebug() << "Сканирование остановлено";
//     }
// }
