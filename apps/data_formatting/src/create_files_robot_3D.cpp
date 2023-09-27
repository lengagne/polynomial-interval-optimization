#include <QFileDialog>
#include <QDebug>
#include <QApplication>
#include <iostream>

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <sstream>

bool is_double(const std::string& s)
{
    std::istringstream iss(s);
    double d;
    return iss >> d >> std::ws && iss.eof();
}

struct data
{
    double precision;
    unsigned int pb;
    unsigned int solver;

    unsigned int nb_iter;
    double computation_time;
    double total_time;
    double preparation_time=-1.0;
    double time_per_iter;
    std::string filename;
    double crit;
    unsigned int status = 0;
};

std::vector<unsigned int> solveur = {1,2,3,4,21,22,23,24};
std::vector<unsigned int> pb = {10,11,12,13,14,15,16,17,20,21,22,23,24,25,26,27};
std::vector<double> precision = {0.01,0.001,0.0001};
unsigned int nb_pb;
std::vector<data> all_datas;

double toDouble(std::string s){
    std::replace(s.begin(), s.end(), '.', ',');
    return std::atof(s.c_str());
}

bool find_data(unsigned int solver_type,
               unsigned int problem_type,
               double precision,
               data& out)
{
	bool val = false;
    for (int i=0;i<all_datas.size();i++)
    {
        const data& d = all_datas[i];
        if(d.precision == precision && d.solver == solver_type && d.pb == problem_type)
        {
            out = d;
            val = true;
        }
    }
    return val;
}

// return false if data is no suitable
void read_data( const std::string &filename, data & d);

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
	QDir directory("./");

    QStringList filesList = directory.entryList(QDir::Files);
    QString fileName;

    for (QStringList::iterator it = filesList.begin();it != filesList.end(); ++it) {
        QString current = *it;
        data tmp;
        read_data(current.toStdString(),tmp);
        //if (current == "slurm-3618287.out")
        {
            tmp.filename = current.toStdString();
            std::cout<<"File : "<< current.toStdString()<<std::endl;
            std::cout<<"status = "<< tmp.status<<std::endl;
            std::cout<<"precision = "<< tmp.precision<<std::endl;
            std::cout<<"pb = "<< tmp.pb<<std::endl;
            std::cout<<"solver = "<< tmp.solver<<std::endl;
            std::cout<<"nb_iter = "<< tmp.nb_iter<<std::endl;
            std::cout<<"preparation_time = "<< tmp.preparation_time<<std::endl;
            std::cout<<"computation_time = "<< tmp.computation_time<<std::endl;
            std::cout<<"time_per_iter = "<< tmp.time_per_iter<<std::endl;
            std::cout<<"total_time = "<< tmp.total_time<<std::endl;
            std::cout<<"crit = "<< tmp.crit<<std::endl<<std::endl<<std::endl;
            all_datas.push_back(tmp);
        }
//        else
//        {
//            std::cout<<"File : "<< current.toStdString()<< " does not content computation info"<<std::endl;
//        }
    }

    std::ofstream outfile ("result_3D.csv");
//    // create the files
    for (int i=0;i<precision.size();i++)   // 1 to nb-solver
    {
        outfile <<std::endl;
        outfile<<"precision \t"<< precision[i]<<std::endl;
        outfile <<"solveur / problem \t";
        for (int j=0;j<pb.size();j++)   outfile << pb[j] <<"\t";
        outfile<<std::endl;
        for (int k=0;k<solveur.size();k++)
        {
            outfile<< solveur[k]<<"\t";
            for (int j=0;j<pb.size();j++)
            {
                data d;
                if( find_data(solveur[k],pb[j],precision[i],d))
                {
                    if(d.status == 0)
                    {
                        outfile<< d.nb_iter<<" / "<< d.computation_time<<"( + "<< d.preparation_time<<") = "<< d.total_time;
                        if(solveur[k]>=20)
                            outfile<<"## "<< d.crit;
                        outfile<<"\t";
                    }
                    else if(d.status == 1)
                        outfile<<"TIMEOUT\t";
                    else if(d.status == 2)
                        outfile<<"CANCELLED\t";
                    else if(d.status == 3)
                        outfile<<"MEMORY LIMIT\t";
                    else if(d.status == 4)
                        outfile<<"RUNNING\t";
                    else if(d.status == 5)
                        outfile<<"FAIL\t";
                    else if(d.status == 6)
                        outfile<<"SEG FAULT\t";
                }else
                {
                    outfile<<"X / X \t";
                }
            }
            outfile<<std::endl;
        }
    }
	outfile.close();

	std::ofstream filefile ("filenames_3D.csv");
