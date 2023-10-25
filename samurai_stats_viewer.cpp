#include "ninja_stats_viewer.h"

ninja_stats_viewer::ninja_stats_viewer(QObject *parent) : QObject(parent)
{

}


void ninja_stats_viewer::setTableWidget(QTableWidget* tableWidget){

    this->Table = tableWidget;
    this->Table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->setStatsNames();

}


void ninja_stats_viewer::setNinjaStats(QString ID, QString Edad, QString Probabilidad,
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

    /*
    this->Table->setItem(row,IDC,new QTableWidgetItem(ID));
    this->Table->setItem(row,EdadC,new QTableWidgetItem(Edad));
    this->Table->setItem(row,ProbabilidadC,new QTableWidgetItem(Probabilidad));
    this->Table->setItem(row,GeneracionesC,new QTableWidgetItem(Generaciones));
    this->Table->setItem(row,InteligenciaC,new QTableWidgetItem(Inteligencia));
    this->Table->setItem(row,CondicionC,new QTableWidgetItem(Condicion));
    this->Table->setItem(row,Tronc_SupC,new QTableWidgetItem(Tronc_Sup));
    this->Table->setItem(row,Tronc_InfC,new QTableWidgetItem(Tronc_Inf));
    this->Table->setItem(row,ResistenciaC,new QTableWidgetItem(Resistencia));


*/

}

void ninja_stats_viewer::insertToTable(QString elem){


}


void ninja_stats_viewer::setStatsNames(){

    QStringList atributos;
    atributos<<"ID"<<"Edad"<<"Probabilidad"
            <<"Generaciones"<<"Inteligencia"<<"Condicion"
           <<"Tronc Sup"<<"Tronc Inf"<<"Resistencia";

    this->Table->setColumnCount(9);
    this->Table->setHorizontalHeaderLabels(atributos);



}
