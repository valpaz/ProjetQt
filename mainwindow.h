#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include<QPlainTextEdit>
#include <QSettings>
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
    QMap<QWidget*, bool> tabStatue;
    void addToRecentFiles(const QString &filePath);
    QStringList recentFiles;
    QSettings *settings;
    QMenu* menuAjout;
    void chargerFichierRecentNouveauLancement();

private slots:
    void nouveauFichier();
    void closeTab(const int& index);
    void ouvrirFichier();
    void plainTextEditChanged();
    void sauvegarderFichier();
    void cursorPosition();
    void showResearchBar();
    void hideResearchBar();
    void showReplaceBar();
    void hideReplaceBar();
    void textSearch();
    void textReplace();
    void actionOuvrir();

private:

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