//    // create the files
    for (int i=0;i<precision.size();i++)   // 1 to nb-solver
    {
        filefile <<std::endl;
        filefile<<"precision \t"<< precision[i]<<std::endl;

        filefile <<"solveur \ problem \t";
        for (int j=0;j<pb.size();j++)   filefile << pb[j] <<"\t";
        filefile<<std::endl;
        for (int k=0;k<solveur.size();k++)
        {
            filefile<< solveur[k]<<"\t";
            for (int j=0;j<pb.size();j++)
            {
                data d;
                if( find_data(solveur[k],pb[j],precision[i],d))
                {
                        if(d.status == 1)
                        filefile<<"TIMEOUT  ";
                        else if(d.status == 2)
                        filefile<<"CANCELLED  ";
                        else if(d.status == 3)
                        filefile<<"MEMORY LIMIT  ";
                        else if(d.status == 4)
                        filefile<<"RUNNING  ";
                        else if(d.status == 5)
                        filefile<<"FAIL  ";
                        else if(d.status == 6)
                        filefile<<"SEG FAULT";
                        filefile<< d.filename<<"\t";
                }else
                {
                    filefile<<"X / X \t";
                }
            }
            filefile<<std::endl;
        }
    }
	filefile.close();

    {
        std::ofstream filefile1 ("draw_data_iteration_3D.csv");
        for (int i=0;i<precision.size();i++)   // 1 to nb-solver
        {
            filefile1 <<std::endl;
            filefile1<<"precision \t"<< precision[i]<<std::endl;

            filefile1 <<"problem \t";
            for (int j=0;j<pb.size();j++)
                filefile1<< pb[j]<<"\t";
            filefile1<<std::endl;

            for (int k=0;k<solveur.size();k++)
            {
                filefile1<< solveur[k]<<"\t";
                for (int j=0;j<pb.size();j++)
                {
                    data d;
                    if( find_data(solveur[k],pb[j],precision[i],d))
                    {
                        if(d.status == 0)
                        filefile1<< d.nb_iter<<"\t";
                        else if(d.status == 1)
                        filefile1<<"TIMEOUT\t";
                        else if(d.status == 2)
                        filefile1<<"CANCELLED\t";
                        else if(d.status == 3)
                        filefile1<<"MEMORY LIMIT\t";
                        else if(d.status == 4)
                        filefile1<<"RUNNING\t";
                        else if(d.status == 5)
                        filefile1<<"FAIL\t";
                        else if(d.status == 6)
                        filefile<<"SEG FAULT\t";
                    }else
                    {
                        filefile1<<"X / X \t";
                    }
                }
                filefile1<<std::endl;
            }
            filefile1<<std::endl;
            filefile1<<std::endl;
        }
        filefile1.close();
    }


    {
        std::ofstream filefile1 ("draw_data_time_3D.csv");
        for (int i=0;i<precision.size();i++)   // 1 to nb-solver
        {
            filefile1 <<std::endl;
            filefile1<<"precision \t"<< precision[i]<<std::endl;

            filefile1 <<"problem \t";
            for (int j=0;j<pb.size();j++)
                filefile1<< pb[j]<<"\t";
            filefile1<<std::endl;

            for (int k=0;k<solveur.size();k++)
            {
                filefile1<< solveur[k]<<"\t";
                for (int j=0;j<pb.size();j++)
                {
                    data d;
                    if( find_data(solveur[k],pb[j],precision[i],d))
                    {
                        if(d.status == 0)
                        filefile1<< d.computation_time<<"\t";
                        else if(d.status == 1)
                        filefile1<<"TIMEOUT\t";
                        else if(d.status == 2)
                        filefile1<<"CANCELLED\t";
                        else if(d.status == 3)
                        filefile1<<"MEMORY LIMIT\t";
                        else if(d.status == 4)
                        filefile1<<"RUNNING\t";
                        else if(d.status == 5)
                        filefile1<<"FAIL\t";
                        else if(d.status == 6)
                        outfile<<"SEG FAULT\t";
                    }else
                    {
                        filefile1<<"X / X \t";
                    }
                }
                filefile1<<std::endl;
            }
            filefile1<<std::endl;
            filefile1<<std::endl;
        }

        filefile1.close();
    }

}

