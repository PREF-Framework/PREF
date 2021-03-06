#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "widgets/views/binaryview.h"
#include "dialogs/aboutdialog.h"
#include <QDesktopWidget>
#include <QCloseEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QMimeData>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), _currentview(NULL)
{
    ui->setupUi(this);

    this->_lblstatus = new QLabel();
    ui->statusBar->addPermanentWidget(this->_lblstatus, 1);

    this->centerWindow();
    this->setAcceptDrops(true);
    this->updateToolBar(NULL);
    this->parseCommandLine();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if(!e->mimeData()->hasUrls())
        return;

    e->acceptProposedAction();
}

void MainWindow::dragMoveEvent(QDragMoveEvent *e)
{
    if(!e->mimeData()->hasUrls())
        return;

    e->acceptProposedAction();
}

void MainWindow::dropEvent(QDropEvent *e)
{
    const QMimeData* mimedata = e->mimeData();

    if(!mimedata->hasUrls())
        return;

      QList<QUrl> urllist = mimedata->urls();
      QString locfile = urllist.first().toLocalFile();
      QFileInfo fi(locfile);

      if(!fi.isFile())
          return;

      this->loadFile(locfile);
      e->acceptProposedAction();
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    if(this->closeApplication())
    {
        e->accept();
        QMainWindow::closeEvent(e);
        return;
    }

    e->ignore();
}

void MainWindow::updateToolBar(AbstractView *abstractview) const
{
    ui->toolBar->clear();
    ui->toolBar->setToolButtonStyle(abstractview ? Qt::ToolButtonTextUnderIcon : Qt::ToolButtonTextBesideIcon);
    ui->toolBar->addAction(ui->action_Analyze);

    if(abstractview)
        abstractview->updateToolBar(ui->toolBar);

    ui->toolBar->addAction(ui->action_About_PREF);
}

void MainWindow::centerWindow()
{
    QRect position = this->frameGeometry();
    position.moveCenter(QDesktopWidget().availableGeometry().center());
    this->move(position.topLeft());
}

void MainWindow::parseCommandLine()
{
    QStringList args = qApp->arguments();

    if(args.length() <= 1)
        return;

    this->loadFile(args[1]);
}

bool MainWindow::closeApplication() const
{
    if(!this->_currentview)
        return true;

    QMessageBox m;
    m.setWindowTitle(tr("Closing..."));
    m.setText(tr("Do you want to close PREF?"));
    m.setIcon(QMessageBox::Question);
    m.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    m.setDefaultButton(QMessageBox::No);

    return m.exec() == QMessageBox::Yes;
}

void MainWindow::loadFile(const QString &file)
{
    QFileInfo fi(file);

    if(!fi.isFile())
        return;

    if(this->_currentview)
        this->_currentview->deleteLater();

    this->setWindowTitle(fi.fileName());

    this->_currentview = new BinaryView(QHexDocument::fromFile(file), this->_lblstatus, file, this);
    this->setCentralWidget(this->_currentview);
    this->updateToolBar(this->_currentview);
}

void MainWindow::on_action_Analyze_triggered()
{
    QString file = QFileDialog::getOpenFileName(this);

    if(file.isEmpty())
        return;

    this->loadFile(file);
}

void MainWindow::on_action_About_PREF_triggered()
{
    AboutDialog dlgabout(this);
    dlgabout.exec();
}
