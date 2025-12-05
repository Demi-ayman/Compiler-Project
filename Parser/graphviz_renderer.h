#ifndef GRAPHVIZ_RENDERER_H
#define GRAPHVIZ_RENDERER_H

#include <QString>
#include <QMap>
#include "parser.h"

class GraphvizRenderer {
public:
    static QString generateDot(Node *root, QMap<Node*,int> &map);
    static bool runDot(const QString &dotPath, const QString &svgPath);
};

#endif
