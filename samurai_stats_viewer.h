#ifndef NINJA_STATS_VIEWER_H
#define NINJA_STATS_VIEWER_H

#include <QObject>
#include <QTableWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QVBoxLayout>
#include <QGraphicsSimpleTextItem>
#include <QGraphicsLineItem>
#include <string>
#include <string.h>
#include <QString>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
class samurai_stats_viewer : public QObject
{
    Q_OBJECT
public:
    explicit samurai_stats_viewer(QObject *parent = nullptr);
    QTableWidget *Table;
    QGraphicsScene *scene;
    QVBoxLayout *stats;
    void setSamuraiStats(QString,QString,QString,QString,QString,QString,QString,QString,QString);
    void setTableWidget(QTableWidget* tableWidget,
                        QGraphicsScene *GraficScene,
                        QGraphicsView *Lienso,
                        QVBoxLayout *layout);



signals:

    void setSamuraiStatsSignal(const QString &stats);
private:
    void setStatsNames();
    void insertToTable(QString);
    int iter = 0;
    int anterior = 0;
    enum Columna
    {
        IDC,EdadC,ProbabilidadC,GeneracionesC,InteligenciaC,CondicionC,
        Tronc_SupC,Tronc_InfC,ResistenciaC
    };

};

#endif // NINJA_STATS_VIEWER_H
