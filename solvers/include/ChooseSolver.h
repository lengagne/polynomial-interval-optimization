#ifndef ChooseSolver_H
#define ChooseSolver_H

#include "AbstractSolver.h"
#include "ChooseBasisFunction.h"

class ChooseSolver
{
public:
    ChooseSolver();
    
    void choose(AbstractCSP* pb,
                AbstractSolver** solver, 
                uint index);
    
    std::string get_solver_name(uint index)
    {
        return solver_name_[index];
    }

    uint get_nb_solver() const
    {
        return solver_name_.size();
    }
    
private:
    uint nb_solver_;
    std::map<uint,std::string> solver_name_;
    
    ChooseBasisFunction choice_;
};

#endif // ChooseSolver_H
