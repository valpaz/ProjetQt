#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QMap<QWidget*, QString> tabName;
    //QMap<QTabWidget*, bool> tabStatue;
private slots:
    void nouveauFichier();
    void closeTab(const int& index);
    void ouvrirFichier();

private:

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
