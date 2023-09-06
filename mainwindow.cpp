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


    // Initialization //
    ui->lineEdit->hide();
    ui->toolButton->hide();
    ui->toolButton_2->hide();
    ui->lineEdit_2->hide();
    ui->lineEdit_3->hide();
    ui->toolButton_3->hide();
    ui->toolButton_4->hide();
    menuAjout = new QMenu("fichier recent");
    settings = new QSettings("MonApplication", "MonEditeurDeTexte");
    chargerFichierRecentNouveauLancement();

    setWindowTitle("Editeur de texte");
    ui->tabWidget->setTabsClosable(true);
    ui->tabWidget->removeTab(1);
    QWidget *firstTabContent = ui->tabWidget->widget(0);
    tabName[firstTabContent] = ""; // assoocie un tab  a un path
    tabStatue[firstTabContent] = false; // to know of the file is save or not
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
    // Remplacer
    connect(ui->actionBarre_de_remplacement, SIGNAL(triggered()), this, SLOT(showReplaceBar()));
    QShortcut* shortcut1 = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_H), this);
    connect(shortcut1, SIGNAL(activated()), this, SLOT(showReplaceBar()));
    connect(ui->toolButton_4, SIGNAL(clicked()), this, SLOT(hideReplaceBar()));
    connect(ui->toolButton_3, SIGNAL(clicked()), this, SLOT(textReplace()));

}

void MainWindow::chargerFichierRecentNouveauLancement() {
    // Charge les fichiers dernierement utilisés

    QStringList recentFiles = settings->allKeys();
    // Parcours l'objet  settings (QSettings)
    foreach (const QString &recentFile, recentFiles) {
        QString filePath = settings->value(recentFile).toString(); // Recup Path
        QAction* recentFileAction = new QAction(recentFile, this); // Creer objet Action
        menuAjout->addAction(recentFileAction);
        ui->actionFichiers_r_cents->setMenu(menuAjout);
        connect(recentFileAction, SIGNAL(triggered()), this, SLOT(actionOuvrir()));
    }
}


void MainWindow::addToRecentFiles(const QString &filePath) {
    if (!recentFiles.contains(filePath)) {
        if (recentFiles.size() >= 10) {

            QString fileToRemove = recentFiles.first();
            QFileInfo fileInfo(fileToRemove);
            QString fileName = fileInfo.fileName();

            settings->remove(fileName);
            recentFiles.removeAt(0);

            QList<QAction*> actions = menuAjout->actions();
            foreach (QAction* action, actions) {
                if (action->text() == fileName) {
                    menuAjout->removeAction(action);
                    break;
                }
            }
        }
        recentFiles.append(filePath);


        QFileInfo fileInfo(filePath);
        QString justFileName = fileInfo.fileName();
        settings->setValue(justFileName, filePath);

        QAction* recentFileAction = new QAction(justFileName, this);
        menuAjout->addAction(recentFileAction);
        int tailleMenu = menuAjout->actions().size();

        qDebug() <<"taille menu" << tailleMenu;
        ui->actionFichiers_r_cents->setMenu(menuAjout);

        connect(recentFileAction, SIGNAL(triggered()), this, SLOT(actionOuvrir()));


    settings->sync();
    }

}
void MainWindow::actionOuvrir() {
    QAction* senderAction = qobject_cast<QAction*>(sender());
    if (senderAction) {
        QString nomFichierRecent = senderAction->text();
        QString filePath = settings->value(nomFichierRecent).toString();
        if (!filePath.isEmpty()) {
            QPlainTextEdit* newTextEdit = new QPlainTextEdit(this);
            ui->tabWidget->addTab(newTextEdit, nomFichierRecent);
            QFile file(filePath);
            if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
                QTextStream in(&file);
                QString fileContent = in.readAll();
                file.close();
                newTextEdit->setPlainText(fileContent);

            } else {
                qDebug() << "Échec de l'ouverture du fichier récent : " << filePath;
            }
        }
    }
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
    addToRecentFiles(fileName);
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
    addToRecentFiles(filePath);
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

MainWindow::~MainWindow()
{
    delete ui;
}


