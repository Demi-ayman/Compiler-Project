#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include "parser.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class QGraphicsScene;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void loadFile();
    void parseCode();
    void exportPNG();
    void exportPDF();

private:
    Ui::MainWindow *ui;
    QGraphicsScene *scene;

    Node *root;
    QMap<Node*,int> idMap;

    void renderSVG(const QString &svgPath);
    void buildDOTandSVG();
    void clearTree();
};

#endif // MAINWINDOW_H
