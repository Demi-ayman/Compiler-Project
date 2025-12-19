#include "graphviz_renderer.h"
#include <QProcess>
#include <QTextStream>

/*
 * DFS deterministic traversal:
 * - assigns IDs based ONLY on tree structure
 * - writes nodes and edges in the same order every time
 */
static void dfs(Node* node,
                QTextStream& out,
                QMap<Node*, int>& ids,
                int& nextId)
{
    // Assign stable ID
    int myId = nextId++;
    ids[node] = myId;

    // Decide shape
    QString shape = "oval";
    if (node->type == "if" || node->type == "repeat" ||
        node->type == "assign" || node->type == "read" ||
        node->type == "write" || node->type == "stmt-sequence")
    {
        shape = "box";
    }

    // Label
    QString label = QString::fromStdString(node->type);
    if (!node->value.empty()) {
        label += "\\n(" + QString::fromStdString(node->value) + ")";
    }

    // Emit node
    out << "n" << myId
        << " [shape=" << shape
        << ", label=\"" << label << "\"];\n";

    // Children
    for (Node* child : node->children) {
        dfs(child, out, ids, nextId);
        out << "n" << myId
            << " -> n" << ids[child] << ";\n";
    }
}

QString GraphvizRenderer::generateDot(Node* root, QMap<Node*, int>& map)
{
    map.clear();

    QString dot;
    QTextStream out(&dot);

    out << "digraph G {\n"
           "graph [rankdir=TB];\n"
           "node [fontname=\"Helvetica\", fontsize=12];\n";

    int nextId = 0;
    dfs(root, out, map, nextId);

    out << "}\n";
    return dot;
}

bool GraphvizRenderer::runDot(const QString& dotPath,
                              const QString& svgPath)
{
    QProcess p;
    p.start("dot", QStringList() << "-Tsvg" << dotPath << "-o" << svgPath);

    if (!p.waitForFinished(3000))
        return false;

    return p.exitCode() == 0;
}
