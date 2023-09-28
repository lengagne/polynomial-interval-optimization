
#include <iostream>

template < typename T >
void Problem3D_with_torque_limit::functions(   std::vector<T> & in,
                            std::vector<T> & out,
                            MogsDynamics<T> * kin)
{
    Eigen::Matrix<T,Eigen::Dynamic,1> q(nb_dof_),torque(nb_dof_);
    for (unsigned int i=0;i<nb_dof_;i++)
        q(i) = in[i];

//    kin->InverseDynamics(q,Eigen::Matrix<T,3,1>::Zero(nb_dof_),Eigen::Matrix<T,3,1>::Zero(nb_dof_),torque);
    kin->InverseDynamicsStatic(q,torque);
    Eigen::Matrix<T,3,1> eff,com;
    eff = kin->getPosition(eff_id_,offset_);

    out[0] = eff(0);
    out[1] = eff(1);
    out[2] = eff(2);
//    com = kin->getCenterOfMAss( );
////     mem_com(0).set_name("COM_X");
//    out[3] = com(0);
//    out[4] = com(1);

    int cpt = 3;
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
//             std::cout<<"torque("<<i<<") = "<< torque(i)<<std::endl;
            out[cpt] += torque(i)*torque(i);
         }

    }


//    for (int i=0;i<6;i++)
//        std::cout<<"out["<<i<<"] = "<< out[i]<<std::endl;
}
