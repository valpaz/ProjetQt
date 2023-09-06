#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QVBoxLayout>
#include<QDebug>
#include <QFile>
#include <QFileDialog>
#include <QStatusBar>
#include <QShortcut>
#include <QInputDialog>
#include <QTextCharFormat>
#include <QtGui>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    // Initialisation //
    ui->lineEdit->hide();
    ui->toolButton->hide();
    ui->toolButton_2->hide();
    ui->lineEdit_2->hide();
    ui->lineEdit_3->hide();
    ui->toolButton_3->hide();
    ui->toolButton_4->hide();
    setWindowTitle("Editeur de texte");
    ui->tabWidget->setTabsClosable(true);
    ui->tabWidget->removeTab(1);
    QWidget *firstTabContent = ui->tabWidget->widget(0);
    tabName[firstTabContent] = "";
    tabStatue[firstTabContent] = false; // to know of the  file is save or not
    QPlainTextEdit *FirstTextEdit = new QPlainTextEdit;

    QVBoxLayout *firstLayout = new QVBoxLayout(firstTabContent);
    firstLayout->addWidget(FirstTextEdit);
    connect(FirstTextEdit, SIGNAL(textChanged()), this, SLOT(plainTextEditChanged()));
    connect(FirstTextEdit, SIGNAL(textChanged()), this, SLOT(cursorPosition()));


    // Actions //
    // Nouveau Fichier
    connect(ui->actionajout,SIGNAL(triggered()),this,SLOT(nouveauFichier()));
    // Ouvrir fichier
    connect(ui->actionouvrirFichier,SIGNAL(triggered()),this,SLOT(ouvrirFichier()));
    // fermer onglet
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    // Sauvegarder
    connect(ui->actionSauvegarder, SIGNAL(triggered()), this, SLOT(sauvegarderFichier()));
    // Rechercher
    connect(ui->actionBarre_de_recherche, SIGNAL(triggered()), this, SLOT(showResearchBar()));
    QShortcut* shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_F), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(showResearchBar()));
    connect(ui->toolButton, SIGNAL(clicked()), this, SLOT(hideResearchBar()));
    connect(ui->toolButton_2, SIGNAL(clicked()), this, SLOT(textSearch()));
    connect(ui->actionBarre_de_remplacement, SIGNAL(triggered()), this, SLOT(showReplaceBar()));
    connect(ui->toolButton_4, SIGNAL(clicked()), this, SLOT(hideReplaceBar()));
    connect(ui->toolButton_3, SIGNAL(clicked()), this, SLOT(textReplace()));

}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::hideResearchBar(){
    ui->lineEdit->hide();
    ui->toolButton->hide();
    ui->toolButton_2->hide();

    // disable the highlight text
    QWidget *currentTabContent = ui->tabWidget->currentWidget();
    QPlainTextEdit *currentTextEdit = currentTabContent->findChild<QPlainTextEdit*>();
    QTextCursor cursor = currentTextEdit->textCursor();
    currentTextEdit->setTextCursor(cursor);
    cursor.select(QTextCursor::Document);
    QTextCharFormat format;
    cursor.setCharFormat(format);
    cursor.setPosition(0);

}
void MainWindow::hideReplaceBar(){
    ui->lineEdit_2->hide();
    ui->lineEdit_3->hide();
    ui->toolButton_3->hide();
    ui->toolButton_4->hide();

    // disable the highlight text
    QWidget *currentTabContent = ui->tabWidget->currentWidget();
    QPlainTextEdit *currentTextEdit = currentTabContent->findChild<QPlainTextEdit*>();
    QTextCursor cursor = currentTextEdit->textCursor();
    currentTextEdit->setTextCursor(cursor);
    cursor.select(QTextCursor::Document);
    QTextCharFormat format;
    cursor.setCharFormat(format);
    cursor.setPosition(0);
}
void MainWindow::showResearchBar(){
    ui->lineEdit->show();
    ui->toolButton->show();
    ui->toolButton_2->show();
}
void MainWindow::showReplaceBar(){
    ui->lineEdit_2->show();
    ui->lineEdit_3->show();
    ui->toolButton_3->show();
    ui->toolButton_4->show();

}

void MainWindow::textReplace()
    {
        QString searchText = ui->lineEdit_2->text();
        QString replaceText = ui->lineEdit_3->text();
        QWidget *currentTabContent = ui->tabWidget->currentWidget();
        QPlainTextEdit *currentTextEdit = currentTabContent->findChild<QPlainTextEdit*>();
        if (currentTextEdit){
            QString textToSearch = currentTextEdit->toPlainText();

            // Obtenez la position actuelle du curseur
            QTextCursor cursor = currentTextEdit->textCursor();
            QTextCharFormat format;
            format.setBackground(QBrush(QColor("yellow")));
            QRegExp regex(searchText);
            int pos = 0;
            int index = regex.indexIn(textToSearch, pos);
            while (index != -1) {
                cursor.setPosition(index);
                cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
                cursor.mergeCharFormat(format);
                cursor.insertText(replaceText);

                // Move to the next match
                pos = index + regex.matchedLength();
                index = regex.indexIn(textToSearch, pos);

            }

        }
    }
void MainWindow::textSearch()
    {
        QString searchText = ui->lineEdit->text();
        QWidget *currentTabContent = ui->tabWidget->currentWidget();
        QPlainTextEdit *currentTextEdit = currentTabContent->findChild<QPlainTextEdit*>();
        if (currentTextEdit){
            QString textToSearch = currentTextEdit->toPlainText();

            // Obtenez la position actuelle du curseur
            QTextCursor cursor = currentTextEdit->textCursor();
            QTextCharFormat format;
            format.setBackground(QBrush(QColor("yellow")));
            QRegExp regex(searchText);
            int pos = 0;
            int index = regex.indexIn(textToSearch, pos);
            while (index != -1) {
                cursor.setPosition(index);
                cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);
                cursor.mergeCharFormat(format);

                // Move to the next match
                pos = index + regex.matchedLength();
                index = regex.indexIn(textToSearch, pos);

            }

        }
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
    connect(textEdit, SIGNAL(textChanged()), this, SLOT(cursorPosition()));
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
    connect(textEdit, SIGNAL(textChanged()), this, SLOT(cursorPosition()));
    QVBoxLayout *layout = new QVBoxLayout(newTabContent);
    layout->addWidget(textEdit);
    textEdit->setPlainText(fileContent);
}

void MainWindow::plainTextEditChanged() {
    // Add * to the title when the file have been modify //
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
void MainWindow::cursorPosition() {
    //Display cursor position //
    QPlainTextEdit* senderTextEdit = qobject_cast<QPlainTextEdit*>(sender());
    for (int tabIndex = 0; tabIndex < ui->tabWidget->count(); ++tabIndex) {
        QWidget* tabContent = ui->tabWidget->widget(tabIndex);
        QPlainTextEdit* textEdit = tabContent->findChild<QPlainTextEdit*>();
        if (textEdit == senderTextEdit) {
            QTextCursor cursor = textEdit->textCursor();
            int currentLine = cursor.blockNumber() + 1;
            int currentColumn = cursor.columnNumber() + 1;
            ui->statusbar->showMessage(QString("Ligne : %1, Colonne : %2").arg(currentLine).arg(currentColumn));
        }
    }
}
void MainWindow::sauvegarderFichier() {
    // Save file //
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


