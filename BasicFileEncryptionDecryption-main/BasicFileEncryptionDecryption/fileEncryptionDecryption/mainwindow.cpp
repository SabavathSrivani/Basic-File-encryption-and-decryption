/**
 * @file main.cpp
 * @brief File Encryption and Decryption Application
 *
 * This application provides a simple GUI interface for encrypting and decrypting files using a symmetric encryption algorithm.
 * It uses Qt framework for the GUI components and file operations.
 *
 * The encryption process increments each byte in the file by 1, while the decryption process decrements each byte by 1,
 * effectively performing a simple symmetric encryption/decryption algorithm.
 *
 * The user can select a file to encrypt or a previously encrypted file to decrypt using the provided GUI buttons.
 * A progress bar shows the progress of the encryption or decryption process.
 * The bytes of the file before encryption/decryption are displayed in the console.
 *
 * @note This application uses a simplistic encryption algorithm for demonstration purposes only.
 * It should not be considered secure for real-world encryption needs.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QString>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    progressBar = new QProgressBar(this);
    progressBar->setVisible(false);
    statusBar()->addWidget(progressBar);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_encryptionButton_clicked()
{
    /**
     * @brief Slot function for the encryption button click event.
     * Allows the user to select a file for encryption.
     * If a file is selected, it calls the encryptFile() function to perform the encryption.
     */
    
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select File"), "", tr("All Files (*.*)"));

    if (!filePath.isEmpty()) {
        encryptFile(filePath);
    }
}

void MainWindow::on_decryptionButton_clicked()
{
    /**
     * @brief Slot function for the decryption button click event.
     * Allows the user to select an encrypted file for decryption.
     * If a file is selected, it calls the decryptFile() function to perform the decryption.
     */
    
    QString filePath = QFileDialog::getOpenFileName(this, tr("Select Encrypted File"), "", tr("All Files (*.*)"));

    if (!filePath.isEmpty()) {
        decryptFile(filePath);
    }
}

void MainWindow::displayFileBytes(const QByteArray& fileData)
{
    /**
     * @brief Displays the bytes of the given file data in the console.
     * Only displays the last 16 bytes.
     *
     * @param fileData The byte array representing the file data.
     */
    
    qDebug() << "Last 16 File Bytes:";
    int startIndex = qMax(0, fileData.size() - 16);
    for (int i = startIndex; i < fileData.size(); ++i) {
        qDebug().noquote() << QString("%1").arg(static_cast<quint8>(fileData[i]), 2, 16, QLatin1Char('0'));
    }
}

void MainWindow::encryptFile(const QString &filePath)
{
    /**
     * @brief Encrypts the selected file.
     * Reads the file data, increments each byte by 1, and writes the encrypted data to a new file.
     * Displays the bytes of the file before encryption.
     * Shows a progress bar indicating the encryption progress.
     * Displays a success message or error message based on the encryption result.
     *
     * @param filePath The path of the file to be encrypted.
     */
    
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray fileData = file.readAll();
        file.close();

        displayFileBytes(fileData); // Displays bytes before encryption.

        QString encryptedFilePath = filePath + ".encrypted";
        QFile encryptedFile(encryptedFilePath);
        if (encryptedFile.open(QIODevice::WriteOnly)) {
            progressBar->setVisible(true);
            progressBar->setValue(0);
            progressBar->setRange(0, fileData.size());

            for (int i = 0; i < fileData.size(); i++) {
                char encryptedByte = fileData[i] + 1; // incrementing each byte by 1
                encryptedFile.write(&encryptedByte, sizeof(encryptedByte));
                progressBar->setValue(i);
            }

            progressBar->setVisible(false);
            encryptedFile.close();
            QMessageBox::information(this, tr("Encryption"), tr("File encrypted succesfully!"));
        } else {
            QMessageBox::critical(this, tr("Encryption"), tr("Failed to write encrypted file."));
        }
    } else {
        QMessageBox::critical(this, tr("Encryption"), tr("Failed to open file for reading."));

    }
}

void MainWindow::decryptFile(const QString& filePath)
{
    /**
     * @brief Decrypts the selected encrypted file.
     * Reads the encrypted file data, decrements each byte by 1, and writes the decrypted data to a new file.
     * Displays the bytes of the file before decryption.
     * Shows a progress bar indicating the decryption progress.
     * Displays a success message or error message based on the decryption result.
     *
     * @param filePath The path of the encrypted file to be decrypted.
     */
    
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray fileData = file.readAll();
        file.close();

        displayFileBytes(fileData); // Displays bytes before decryption.

        QString decryptedFilePath = filePath.left(filePath.lastIndexOf(".encrypted"));
        QFile decryptedFile(decryptedFilePath);
        if (decryptedFile.open(QIODevice::WriteOnly)) {
            progressBar->setVisible(true);
            progressBar->setValue(0);
            progressBar->setRange(0,fileData.size());

            for (int i = 0; i < fileData.size(); i ++) {
                char decryptedByte = fileData[i] - 1; // decrementing each byte by 1
                decryptedFile.write(&decryptedByte, sizeof(decryptedByte));
                progressBar->setValue(i);
            }

            progressBar->setVisible(false);
            decryptedFile.close();
            QMessageBox::information(this, tr("Decryption"), tr("File decrypted succesfully!"));
        } else {
            QMessageBox::critical(this, tr("Decryption"), tr("Failed to write decrypted file"));
        }
    } else {
        QMessageBox::critical(this, tr("Decryption"), tr("Failed to open file for reading"));
    }
}
