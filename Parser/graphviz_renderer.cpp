#include "graphviz_renderer.h"
#include <QProcess>
#include <QTextStream>

static void dfs(Node *n, QMap<Node*,int> &map, int &id)
{
    map[n] = id++;
    for (auto c : n->children)
        dfs(c,map,id);
}

QString GraphvizRenderer::generateDot(Node *root, QMap<Node*,int> &map)
{
    map.clear();
    int id = 0;
    dfs(root,map,id);

    QString dot;
    QTextStream out(&dot);

    out << "digraph G {\n"
           "graph [rankdir=TB];\n"
           "node [fontname=\"Helvetica\", fontsize=12];\n";

    // nodes
    for (auto it = map.begin(); it != map.end(); ++it) {
        Node *n = it.key();
        int i = it.value();
        QString shape = "oval";

        // statements = rectangles
        if (n->type == "if" || n->type == "repeat" || n->type == "assign" ||
            n->type == "read" || n->type == "write" || n->type == "stmt-sequence")
            shape = "box";

        QString label = QString::fromStdString(n->type);
        if (!n->value.empty())
            label += "\\n(" + QString::fromStdString(n->value) + ")";

        out << "n" << i << " [shape=" << shape << ", label=\"" << label << "\"];\n";
    }

    // edges
    for (auto it = map.begin(); it != map.end(); ++it)
    {
        Node *n = it.key();
        int i = it.value();
        for (auto c : n->children)
            out << "n" << i << " -> n" << map[c] << ";\n";
    }

    out << "}\n";
    return dot;
}

bool GraphvizRenderer::runDot(const QString &dotPath, const QString &svgPath)
{
    QProcess p;
    p.start("dot", QStringList() << "-Tsvg" << dotPath << "-o" << svgPath);
    if (!p.waitForFinished(3000)) return false;
    return p.exitCode() == 0;
}
