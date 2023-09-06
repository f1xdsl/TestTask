
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include<QMessageBox>
#include<QVector>
#include<QQueue>
#include<QImage>
#include<QPixmap>
#include<QFileInfo>

QString bytesToBin(QByteArray inputBytes){ //Перевод из байтов в двоичный код

    QString result; // результирующая строка

    inputBytes = inputBytes.toHex(); //перевод байтов из вида "\x89\xff" в вид "89ff"

    for(int i = 0; i < inputBytes.length(); i++){

        QString strnum;
        QString newNum;
        uint num;

        strnum = char(inputBytes[i]);

        num = strnum.toUInt(nullptr, 16);//перевод полубайта в 10-ю систему

        newNum = QString("%1").arg(num,4,2,QChar('0')); // перевод в двоичную систему
        result += newNum; // запись в результирующую строку
    }
    return result;
}

QByteArray binToBytes(QString& inputString){

    QByteArray outputByteArray;
    uint num10;
    QString num16;

    for(int i = 0; i < inputString.length(); i += 4){

        num10 = inputString.mid(i,4).toUInt(nullptr,2); //перевод полубайта в 10-ю систему
        num16 = QString("%1").arg(num10,1,16); // перевод в 16-ю систему

        outputByteArray.append(num16); //запись в выходной массив байтов в 16-й системе
    }

    outputByteArray = outputByteArray.fromHex(outputByteArray); //перевод в байтовый вид

    return outputByteArray;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{

    //В открывшемся окне необходимо выбрать нужное изображение, результатом работы программы будут файл
    //binResult.txt, содержащий переведенное в двоичный код изображение и файл resultImage.* с расширением
    //выбранного файла, содержащий восстановленное из двоичного кода изображение

    QString file_name = QFileDialog::getOpenFileName(this,"Open File",QDir::homePath(),"Images (*.jpg *.png *.xpm)");


    if(!file_name.isEmpty()){

        QFileInfo file(file_name); //полный путь входного файла
        QString fileDir = file.path(); // извлечение директории входного файла
        QString resultFileName = fileDir+"/resultFile.txt"; // полный путь выходного файла с двоичным кодом
        QString resultImageName = fileDir+"/resultImage."+file.suffix(); // полный путь выходного файла с восстановленным изображением



        QImage img(file_name);

        QPixmap pix = QPixmap::fromImage(img);

        int h = ui->label_pic->height();
        int w = ui->label_pic->width();

        ui->label_pic->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio)); //вывод выбранного файла в окно программы

        QFile inputImage(file_name);

        QFile resultFile(resultFileName);
        QFile inputFile(resultFileName);

        QImage resultImage;
        QByteArray inputBytes;
        QByteArray byteResult;
        QString inputBin;
        QString binResult;

        inputImage.open(QIODevice::ReadOnly);
        inputBytes = inputImage.readAll(); //считывание информации с входного файла в байтовом формате
        binResult = bytesToBin(inputBytes);
        inputImage.close();

        resultFile.open(QIODevice::WriteOnly);
        resultFile.write(binResult.toUtf8()); // запись двоичного кода в результирующий файл
        resultFile.close();

        inputFile.open(QIODevice::ReadOnly);
        inputBin = inputFile.readAll();
        inputFile.close();

        byteResult = binToBytes(inputBin);
        resultImage.loadFromData(byteResult);
        resultImage.save(resultImageName);

        QMessageBox::information(this,"...", "Picture processed successfully");
    }
}
