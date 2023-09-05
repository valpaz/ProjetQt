#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QVBoxLayout>
#include<QPlainTextEdit>
#include<QDebug>
#include <QFile>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);

    // Initialisation //
    setWindowTitle("Editeur de texte");
    ui->tabWidget->setTabsClosable(true);
    ui->tabWidget->removeTab(1);
    QWidget *firstTabContent = ui->tabWidget->widget(0);
    tabName[firstTabContent] = "";
    tabStatue[newTabContent] = false;
    QPlainTextEdit *FirstTextEdit = new QPlainTextEdit;
    QVBoxLayout *firstLayout = new QVBoxLayout(firstTabContent);
    firstLayout->addWidget(FirstTextEdit);

    // Actions //
    // Nouveau Fichier
    connect(ui->actionajout,SIGNAL(triggered()),this,SLOT(nouveauFichier()));
    // Ouvrir fichier
    connect(ui->actionouvrirFichier,SIGNAL(triggered()),this,SLOT(ouvrirFichier()));
    // fermer onglet
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    // check si non enregistrer
    //connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::nouveauFichier()
{
    // Ajout nouveau tab
    QWidget *newTabContent = new QWidget;
    tabName[newTabContent] = "";
    tabStatue[newTabContent] = false;
    ui->tabWidget->addTab(newTabContent, "nouveau fichier");
    QPlainTextEdit *textEdit = new QPlainTextEdit;
    QVBoxLayout *layout = new QVBoxLayout(newTabContent);
    layout->addWidget(textEdit);

}

void MainWindow::closeTab(const int& index)
{
    if (index == -1) {
        return;
    }

    QWidget* tabItem = ui->tabWidget->widget(index);
    ui->tabWidget->removeTab(index);

    delete(tabItem);
    tabItem = nullptr;
}

void MainWindow::ouvrirFichier(){

    // Ouvre une boite de dialogue pour recuperer un fichier
    QString fileName = QFileDialog::getOpenFileName(this,
    "ouvrir fichier", "/home",QString());

    // Ouvre le fichier et recupere le contenu dans fileContent
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QString fileContent;
    while (!file.atEnd())
    {
        QString ligne = file.readLine();
        fileContent+=ligne;
        fileContent+="\n";
    }

    // recupere le nom du fichier
    QFileInfo fileInfo(fileName);
    QString justFileName = fileInfo.fileName();
    // Creer un nouvel onglet  et ajoute le contenu de fileContent
    QWidget *newTabContent = new QWidget;
    tabName[newTabContent] = fileName;
    tabStatue[newTabContent] = true;
    ui->tabWidget->addTab(newTabContent, justFileName);
    QPlainTextEdit *textEdit = new QPlainTextEdit;
    QVBoxLayout *layout = new QVBoxLayout(newTabContent);
    layout->addWidget(textEdit);
    textEdit->setPlainText(fileContent);
}

