#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QGraphicsScene>
#include <QtSvgWidgets/QGraphicsSvgItem>
#include <QSvgRenderer>
#include <QMessageBox>
#include <QTemporaryFile>
#include <QProcess>
#include <QPdfWriter>
#include <QPainter>
#include <QPageSize>
#include <QDir>
#include <QStandardPaths>

#include "graphviz_renderer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , root(nullptr)
{
    ui->setupUi(this);

    scene = new QGraphicsScene(this);
    ui->svgView->setScene(scene);

    // Improve rendering interaction
    ui->svgView->setRenderHint(QPainter::Antialiasing);
    ui->svgView->setRenderHint(QPainter::SmoothPixmapTransform);
    ui->svgView->setDragMode(QGraphicsView::ScrollHandDrag);
    ui->svgView->setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    ui->svgView->setResizeAnchor(QGraphicsView::AnchorUnderMouse);

    connect(ui->btnLoad, &QPushButton::clicked, this, &MainWindow::loadFile);
    connect(ui->btnParse, &QPushButton::clicked, this, &MainWindow::parseCode);
    connect(ui->btnPNG, &QPushButton::clicked, this, &MainWindow::exportPNG);
    connect(ui->btnPDF, &QPushButton::clicked, this, &MainWindow::exportPDF);
}

MainWindow::~MainWindow()
{
    clearTree();
    delete ui;
}

void MainWindow::clearTree()
{
    if (root) {
        freeTree(root);
        root = nullptr;
    }
}

void MainWindow::loadFile()
{
    QString path = QFileDialog::getOpenFileName(this, "Load Scanner Output", "", "*.txt");
    if (path.isEmpty()) return;

    QFile f(path);
    if (!f.open(QFile::ReadOnly)) {
        QMessageBox::warning(this, "Error", "Could not open file.");
        return;
    }

    ui->textTokens->setPlainText(f.readAll());
    ui->labelStatus->setText("Loaded: " + path);
}

void MainWindow::parseCode()
{
    clearTree();

    QString tokensText = ui->textTokens->toPlainText().trimmed();
    if (tokensText.isEmpty()) {
        QMessageBox::warning(this, "No tokens", "Token area is empty. Load or paste token text.");
        return;
    }

    // Save tokens to temp file
    QString tmp = QDir::temp().filePath("tokens_temp.txt");
    QFile f(tmp);
    if (!f.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, "Error", "Failed to create temporary token file.");
        return;
    }
    f.write(tokensText.toUtf8());
    f.close();

    try {
        loadTokensFromFile(tmp.toStdString());
        currentIndex = 0;
        root = parseProgram();
        buildDOTandSVG();
    }
    catch (std::exception &ex) {
        QMessageBox::critical(this, "Parser Error", ex.what());
    }
}

void MainWindow::buildDOTandSVG()
{
    if (!root) {
        QMessageBox::warning(this, "No AST", "Parser did not return a valid AST.");
        return;
    }

    QMap<Node*, int> idMap;
    QString dot = GraphvizRenderer::generateDot(root, idMap);

    QString dotPath = QDir::temp().filePath("tree.dot");
    QString svgPath = QDir::temp().filePath("tree.svg");

    // Write .dot file
    QFile f(dotPath);
    if (!f.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, "Error", "Failed to write DOT file.");
        return;
    }
    f.write(dot.toUtf8());
    f.close();

    // Try GraphvizRenderer method
    bool ok = GraphvizRenderer::runDot(dotPath, svgPath);

    // Fallback if needed
    if (!ok) {
        QProcess proc;
        proc.start("dot", QStringList() << "-Tsvg" << "-o" << svgPath << dotPath);
        proc.waitForFinished(8000);

        if (proc.exitCode() != 0) {
            QString err = proc.readAllStandardError();
            if (err.isEmpty()) err = "GraphViz 'dot' execution failed. Ensure Graphviz is installed.";
            QMessageBox::critical(this, "GraphViz Error", err);
            return;
        }
    }

    renderSVG(svgPath);
    ui->labelStatus->setText("Rendered Successfully");
}

void MainWindow::renderSVG(const QString &svgPath)
{
    QFileInfo info(svgPath);
    if (!info.exists()) {
        QMessageBox::warning(this, "Error", "SVG file not found.");
        return;
    }

    scene->clear();
    auto *svgItem = new QGraphicsSvgItem(svgPath);

    svgItem->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    scene->addItem(svgItem);

    scene->setSceneRect(svgItem->boundingRect());
    ui->svgView->fitInView(svgItem->boundingRect(), Qt::KeepAspectRatio);
}

void MainWindow::exportPNG()
{
    QString path = QFileDialog::getSaveFileName(this, "Save PNG", "", "*.png");
    if (path.isEmpty()) return;

    QRectF bounds = scene->itemsBoundingRect();
    if (bounds.isEmpty()) {
        QMessageBox::warning(this, "Error", "Nothing to export.");
        return;
    }

    const int margin = 20;
    QSize size(bounds.width() + margin * 2, bounds.height() + margin * 2);

    QImage img(size, QImage::Format_ARGB32);
    img.fill(Qt::white);

    QPainter p(&img);
    p.setRenderHint(QPainter::Antialiasing);
    p.translate(-bounds.topLeft() + QPointF(margin, margin));

    scene->render(&p);
    p.end();

    img.save(path);
    ui->labelStatus->setText("Saved PNG: " + path);
}

void MainWindow::exportPDF()
{
    QString path = QFileDialog::getSaveFileName(this, "Save PDF", "", "*.pdf");
    if (path.isEmpty()) return;

    QRectF bounds = scene->itemsBoundingRect();
    if (bounds.isEmpty()) {
        QMessageBox::warning(this, "Error", "Nothing to export.");
        return;
    }

    QPdfWriter writer(path);
    writer.setResolution(300); // High-quality print

    // Convert pixels ➜ mm (1 mm ≈ 3.78 px)
    QSizeF mm(bounds.width() / 3.78, bounds.height() / 3.78);

    // Correct Qt-compatible page size API
    writer.setPageSize(QPageSize(mm, QPageSize::Millimeter));

    QPainter p(&writer);
    p.setRenderHint(QPainter::Antialiasing);

    p.translate(-bounds.topLeft());
    scene->render(&p);

    p.end();
    ui->labelStatus->setText("Saved PDF: " + path);
}
