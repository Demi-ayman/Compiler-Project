#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsSvgItem>
#include <QSvgRenderer>
#include <QMessageBox>
#include <QTemporaryFile>
#include <QProcess>
#include <QPdfWriter>
#include <QPainter>
#include <QPageSize>

#include "graphviz_renderer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , root(nullptr)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->svgView->setScene(scene);

    connect(ui->btnLoad, &QPushButton::clicked, this, &MainWindow::loadFile);
    connect(ui->btnParse, &QPushButton::clicked, this, &MainWindow::parseCode);
    connect(ui->btnPNG, &QPushButton::clicked, this, &MainWindow::exportPNG);
    connect(ui->btnPDF, &QPushButton::clicked, this, &MainWindow::exportPDF);
}

MainWindow::~MainWindow() { clearTree(); delete ui; }

void MainWindow::clearTree() {
    if (root) { freeTree(root); root = nullptr; }
}

void MainWindow::loadFile() {
    QString path = QFileDialog::getOpenFileName(this,"Load Scanner Output","","*.txt");
    if (path.isEmpty()) return;

    QFile f(path);
    if (!f.open(QFile::ReadOnly)) {
        QMessageBox::warning(this,"Error","Could not open file.");
        return;
    }


    ui->textTokens->setPlainText(f.readAll());
    ui->labelStatus->setText("Loaded: " + path);
}

void MainWindow::parseCode() {
    clearTree();

    QString tmp = QDir::temp().filePath("tokens_temp.txt");
    QFile f(tmp);
    if (!f.open(QFile::WriteOnly)) {
        QMessageBox::warning(this,"Error","Failed to open temporary file.");
        return;
    }

    f.write(ui->textTokens->toPlainText().toUtf8());
    f.close();

    try {
        loadTokensFromFile(tmp.toStdString());
        currentIndex = 0;
        root = parseProgram();
        buildDOTandSVG();
    }
    catch (std::exception &ex) {
        QMessageBox::critical(this,"Error",ex.what());
    }
}

void MainWindow::buildDOTandSVG() {
    QMap<Node*,int> idMap;

    QString dot = GraphvizRenderer::generateDot(root,idMap);

    QString dotPath = QDir::temp().filePath("tree.dot");
    QString svgPath = QDir::temp().filePath("tree.svg");

    QFile f(dotPath);
    if (!f.open(QFile::WriteOnly)) {
        QMessageBox::warning(this,"Error","Failed to open temporary file.");
        return;
    }

    f.write(dot.toUtf8());
    f.close();

    if (!GraphvizRenderer::runDot(dotPath,svgPath)) {
        QMessageBox::warning(this,"Error","GraphViz failed. Ensure 'dot' is installed.");
        return;
    }

    renderSVG(svgPath);
    ui->labelStatus->setText("Rendered Successfully");
}

void MainWindow::renderSVG(const QString &svgPath) {
    scene->clear();
    QGraphicsSvgItem *svgItem = new QGraphicsSvgItem(svgPath);
    scene->addItem(svgItem);

    scene->setSceneRect(svgItem->boundingRect());
    ui->svgView->fitInView(svgItem->boundingRect(), Qt::KeepAspectRatio);
}

void MainWindow::exportPNG() {
    QString path = QFileDialog::getSaveFileName(this,"Save PNG","","*.png");
    if (path.isEmpty()) return;

    QRectF r = scene->itemsBoundingRect();
    QImage img(r.size().toSize(), QImage::Format_ARGB32);
    img.fill(Qt::white);

    QPainter p(&img);
    scene->render(&p, QRectF(), r);
    p.end();

    img.save(path);
}

void MainWindow::exportPDF() {
    QString path = QFileDialog::getSaveFileName(this, "Save PDF", "", "*.pdf");
    if (path.isEmpty()) return;

    QRectF r = scene->itemsBoundingRect();

    // PDF writer
    QPdfWriter writer(path);
    writer.setResolution(300);  // High quality print

    // Convert scene rectangle to millimeters
    // PDF uses *points*, so we configure page size in points.
    QSizeF mm(r.width() / 3.78, r.height() / 3.78);
    // (1 mm ≈ 3.78 px)

    writer.setPageSize(QPageSize(mm, QPageSize::Millimeter));

    QPainter p(&writer);
    scene->render(&p, QRectF(), r);
    p.end();
}
