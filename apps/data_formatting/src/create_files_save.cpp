#include <QCoreApplication>
#include <QFileDialog>

#include <iostream>
#include <vector>
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
    unsigned int nb_dof;
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

    unsigned status;
};

std::vector<unsigned int> solveur = {1,2,3,4,21,22,23,24};
std::vector<unsigned int> pb = {1,2,3,4,5,6,7,8,9,11,12,13,14,15,16,17,18,19};
std::vector<unsigned int> dof = {2,3,4,5,6,7,8,9,10};
std::vector<double> precision = {0.01,0.001};

unsigned int desired_precision = 0.01;
unsigned int nb_pb;

std::vector<data> all_datas;






bool find_data(unsigned int solver_type,
               unsigned int problem_type,
               unsigned int dof,
               double precision,
               data& out)
{
	bool val = false;
    for (int i=0;i<all_datas.size();i++)
    {
        const data& d = all_datas[i];
        if(d.nb_dof == dof && d.precision == precision && d.solver == solver_type && d.pb == problem_type)
        {
            out = d;
            val = true;
        }
    }
    return val;
}

void create_normalize_file( const std::string filename,
                            unsigned int solveur_current,
                            unsigned int solveur_ref,
                            unsigned id_precision)
{
    std::string temps = filename+"_temps.txt";
    std::ofstream outtemps (temps);
    std::string temps_total = filename+"_temps_total.txt";
    std::ofstream outtemps_total (temps_total);
    std::string iter = filename+"_iter.txt";
    std::ofstream outiter (iter);

    for (int k=0;k<dof.size();k++)
    {
        outtemps<< dof[k]<<"\t";
        outtemps_total<< dof[k]<<"\t";
        outiter<< dof[k]<<"\t";
        for (int ii=0;ii<pb.size();ii++)
        {
                data d_ref, d;
                if( find_data(solveur_ref,pb[ii],dof[k],precision[id_precision],d_ref) && find_data(solveur_current,pb[ii],dof[k],precision[id_precision],d))
                {
                    if(d_ref.status == 0 && d.status == 0)
                    {
                         outtemps<< 100.*d.computation_time /d_ref.computation_time<<"\t";
                         outtemps_total<< 100.*d.total_time /d_ref.total_time<<"\t";
                         outiter<< 100.*d.nb_iter /d_ref.nb_iter<<"\t";
                    }
                    else
                    {
                        outtemps<<"X\t";
                        outtemps_total<<"X\t";
                        outiter<<"X\t";
                    }
                }else
                {
                    outtemps<<"X\t";
                    outtemps_total<<"X\t";
                    outiter<<"X\t";
                }

        }
        outtemps<<std::endl;
        outtemps_total<<std::endl;
        outiter<<std::endl;
    }
    outtemps.close();
    outtemps_total.close();
    outiter.close();
}

// return false if data is no suitable
void read_data( const std::string &filename, data & d);

