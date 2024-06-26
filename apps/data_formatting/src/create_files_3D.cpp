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
    std::string type = "Bissection";
    if (argc>=3)
    {
        type = argv[2];        
    }   
    std::cout<<"type = "<< type <<std::endl;
    
	QDir directory(repo);

    QStringList filesList = directory.entryList(QStringList() << "slurm*.out", QDir::Files); // directory.entryList(QDir::Files);
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


    std::vector<std::string> common;    
    common.push_back("problem");
    common.push_back("type");
    
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
    
    std::vector<std::string> average_on;
    average_on.push_back("robot");
    average_on.push_back("precision");    
    average_on.push_back("solver");    
    
    std::vector<std::string> order_latex_average;        
    order_latex_average.push_back("prep_time");
    order_latex_average.push_back("comput_time");
    order_latex_average.push_back("total_time");
    order_latex_average.push_back("(D-H:M:S.ms)");
    order_latex_average.push_back("nb_iter");
    order_latex_average.push_back("time_per_iter");
    order_latex_average.push_back("comput_time (%)");
    order_latex_average.push_back("total_time (%)");    
    order_latex_average.push_back("nb_iter (%)");
//     order_latex_average.push_back("filename");
//     order_latex_average.push_back("save_filename"); 
    
    
    std::string filename = "recap_bis_3D";
    std::string title_frame = "Bissection of problem 3D with ";
    std::string average_name = "Average of Bissection for 3D problems";
    std::string label = "pb3d";
    if (type == "Contraction")
    {
        filename = "recap_cont_3D";
        title_frame = "Contraction of problem 3D with ";
        average_name = "Average of Contraction for 3D problems";
        label = "pc3d";        
    }    
    
    
    
    std::vector<std::string> remove;
//     remove.push_back("Contraction");
    create_latex( datas_, filename, order_latex,order_latex_average,common,average_on,remove,title_frame,average_name,label);
// //     create_latex( datas_, "recap_bis_3D", order_latex,order_latex_average,common,average_on,remove,"Bissection of problem 3D with ", "Average for 3D Problems","pb3d");
    
//     remove.clear();
//     remove.push_back("Bissection");
//     create_latex( datas_, "recap_cont_3D", order_latex,common,remove,"Contraction of problem 3D with ");    
    
}
