#ifndef NINJA_STATS_VIEWER_H
#define NINJA_STATS_VIEWER_H

#include <QObject>
#include <QTableWidget>
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
    void setSamuraiStats(QString,QString,QString,QString,QString,QString,QString,QString,QString);
    void setTableWidget(QTableWidget*);


signals:

    void setSamuraiStatsSignal(const QString &stats);
private:
    void setStatsNames();
    void insertToTable(QString);
    enum Columna
    {
        IDC,EdadC,ProbabilidadC,GeneracionesC,InteligenciaC,CondicionC,
        Tronc_SupC,Tronc_InfC,ResistenciaC
    };

};

#endif // NINJA_STATS_VIEWER_H
