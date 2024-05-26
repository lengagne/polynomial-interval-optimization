#include <vector>
#include <QCoreApplication>
#include <QFileDialog>
#include <QDebug>

#include "data_format.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    QString repo = ".";
    if (argc >= 2)
    {
        repo = argv[1];
    }           
    
	QDir directory(repo);

    QStringList filesList = directory.entryList(QStringList() << "slurm*.out", QDir::Files); //directory.entryList(QDir::Files);    
    filesList = filesList.filter(".out");
    
    QString fileName;

    std::vector<data_format*> datas_;
    
    init_order();
    
    for (QStringList::iterator it = filesList.begin();it != filesList.end(); ++it) 
    {
        QString current = *it;
        {
            data_format* d = new data_format(( repo  + current).toStdString());
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

    

    std::vector<std::string> common;
    common.push_back("problem");
    common.push_back("type");

    std::vector<std::string> order_latex;
    order_latex.push_back("precision");
    order_latex.push_back("ndof");
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
//     order_latex.push_back("nb_intermediate");
//     order_latex.push_back("filename");
//     order_latex.push_back("save_filename");   
    

    std::vector<std::string> average_on;
    average_on.push_back("precision");
    average_on.push_back("ndof");
    average_on.push_back("solver");
    

    
    std::vector<std::string> order_latex_average;
//     order_latex_average.push_back("precision");
//     order_latex_average.push_back("ndof");
//     order_latex_average.push_back("solver");    
    order_latex_average.push_back("prep_time");
    order_latex_average.push_back("comput_time");
    order_latex_average.push_back("total_time");
    order_latex_average.push_back("(D-H:M:S.ms)");
    order_latex_average.push_back("nb_iter");
    order_latex_average.push_back("comput_time (%)");
    order_latex_average.push_back("total_time (%)");    
    order_latex_average.push_back("nb_iter (%)");
//     order_latex_average.push_back("filename");
//     order_latex_average.push_back("save_filename");       
    


    
    std::vector<std::string> remove;
    remove.push_back("Contraction");
    create_latex( datas_, "recap_bis_2D", order_latex,order_latex_average,common,average_on,remove,"Bissection of problem 2D with ","Average of 2D problems","pb2d");
    
//     remove.clear();
//     remove.push_back("Bissection");
//     create_latex( datas_, "recap_cont_2D", order_latex,order_latex_average,common,remove,"Contraction of problem 2D with ");
    
}
