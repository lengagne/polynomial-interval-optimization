#include <vector>
#include <QCoreApplication>
#include <QFileDialog>
#include <QDebug>

#include "data_format3D.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    QString repo = ".";
    if (argc == 2)
    {
        repo = argv[1];
    }           
    
	QDir directory(repo);

    QStringList filesList = directory.entryList(QDir::Files);
    QString fileName;

    std::vector<data_format*> datas_;
    
    init_order();
    
    for (QStringList::iterator it = filesList.begin();it != filesList.end(); ++it) 
    {
        QString current = *it;
        {
            data_format* d = new data_format3D(( repo  + current).toStdString());
            datas_.push_back(d);
        }
    }
    
    set_pourcentage( datas_);
    
//     create_csv(datas_, "nb_iter", "ndof", "solver", "problem", repo.toStdString() + "problem");
//     create_csv(datas_, "nb_iter", "ndof", "problem", "solver", repo.toStdString() + "solver");
// 
//     create_csv(datas_, "total_time", "ndof", "solver", "problem", repo.toStdString() + "problem");
//     create_csv(datas_, "total_time", "ndof", "problem", "solver", repo.toStdString() + "solver");
// 
//     // to get the ratio
//     create_csv(datas_, "nb_iter", "ndof", "solver", "problem", repo.toStdString() + "problem",true);
//     create_csv(datas_, "total_time", "ndof", "solver", "problem", repo.toStdString() + "problem",true);

    
    std::vector<std::string> order_latex;
    order_latex.push_back("precision");
    order_latex.push_back("solver");
    order_latex.push_back("prep_time");
    order_latex.push_back("comput_time");
    order_latex.push_back("total_time");
    order_latex.push_back("(D-H:M:S.ms)");
    order_latex.push_back("nb_iter");
    order_latex.push_back("time_per_iter");
    order_latex.push_back("criteria");

    order_latex.push_back("comput_time (%)");
    order_latex.push_back("total_time (%)");
    order_latex.push_back("nb_iter (%)");
//     order_latex.push_back("filename");
    
    std::vector<std::string> common;    
    common.push_back("problem");
    common.push_back("type");
    
    std::vector<std::string> remove;
    remove.push_back("Contraction");
    create_latex( datas_, "recap_bis_3D", order_latex,common,remove,"Bissection of problem 3D with ");
    
    remove.clear();
    remove.push_back("Bissection");
    create_latex( datas_, "recap_cont_3D", order_latex,common,remove,"Contraction of problem 3D with ");    
    
}