int main(int argc, char *argv[])
{
    std::vector<std::string> fail_files_;
    std::vector<std::string> rerun_command;
    if (argc == 2)
        desired_precision = atoi(argv[1]);

    std::cout<<"desired_precision = "<<desired_precision<<std::endl;
    QCoreApplication a(argc, argv);
	QDir directory("./");

    QStringList filesList = directory.entryList(QDir::Files);
    QString fileName;

    for (QStringList::iterator it = filesList.begin();it != filesList.end(); ++it) {
        QString current = *it;
        data tmp;
        read_data(current.toStdString(),tmp);
        {
            tmp.filename = current.toStdString();
            std::cout<<"File : "<< current.toStdString()<<std::endl;
            std::cout<<"status = "<< tmp.status<<std::endl;
            if(tmp.status == 5)
            {
                fail_files_.push_back(tmp.filename);
                std::string toto = "sbatch ../script/job.sh "+ std::to_string(tmp.solver)+" "+std::to_string(tmp.pb)+" "
                                    +std::to_string(tmp.nb_dof)+" "+std::to_string(tmp.precision);
                rerun_command.push_back(toto);
            }

            std::cout<<"nb_dof = "<< tmp.nb_dof<<std::endl;
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

    std::ofstream outfile ("result.csv");
//    // create the files
    for (int ii=0;ii<dof.size();ii++)   for (int i=0;i<precision.size();i++)   // 1 to nb-solver
    {
        outfile <<std::endl;
        outfile<<"precision \t"<< precision[i]<<std::endl;
        outfile<<"nb_dof \t"<< dof[ii]<<std::endl;

        outfile <<"solveur \ problem \t";
        for (int j=0;j<pb.size();j++)   outfile << pb[j]<<"\t";
        outfile<<std::endl;
        for (int k=0;k<solveur.size();k++)
        {
            outfile<< solveur[k]<<"\t";
            for (int j=0;j<pb.size();j++)
            {
                data d;
                if( find_data(solveur[k],pb[j],dof[ii],precision[i],d))
                {
                    if(d.status == 0)
                    {
                        outfile<< d.nb_iter<<" / "<< d.computation_time<<"( + "<< d.preparation_time<<") = "<< d.total_time;
                        if(solveur[k]>=20)
                            outfile<<"## "<< d.crit;
                        outfile<<"\t";
                    }

//                        outfile<< d.nb_iter<<" / "<< d.total_time <<"("<< d.preparation_time<<")\t";

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
                    outfile<<"X\t";
                }
            }
            outfile<<std::endl;
        }
    }
	outfile.close();

	std::ofstream filefile ("filenames.csv");
//    // create the files
    for (int ii=0;ii<dof.size();ii++)   for (int i=0;i<precision.size();i++)   // 1 to nb-solver
    {
        filefile <<std::endl;
        filefile<<"precision \t"<< precision[i]<<std::endl;
        filefile<<"nb_dof \t"<< dof[ii]<<std::endl;

        filefile <<"solveur \ problem \t";
        for (int j=0;j<pb.size();j++)   filefile << pb [j] <<"\t";
        filefile<<std::endl;
        for (int k=0;k<solveur.size();k++)
        {
            filefile<< solveur[k]<<"\t";
            for (int j=0;j<pb.size();j++)
            {
                data d;
                if( find_data(solveur[k],pb[j],dof[ii],precision[i],d))
                {
                    if(d.status == 1)
                        filefile<<"TIMEOUT ";
                    else if(d.status == 2)
                        filefile<<"CANCELLED ";
                    else if(d.status == 3)
                        filefile<<"MEMORY LIMIT ";
                    else if(d.status == 4)
                        filefile<<"RUNNING ";
                    else if(d.status == 5)
                        filefile<<"FAIL ";
                    else if(d.status == 6)
                        filefile<<"SEG FAULT";
                    filefile<< d.filename<<"\t";
                }else
                {
                    filefile<<"X\t";
                }
            }
            filefile<<std::endl;
        }
    }
	filefile.close();



        std::ofstream filefile3 ("draw_data_temps_par_iteration.csv");
//    // create the files
    for (int i=0;i<precision.size();i++)   // 1 to nb-solver
    {
        if(precision[i]== desired_precision)
        {

            for (int j=0;j<solveur.size();j++)
            {
                filefile3 <<std::endl;
                filefile3<<"precision \t"<< precision[i]<<std::endl;
                filefile3 <<"solveur \t"<< solveur[j]<<std::endl;
                 filefile3<<"nb_dof \t";
                for (int ii=0;ii<pb.size();ii++)
                        filefile3<< pb[ii]<<"\t";

                filefile3<<std::endl;
                filefile3<<"solver (iteration number)\t";
                filefile3<<std::endl;

                for (int k=0;k<dof.size();k++)
                {
                    //if( solveur[k]==3 || solveur[k]==4 || solveur[k]==6)
                    {
                        filefile3<<dof[k]<<"\t";
                        for (int ii=0;ii<pb.size();ii++)
                        {

                                data d;

                                if( find_data(solveur[j],pb[ii],dof[k],precision[i],d))
                                {
                                    if(d.status == 0)
                                         filefile3<<  d.time_per_iter<<"\t";
                                    else if(d.status == 1)
                                        filefile3<<"TIMEOUT\t";
                                    else if(d.status == 2)
                                        filefile3<<"CANCELLED\t";
                                    else if(d.status == 3)
                                        filefile3<<"MEMORY LIMIT\t";
                                    else if(d.status == 4)
                                        filefile3<<"RUNNING\t";
                                    else if(d.status == 5)
                                        filefile3<<"FAIL\t";
                                    else if(d.status == 6)
                                        filefile3<<"SEG FAULT\t";

                                }else
                                {
                                    filefile3<<"X\t";
                                }
                        }
                        filefile3<<std::endl;
                    }
                }
                filefile3<<std::endl;
                filefile3<<std::endl;
            }
        }
    }
        filefile3.close();



    std::ofstream filefile1 ("draw_data_iteration.csv");
//    // create the files
    for (int i=0;i<precision.size();i++)   // 1 to nb-solver
    {
        if(precision[i]== desired_precision)
        {

            for (int j=0;j<solveur.size();j++)
            {
                filefile1 <<std::endl;
                filefile1<<"precision \t"<< precision[i]<<std::endl;
                filefile1 <<"solveur \t"<< solveur[j]<<std::endl;
                 filefile1<<"pb \t";
                for (int ii=0;ii<pb.size();ii++)
                filefile1<< pb[ii]<<"\t";

                filefile1<<std::endl;
                filefile1<<"number of dof\t";
                filefile1<<std::endl;

                for (int k=0;k<dof.size();k++)
                {
//                    if( solveur[k]==3 || solveur[k]==4 || solveur[k]==6)
                    {
                        filefile1<< dof[k]<<"\t";
                        for (int ii=0;ii<pb.size();ii++)
                        {
                                data d;
                                if( find_data(solveur[j],pb[ii],dof[k],precision[i],d))
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
                                        filefile1<<"SEG FAULT\t";

                                }else
                                {
                                    filefile1<<"X\t";
                                }
                        }
                        filefile1<<std::endl;
                    }
                }
                filefile1<<std::endl;
                filefile1<<std::endl;
            }
        }
    }
    filefile1.close();
    {
        std::ofstream filefile11 ("draw_data_computation_time.csv");
    //    // create the files
        for (int i=0;i<precision.size();i++)   // 1 to nb-solver
        {
            if(precision[i]== desired_precision)
            {
                for (int j=0;j<solveur.size();j++)
                {
                    filefile11 <<std::endl;
                    filefile11<<"precision \t"<< precision[i]<<std::endl;
                    filefile11 <<"solver \t"<< solveur[j]<<std::endl;
                     filefile11<<"pb \t";
                    for (int ii=0;ii<pb.size();ii++)
                    filefile11<< pb[ii]<<"\t";

                    filefile11<<std::endl;
                    filefile11<<"number of dof\t";
                    filefile11<<std::endl;

                    for (int k=0;k<dof.size();k++)
                    {
    //                    if( solveur[k]==3 || solveur[k]==4 ||  solveur[k]==6)
                        {
                            filefile11<< dof[k]<<"\t";
                            for (int ii=0;ii<pb.size();ii++)
                            {

                                    data d;
                                    if( find_data(solveur[j],pb[ii],dof[k],precision[i],d))
                                    {
                                        if(d.status == 0)
                                             filefile11<< d.computation_time<<"\t";
                                        else if(d.status == 1)
                                            filefile11<<"TIMEOUT\t";
                                        else if(d.status == 2)
                                            filefile11<<"CANCELLED\t";
                                        else if(d.status == 3)
                                            filefile11<<"MEMORY LIMIT\t";
                                        else if(d.status == 4)
                                            filefile11<<"RUNNING\t";
                                        else if(d.status == 5)
                                            filefile11<<"FAIL\t";
                                        else if(d.status == 6)
                                            filefile11<<"SEG FAULT\t";

                                    }else
                                    {
                                        filefile1<<"X\t";
                                    }

                            }
                            filefile11<<std::endl;
                        }
                    }
                    filefile11<<std::endl;
                    filefile11<<std::endl;
                }
            }
        }
            filefile11.close();
    }


    {


        std::ofstream filefile11 ("draw_data_total_time.csv");
    //    // create the files
        for (int i=0;i<precision.size();i++)   // 1 to nb-solver
        {
            if(precision[i]== desired_precision)
            {
                for (int j=0;j<solveur.size();j++)
                {
                    filefile11 <<std::endl;
                    filefile11<<"precision \t"<< precision[i]<<std::endl;
                    filefile11 <<"solver \t"<< solveur[j]<<std::endl;
                     filefile11<<"pb \t";
                    for (int ii=0;ii<pb.size();ii++)
                    filefile11<< pb[ii]<<"\t";

                    filefile11<<std::endl;
                    filefile11<<"number of dof\t";
                    filefile11<<std::endl;

                    for (int k=0;k<dof.size();k++)
                    {
    //                    if( solveur[k]==3 || solveur[k]==4 ||  solveur[k]==6)
                        {
                            filefile11<< dof[k]<<"\t";
                            for (int ii=0;ii<pb.size();ii++)
                            {

                                    data d;
                                    if( find_data(solveur[j],pb[ii],dof[k],precision[i],d))
                                    {
                                        if(d.status == 0)
                                             //filefile11<< d.computation_time<<"\t";
                                             filefile11<< d.total_time<<"\t";
                                        else if(d.status == 1)
                                            filefile11<<"TIMEOUT\t";
                                        else if(d.status == 2)
                                            filefile11<<"CANCELLED\t";
                                        else if(d.status == 3)
                                            filefile11<<"MEMORY LIMIT\t";
                                        else if(d.status == 4)
                                            filefile11<<"RUNNING\t";
                                        else if(d.status == 5)
                                            filefile11<<"FAIL\t";
                                        else if(d.status == 6)
                                            filefile11<<"SEG FAULT\t";

                                    }else
                                    {
                                        filefile1<<"X\t";
                                    }

                            }
                            filefile11<<std::endl;
                        }
                    }
                    filefile11<<std::endl;
                    filefile11<<std::endl;
                }
            }
        }
            filefile11.close();
    }

    unsigned int id_precision = desired_precision;
    create_normalize_file("csp_2d_solveur2",2,1,id_precision);
    create_normalize_file("csp_2d_solveur3",3,1,id_precision);
    create_normalize_file("csp_2d_solveur4",4,1,id_precision);
    create_normalize_file("csp_2d_solveur_2_4",4,2,id_precision);

    create_normalize_file("optim_2d_solveur2",22,21,id_precision);
    create_normalize_file("optim_2d_solveur3",23,21,id_precision);
    create_normalize_file("optim_2d_solveur4",24,21,id_precision);

    create_normalize_file("optim_2d_solveur_2_4",24,22,id_precision);

    std::cout<<"List of fail files"<<std::endl;
    for (int i=0;i<fail_files_.size();i++)
    {
        std::cout<<fail_files_[i]<<" ";
    }
    std::cout<<std::endl;
     std::cout<<"List of rerun_command files"<<std::endl;
    for (int i=0;i<rerun_command.size();i++)
    {
        std::cout<<rerun_command[i]<<std::endl;
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
	fscanf (pFile, "ndof = %d\n", &(d.nb_dof));
	fscanf (pFile, "precision = %s\n", str);
	d.precision = toDouble(std::string(str));
	//  fscanf (pFile,"X = %s\n",str);    std::cout<<"str = "<< str <<std::endl;
	//  fscanf (pFile,"Y = %s\n",str);    std::cout<<"str = "<< str <<std::endl;
	fgets(str,800,pFile);
	fgets(str,800,pFile);
	fscanf (pFile, "problem %d\n", &d.pb);
	fscanf (pFile, "solveur %d\n", &d.solver);
	fgets(str,800,pFile); // *********** solve_bissection_interval *************
	fgets(str,800,pFile); //GIT BRANCH = master
	fgets(str,800,pFile); // samedi 19 août 2017, 22:08:49 (UTC+0200)
	fgets(str,800,pFile); //Creating solver for problem : Problem1_X0.200000_Y_1.700000
    fgets(str,800,pFile); // BissectionIntervalSolver::solve_csp_one_solution

	fscanf (pFile, "preparation time : %s seconds.\n", str);
	d.preparation_time = toDouble(std::string(str));
//	str = "01234567891011121314";
	fscanf (pFile, "Number of Bissections : %d\n", &d.nb_iter);
	fgets(str,800,pFile);  // Number of valid boxes : 0
	fgets(str,800,pFile);  // Number of possible boxes : 12
    fscanf (pFile, "computation time (wo prep) : %s seconds.\n", str);
	d.computation_time = toDouble(std::string(str));
//	str = "01234567891011121314";
    fscanf (pFile, "Time per iteration : %s seconds.\n",str);
    d.time_per_iter = toDouble(std::string(str));
//    str = "01234567891011121314";
    fscanf (pFile, "total time : %s seconds.\n",str);
    d.total_time = toDouble(std::string(str));
//    fscanf (pFile, "%s\n",str); // empty line
    fscanf (pFile, "crit = %s\n",str);
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
