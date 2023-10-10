#ifndef AbstractSolver_H
#define AbstractSolver_H
#include <iostream>
#include <fstream>

#include "AbstractCSP.h"
#include "Result.h"

typedef struct param_optim
{
    uint nb_bissections;
    uint nb_valid_boxes;
    uint nb_possible_boxes;
    uint nb_intermediate;
//     double ignored_space_;
    double computation_time;
    double computation_time_wo_prep;
    double optim;
    bool solution_found = true;
} param_optim;



class AbstractSolver
{
    public:
        bool bissect(   const Result& in,
                        Result &out1, 
                        Result& out2);

        bool check_size(   const Result& in);

        void close_files();

        void set_precision_gnuplot(const double a)
        {
            precision_gnuplot_ = a;
        }

        void set_print(bool in)
        {
            print_ = in;
        }

        void prepare_files(const std::string& filename);

        virtual param_optim solve_optim(double eps=1e-3) = 0;

        /// test if the Interval in is INSIDE/OUTSIDE or OVERLAPPING the bound
        check_constraint test_Interval( const Interval &in ,
                                       const Interval & bound);


    protected:
        AbstractCSP *pb_;

        std::vector<Result> results;
        std::vector<Interval> bounds_,bounds_input_;

        unsigned int nb_valid_box_;
        unsigned int nb_maybe_box_;
//         double ignored_space_;

        unsigned int nb_fun_, nb_var_;

        double precision_;
        unsigned long long int cpt_iter_;
        
        double tsglobal,ts, preparation_time_;

        Result optim_;

        bool print_ = false;
        
        uint bissection_type_ = 0;
        
    private:

        bool file_open = false;
        std::ofstream file_safe_;
        std::ofstream file_maybe_;
        std::ofstream file_gnuplot_input_;
        std::ofstream file_gnuplot_output_;
        double precision_gnuplot_ = 0.9e-3;
        std::string file_prefix_;
        unsigned int cpt_rec_input_, cpt_rec_output_;
        
        

        

};

#endif // AbstractSolver_H
