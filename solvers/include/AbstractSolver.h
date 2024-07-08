#ifndef AbstractSolver_H
#define AbstractSolver_H

#define DEFAULT_SAVE_EACH_ITER 10000000

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
//         bool bissect(   const Result& in,
//                         Result &out1, 
//                         Result& out2);
        bool bissect(   const Result& in,
                        std::vector<Result>& Pile)     ;
        

        bool check_size(   const Result& in);
        
        /// test if the Interval in is INSIDE/OUTSIDE or OVERLAPPING the bound
        check_constraint test_Interval( const Interval &in ,
                                       const Interval & bound);
        

        virtual void evaluate(  const std::vector<Interval>& in,
                                std::vector<Interval> & out) = 0;
                                
        bool load_warm_start_filename(  const std::string& s,
                                        const Result & res  // is initialized
        );     
        
        virtual param_optim solve_optim(double eps=1e-3) = 0;
                                
        void save_current_state( const std::string& filename = "OptimTemporaryFile");

        void set_print(bool in)
        {
            print_ = in;
        }
        
        virtual param_optim set_results();
        
        void set_warm_start_filename( const std::string& s);
        
        std::string update_filename( const std::string& filename); // check the filename and update version.

    protected:
        AbstractCSP *pb_;

        std::vector<Result> current_vector_;
        
        std::vector<Result> results;
        std::vector<Interval> bounds_,bounds_input_;

        unsigned int nb_valid_box_;
        unsigned int nb_maybe_box_;
//         double ignored_space_;
        
        unsigned int nb_intermediate_ = 0;

        unsigned int nb_fun_, nb_var_;

        double precision_;
        unsigned long long int cpt_iter_;
        unsigned long int save_each_iter_ = DEFAULT_SAVE_EACH_ITER;
        unsigned long int saved_iter_ = 0;
        
        double current_time_;
        double start_preparation_time_;
        double start_computation_time_;
        double preparation_duration_ = 0.0;
        
        double previous_time_ = 0;
        
        double optim_crit_;
        bool find_one_feasible_ =false;
        Result optim_;

        bool print_ = false;
        
        uint bissection_type_ = 0;
        
        bool warm_start_ = false;
        std::string warm_start_filename_ = "no_warm_start";
        std::string save_filename_ = "OptimTemporaryFile";
    
               
    private:

        unsigned int cpt_rec_input_, cpt_rec_output_;
};

#endif // AbstractSolver_H
