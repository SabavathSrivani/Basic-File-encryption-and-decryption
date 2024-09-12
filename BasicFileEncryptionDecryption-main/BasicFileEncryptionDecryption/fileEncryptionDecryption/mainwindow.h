#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QProgressBar>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QProgressBar *progressBar;

    void displayFileBytes(const QByteArray& fileData);
    void encryptFile(const QString& filePath);
    void decryptFile(const QString& filePath);

private slots:
    void on_encryptionButton_clicked();
    void on_decryptionButton_clicked();
};
#endif // MAINWINDOW_H
