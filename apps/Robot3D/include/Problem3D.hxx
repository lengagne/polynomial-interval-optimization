
#include <iostream>

template < typename T >
void Problem3D::functions(   std::vector<T> & in,
                            std::vector<T> & out,
                            MogsKinematics<T> * kin)
{
    Eigen::Matrix<T,Eigen::Dynamic,1> q(nb_dof_),torque(nb_dof_);
    for (unsigned int i=0;i<nb_dof_;i++)
        q(i) = in[i];

    std::cout<<"debut updatekin"<<std::endl;
    kin->UpdateKinematicsCustom(&q);
    std::cout<<"fin updatekin"<<std::endl;
    Eigen::Matrix<T,3,1> eff,com;
    eff = kin->getPosition(eff_id_,Eigen::Matrix<double,3,1>::Zero());

    out[0] = eff(0);
    out[1] = eff(1);
    out[2] = eff(2);
    com = kin->getCenterOfMAss( );
//     mem_com(0).set_name("COM_X");
    out[3] = com(0);
    out[4] = com(1);

    // criteria
    out[5] = 0;
    for (int i=0;i<nb_dof_;i++)
        out[5] += in[i]*in[i];

//    for (int i=0;i<6;i++)
//        std::cout<<"out["<<i<<"] = "<< out[i]<<std::endl;
}
