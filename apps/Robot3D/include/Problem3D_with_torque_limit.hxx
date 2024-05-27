
#include <iostream>

template < typename T >
void Problem3D_with_torque_limit::functions(   std::vector<T> & in,
                            std::vector<T> & out,
                            MogsDynamics<T> * kin)
{
    Eigen::Matrix<T,Eigen::Dynamic,1> q(nb_dof_),torque(nb_dof_);
    for (unsigned int i=0;i<nb_dof_;i++)
        q(i) = in[i];

    kin->InverseDynamicsStatic(q,torque);
    Eigen::Matrix<T,3,1> eff,com;
    eff = kin->getPosition(eff_id_,offset_);

//     T a = cos(in[0]);    
//     out[0] = a*in[0] + in[1]*in[1];
//     for (int i=1;i<12;i++)
//         out[i] = in[0];   
//     out[11] = cos(in[0]) * sin(in[0]);
    
    out[0] = eff(0);
    out[1] = eff(1);
    out[2] = eff(2);
    com = kin->getCenterOfMAss( );
    out[3] = com(0);
    out[4] = com(1);

    int cpt = 5;
    for (int i=0;i<nb_dof_;i++)
        out[cpt++] = torque(i);
    
    // criteria
    out[cpt] = 0;    
    if(crit_ == 0)  // min of q²
    {
        for (int i=0;i<nb_dof_;i++)
            out[cpt] += in[i]*in[i];
    }else if (crit_ == 1)
    {
         for (int i=0;i<nb_dof_;i++)
         {
            out[cpt] += torque(i)*torque(i);
         }
    }
    
}
