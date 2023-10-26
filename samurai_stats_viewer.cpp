#include "samurai_stats_viewer.h"

samurai_stats_viewer::samurai_stats_viewer(QObject *parent) : QObject(parent)
{

}


void samurai_stats_viewer::setTableWidget(QTableWidget* tableWidget){

    this->Table = tableWidget;
    this->Table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setStatsNames();

}


void samurai_stats_viewer::setSamuraiStats(QString ID, QString Edad, QString Probabilidad,
                                       QString Generaciones, QString Inteligencia,
                                       QString Condicion, QString Tronc_Sup,
                                       QString Tronc_Inf, QString Resistencia)

{

    QStringList atributos;
    atributos<<ID<<Edad<<Probabilidad
            <<Generaciones<<Inteligencia<<Condicion
           <<Tronc_Sup<<Tronc_Inf<<Resistencia;

    this->Table->insertRow(Table->rowCount());
    int row = Table->rowCount()-1;
    for(int i = 0 ; i<atributos.size(); i++){
        this->Table->setItem(row,i,new QTableWidgetItem(atributos.at(i)));

    }


}

void samurai_stats_viewer::insertToTable(QString elem){


}


void samurai_stats_viewer::setStatsNames(){

    QStringList atributos;
    atributos<<"ID"<<"Edad"<<"Probabilidad"
            <<"Generaciones"<<"Inteligencia"<<"Condicion"
           <<"Tronc Sup"<<"Tronc Inf"<<"Resistencia";

    this->Table->setColumnCount(9);
    this->Table->setHorizontalHeaderLabels(atributos);



}
