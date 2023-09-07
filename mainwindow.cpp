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
    recentFileLoading();

    setWindowTitle("textEditor");
    ui->tabWidget->setTabsClosable(true);
    ui->tabWidget->removeTab(1);
    QWidget *firstTabContent = ui->tabWidget->widget(0);
    tabName[firstTabContent] = ""; // associate a tab to a path
    tabStatue[firstTabContent] = false; // to know of the file is save or not
    QPlainTextEdit *FirstTextEdit = new QPlainTextEdit; // Add plain text to the default tab
    QVBoxLayout *firstLayout = new QVBoxLayout(firstTabContent);
    firstLayout->addWidget(FirstTextEdit);
    // Connect default plain text
    connect(FirstTextEdit, SIGNAL(textChanged()), this, SLOT(plainTextEditChanged()));
    connect(FirstTextEdit, SIGNAL(textChanged()), this, SLOT(cursorPosition()));


    // Actions //

    // new File connect
    connect(ui->actionajout,SIGNAL(triggered()),this,SLOT(newFile()));
    // Open file connect
    connect(ui->actionouvrirFichier,SIGNAL(triggered()),this,SLOT(openFile()));
    // close file connect
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
    // Save connect
    connect(ui->actionSauvegarder, SIGNAL(triggered()), this, SLOT(saveFile()));
    // Search connect & shortcut F
    connect(ui->actionBarre_de_recherche, SIGNAL(triggered()), this, SLOT(showResearchBar()));
    QShortcut* shortcut = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_F), this);
    connect(shortcut, SIGNAL(activated()), this, SLOT(showResearchBar()));
    connect(ui->toolButton, SIGNAL(clicked()), this, SLOT(hideResearchBar()));
    connect(ui->toolButton_2, SIGNAL(clicked()), this, SLOT(textSearch()));
    // Replace connect & shortcut H
    connect(ui->actionBarre_de_remplacement, SIGNAL(triggered()), this, SLOT(showReplaceBar()));
    QShortcut* shortcut1 = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_H), this);
    connect(shortcut1, SIGNAL(activated()), this, SLOT(showReplaceBar()));
    connect(ui->toolButton_4, SIGNAL(clicked()), this, SLOT(hideReplaceBar()));
    connect(ui->toolButton_3, SIGNAL(clicked()), this, SLOT(textReplace()));

}

void MainWindow::recentFileLoading() {
    // Load last used files in recent file menu

    QStringList recentFiles = settings->allKeys();
    // Run through the settings object (QSettings)
    foreach (const QString &recentFile, recentFiles) {
        QString filePath = settings->value(recentFile).toString(); // get path
        QAction* recentFileAction = new QAction(recentFile, this); // Create action
        menuAjout->addAction(recentFileAction);

        connect(recentFileAction, SIGNAL(triggered()), this, SLOT(OpenRecentFile()));
    }
    ui->actionFichiers_r_cents->setMenu(menuAjout); // link new files sub menu to recent file menu
}