void read_data( const std::string &filename, data & d)
{
//    std::cout<<"Reading data of file "<< filename <<std::endl;
    d.precision = 42;
	d.pb = 42;
	d.solver = 42;
	d.nb_iter = 42424242;
	d.computation_time = 42424242;
	d.time_per_iter = 42;
	d.total_time = 42424242;
	d.crit = -1.0;

        char str [800];
	float f;
	FILE * pFile;

	pFile = fopen (filename.c_str(),"r");
	fscanf (pFile, "precision = %s\n", str);
	d.precision = toDouble(std::string(str));
	fscanf (pFile, "solveur = %d\n", &d.solver);
	fscanf (pFile, "model = %d\n", &d.pb);
	//  fscanf (pFile,"X = %s\n",str);    std::cout<<"str = "<< str <<std::endl;
	//  fscanf (pFile,"Y = %s\n",str);    std::cout<<"str = "<< str <<std::endl;
	for (int i=0;i<14;i++)
        fgets(str,800,pFile);   // Problem3D_with_torque_limit

	fscanf (pFile, "preparation time : %s seconds.\n", str);
	d.preparation_time = toDouble(std::string(str));
//	str = "01234567891011121314";
	fscanf (pFile, "Number of Bissections : %d\n", &d.nb_iter);
	fgets(str,800,pFile);  // Number of valid boxes : 0
	fgets(str,800,pFile);  // Number of possible boxes : 12
    fscanf (pFile, "computation time (wo prep): %s seconds.\n", str);
	d.computation_time = toDouble(std::string(str));
//	str = "01234567891011121314";
    fscanf (pFile, "Time per iteration : %s seconds.\n",str);
    d.time_per_iter = toDouble(std::string(str));
//    str = "01234567891011121314";
    fscanf (pFile, "total time : %s seconds.\n",str);
    d.total_time = toDouble(std::string(str));
    //fgets(str,800,pFile); // empty line
    fscanf (pFile, "\ncrit = %s\n",str);
    //std::cout<<"crit (wo prep): str  = "<< str << " en double ca fait "<< toDouble(std::string(str)) <<std::endl;
    if(is_double(str))
        d.crit = toDouble(std::string(str));
    else
        d.crit = -1.0;
//    str = "01234567891011121314";
	fclose (pFile);

    d.status = 0;
   std::string line ;
    {
        bool test = true;
        std::ifstream  myfile(filename);
        while( std::getline( myfile, line ))
        {
            std::size_t it =  line.find("total time ") ;
            if(it!= std::string::npos) //HERE!
            {
                test = false;
            }
        }
        if (test)
        {
             d.computation_time = 1e9;
             d.total_time = 1e9;
             d.time_per_iter = 1e9;
             d.status = 4;
        }

    }
    {
        std::ifstream  myfile(filename);
        while( std::getline( myfile, line ))
        {
            std::size_t it =  line.find("autocode") ;
            if(it!= std::string::npos) //HERE!
            {
                 d.computation_time = 1e9;
                 d.total_time = 1e9;
                 d.time_per_iter = 1e9;
                 d.status = 5;
                 return;
            }
        }
    }
    {
        std::ifstream  myfile(filename);
        while( std::getline( myfile, line ))
        {
            std::size_t it =  line.find("DUE TO TIME LIMIT") ;
            if(it!= std::string::npos) //HERE!
            {
                 d.computation_time = 1e9;
                 d.total_time = 1e9;
                 d.time_per_iter = 1e9;
                 d.status = 1;
                 return;
            }
        }
    }
    {
        std::ifstream  myfile(filename);
        while( std::getline( myfile, line ))
        {
            std::size_t it =  line.find("Exceeded job memory limit") ;
            if(it!= std::string::npos) //HERE!
            {
                 d.computation_time = 1e9;
                 d.total_time = 1e9;
                 d.time_per_iter = 1e9;
                 d.status = 3;
                 return;
            }
        }
    }
    {
        std::ifstream  myfile(filename);
        while( std::getline( myfile, line ))
        {
            std::size_t it =  line.find("CANCELLED") ;
            if(it!= std::string::npos) //HERE!
            {
                 d.computation_time = 1e9;
                 d.total_time = 1e9;
                 d.time_per_iter = 1e9;
                 d.status = 2;
                 return;
            }
        }
    }
    {
        std::ifstream  myfile(filename);
        while( std::getline( myfile, line ))
        {
            std::size_t it =  line.find("Erreur de segmentation") ;
            if(it!= std::string::npos) //HERE!
            {
                 d.computation_time = 1e9;
                 d.total_time = 1e9;
                 d.time_per_iter = 1e9;
                 d.status = 6;
                 return;
            }
        }
    }
	return ;
}
