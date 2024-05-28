#include <vector>
#include <QCoreApplication>
#include <QFileDialog>
#include <QDebug>

#include "data_format.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//     std::string info_value = argv[2];    
    QString repo = argv[3];
	QDir directory(repo);

    QStringList filesList = directory.entryList(QStringList() << "slurm*.out", QDir::Files); //directory.entryList(QDir::Files);    
    filesList = filesList.filter(".out");
    
    QString fileName;
    
    std::cout<<"Select files : print the list of all files "<<std::endl;
    if (argc != 4)
    {
        std::cout<<"Please select info_name info_value repo"<<std::endl;
        return 1;
    }

    std::vector<data_format*> datas_;
    
    init_order();
    
    for (QStringList::iterator it = filesList.begin();it != filesList.end(); ++it) 
    {
        QString current = *it;
        {
            data_format* d = new data_format(( repo  + "/"+ current).toStdString());
            datas_.push_back(d);
        }
    }
    
    std::string info_name = argv[1];
    std::string info_value = argv[2];

    std::cout<<"info_name =" << info_name <<std::endl;
    std::cout<<"info_value =" << info_value <<std::endl;
    
    for ( auto& d : datas_)
    {
        if (d->infos[info_name] == info_value)
        {
            std::cout<< d->infos["filename"] <<" ";
        }
    }
    
}
