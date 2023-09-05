#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QVBoxLayout>

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
    tabStatue[firstTabContent] = false;
    QPlainTextEdit *FirstTextEdit = new QPlainTextEdit;

    QVBoxLayout *firstLayout = new QVBoxLayout(firstTabContent);
    firstLayout->addWidget(FirstTextEdit);
    connect(FirstTextEdit, SIGNAL(textChanged()), this, SLOT(plainTextEditChanged()));
    // Actions //
    // Nouveau Fichier
    connect(ui->actionajout,SIGNAL(triggered()),this,SLOT(nouveauFichier()));
    // Ouvrir fichier
    connect(ui->actionouvrirFichier,SIGNAL(triggered()),this,SLOT(ouvrirFichier()));
    // fermer onglet
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    // Sauvegarder
    connect(ui->actionSauvegarder, SIGNAL(triggered()), this, SLOT(sauvegarderFichier()));
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
    connect(textEdit, SIGNAL(textChanged()), this, SLOT(plainTextEditChanged()));
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
    connect(textEdit, SIGNAL(textChanged()), this, SLOT(plainTextEditChanged()));
    QVBoxLayout *layout = new QVBoxLayout(newTabContent);
    layout->addWidget(textEdit);
    textEdit->setPlainText(fileContent);
}

void MainWindow::plainTextEditChanged() {
    QPlainTextEdit* senderTextEdit = qobject_cast<QPlainTextEdit*>(sender());
    for (int tabIndex = 0; tabIndex < ui->tabWidget->count(); ++tabIndex) {
        QWidget* tabContent = ui->tabWidget->widget(tabIndex);
        QPlainTextEdit* textEdit = tabContent->findChild<QPlainTextEdit*>();
        if (textEdit == senderTextEdit) {
            tabStatue[tabContent] = false;
            QString tabText = ui->tabWidget->tabText(tabIndex);
            if (!tabText.endsWith('*')) {
                ui->tabWidget->setTabText(tabIndex, tabText + "*");
            }
        }
    }
}

void MainWindow::sauvegarderFichier() {
    QWidget* currentTab = ui->tabWidget->currentWidget();
    QPlainTextEdit* textEdit = currentTab->findChild<QPlainTextEdit*>();
    QString filePath;
    if (tabName[currentTab].isEmpty()) {
        filePath = QFileDialog::getSaveFileName(this, "Sauvegarder le fichier", "/home", "Fichiers texte (*.txt);;Tous les fichiers (*.*)");
        tabName[currentTab] = filePath;
        QString nom = QFileInfo(filePath).fileName();
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), nom);
    }
    else{
        filePath = tabName[currentTab];
    }
    qDebug() << filePath;
    tabStatue[currentTab]=true;
    QString contentToSave = textEdit->toPlainText();
    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
        QTextStream out(&file);
        out << contentToSave;
        file.close();
        int tabIndex = ui->tabWidget->currentIndex();
        QString tabText = ui->tabWidget->tabText(tabIndex);
        if (tabText.endsWith('*'))
            {
            tabText.chop(1);
            ui->tabWidget->setTabText(tabIndex, tabText);
            }
        }
    else{
        qDebug() << "Échec de l'ouverture du fichier pour la sauvegarde.";
    }

}