void MainWindow::addToRecentFiles(const QString &filePath) {
    // This function add to the recent file menu the files that have been open or save
    if (!recentFiles.contains(filePath)) {
        // Limit the nombre of recent file to 10
        if (recentFiles.size() >= 10) {

            QString fileToRemove = recentFiles.first();
            QFileInfo fileInfo(fileToRemove);
            QString fileName = fileInfo.fileName();
            // remove the oldest file from settings, recentFiles and menuAjout (the sub menu)
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

        // get path and file name
        QFileInfo fileInfo(filePath);
        QString justFileName = fileInfo.fileName();
        settings->setValue(justFileName, filePath);
        // add the new action
        QAction* recentFileAction = new QAction(justFileName, this);
        menuAjout->addAction(recentFileAction);

        // add the new sub menu
        ui->actionFichiers_r_cents->setMenu(menuAjout);
        // connect it so that it can be open on trigger
        connect(recentFileAction, SIGNAL(triggered()), this, SLOT(OpenRecentFile()));


    settings->sync();
    }

}
void MainWindow::OpenRecentFile() {
    // This function open a new file through the menu action
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
    // This function hide the search bar when close is clicked & disable the highlight
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
    // This function hide the replace bar when close is clicked & disable the highlight
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
void MainWindow::textReplace(){
    // This function replace a given pattern by another
    // get strings
    QString searchText = ui->lineEdit_2->text();
    QString replaceText = ui->lineEdit_3->text();
    QWidget *currentTabContent = ui->tabWidget->currentWidget();
    QPlainTextEdit *currentTextEdit = currentTabContent->findChild<QPlainTextEdit*>();
    if (currentTextEdit){
        QString textToSearch = currentTextEdit->toPlainText();

        // get the ccursor of the current plain text
        QTextCursor cursor = currentTextEdit->textCursor();
        QTextCharFormat format;
        format.setBackground(QBrush(QColor("yellow")));
        QRegExp regex(searchText);
        int pos = 0;
        // search the first match
        int index = regex.indexIn(textToSearch, pos);

        // loop through the matching patterns
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
void MainWindow::textSearch(){
    // This function search for a given pattern
    // get strings
    QString searchText = ui->lineEdit->text();
    QWidget *currentTabContent = ui->tabWidget->currentWidget();
    QPlainTextEdit *currentTextEdit = currentTabContent->findChild<QPlainTextEdit*>();
    if (currentTextEdit){
        QString textToSearch = currentTextEdit->toPlainText();

        // get the ccursor of the current plain text
        QTextCursor cursor = currentTextEdit->textCursor();
        QTextCharFormat format;
        format.setBackground(QBrush(QColor("yellow")));
        QRegExp regex(searchText, Qt::CaseInsensitive);
        int pos = 0;
        int index = regex.indexIn(textToSearch, pos);

        // loop through the matching patterns
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
void MainWindow::newFile()
{
    // Add a new tab and its associated plain text objet
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
void MainWindow::closeTab(const int& index){
    // This function close the tab when table cros is clicked
    if (index == -1) {
        return;
    }
    // Remove the tab object
    QWidget* tabItem = ui->tabWidget->widget(index);
    ui->tabWidget->removeTab(index);
    delete(tabItem);
}
void MainWindow::openFile(){
    // This function open a new file through the open file menu
    // Create a dialog box
    QString fileName = QFileDialog::getOpenFileName(this,
    "ouvrir fichier", "/home",QString());
    // Open the file
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QString fileContent;

    // retrieve the content
    while (!file.atEnd())
    {
        QString ligne = file.readLine();
        fileContent+=ligne;
        fileContent+="\n";
    }
    // Get file name
    addToRecentFiles(fileName);
    QFileInfo fileInfo(fileName);
    QString justFileName = fileInfo.fileName();

    // Create a new widget item and plain text
    QWidget *newTabContent = new QWidget;
    tabName[newTabContent] = fileName;
    tabStatue[newTabContent] = true;
    ui->tabWidget->addTab(newTabContent, justFileName);
    QPlainTextEdit *textEdit = new QPlainTextEdit;
    connect(textEdit, SIGNAL(textChanged()), this, SLOT(plainTextEditChanged()));
    connect(textEdit, SIGNAL(textChanged()), this, SLOT(cursorPosition()));
    QVBoxLayout *layout = new QVBoxLayout(newTabContent);
    layout->addWidget(textEdit);

    // add the file content to the newly created tab
    textEdit->setPlainText(fileContent);

}

void MainWindow::plainTextEditChanged() {
    // Add * to the title when the plain text have been trigger by a text change
    QPlainTextEdit* senderTextEdit = qobject_cast<QPlainTextEdit*>(sender());
    // loop to find the concerned tab
    for (int tabIndex = 0; tabIndex < ui->tabWidget->count(); ++tabIndex) {
        QWidget* tabContent = ui->tabWidget->widget(tabIndex);
        QPlainTextEdit* textEdit = tabContent->findChild<QPlainTextEdit*>();
        if (textEdit == senderTextEdit) {
            tabStatue[tabContent] = false;
            QString tabText = ui->tabWidget->tabText(tabIndex);
            // add the * to the tab name
            if (!tabText.endsWith('*')) {
                ui->tabWidget->setTabText(tabIndex, tabText + "*");
            }
        }
    }

}
void MainWindow::cursorPosition() {
    // This function display cursor position
    QPlainTextEdit* senderTextEdit = qobject_cast<QPlainTextEdit*>(sender());
    // Loop to find the current tab
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
void MainWindow::saveFile() {
    // This function save a file
    // find the current tab and plain text
    QWidget* currentTab = ui->tabWidget->currentWidget();
    QPlainTextEdit* textEdit = currentTab->findChild<QPlainTextEdit*>();
    QString filePath;
    if (tabName[currentTab].isEmpty()) {
        // create a window to save the file and retrieve the path
        filePath = QFileDialog::getSaveFileName(this, "Sauvegarder le fichier", "/home", "Fichiers texte (*.txt);;Tous les fichiers (*.*)");
        tabName[currentTab] = filePath;
        QString nom = QFileInfo(filePath).fileName();
        // Give the tab the name of the save file
        ui->tabWidget->setTabText(ui->tabWidget->currentIndex(), nom);
    }
    else{
        filePath = tabName[currentTab];
    }
    // add to recent file menu
    addToRecentFiles(filePath);
    tabStatue[currentTab]=true;
    QString contentToSave = textEdit->toPlainText();
    QFile file(filePath);
    // create the file and add the tab content to it
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
        QTextStream out(&file);
        out << contentToSave;
        file.close();
        // remove the * from the tab name
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


