#include "Problem3D_with_torque_limit.h"

Problem3D_with_torque_limit::Problem3D_with_torque_limit(   const mogs_string &filename,
                                                            const mogs_string &effector,
                                                            double coeff_torque,
                                                            unsigned int target,
                                                            unsigned int crit)
{
    std::cout<<"Problem3D_with_torque_limit "<< std::endl;

    filename_ = filename;
    eff_name_ = effector;

    offset_(0) = 0.01;
    offset_(1) = 0.0;
    offset_(2) = 0.1;

    robot_ = new RigidBodyDynamics::MogsRobotProperties();
    robot_->SetRobotFile(filename_);

    dkin_ = new RigidBodyDynamics::MogsDynamics<double>(robot_);
    Ikin_ = new RigidBodyDynamics::MogsDynamics<Interval>(robot_);
    ICkin_ = new RigidBodyDynamics::MogsDynamics<IntervalsContainers>(robot_);
    MIkin_ = new RigidBodyDynamics::MogsDynamics<MogsInterval>(robot_);

    nb_dof_ = dkin_->getNDof();
    eff_id_ = robot_->GetBodyId(eff_name_);
//     std::cout<<"eff_id_ = "<< eff_id_ <<std::endl;
    width = 0.01;
    switch(target)
    {
        case(1):
            Xt = 0.6;
            Yt = 0.6;
            Zt = 0.6;
            break;
        case(0):
        default:
            Xt = 0.4;
            Yt = 0.4;
            Zt = 0.7;
    }
    get_criteria_ = true;
    coeff_torque_ = coeff_torque;
    crit_ = crit;
}

void Problem3D_with_torque_limit::init()
{
    nb_var_ = nb_dof_;
    nb_fun_ = 3 + nb_dof_;

    input_.resize(nb_dof_);
    std::vector<double> qmin(nb_dof_),qmax(nb_dof_),tmax(nb_dof_);;
    dkin_->model->getPositionLimit(qmin,qmax);
    for (int i=0;i<nb_dof_;i++)
    {
		input_[i] = Hull (qmin[i],qmax[i]);
//		input_[i] = Hull (0,0);
    }
//    input_[0] = Hull(-2.5,-2.2);
//    input_[1] = Hull(-0.1,0.2);
//    input_[2] = Hull(-1.8,-1.5);
//    input_[3] = Hull(-0.1,0.2);

    bound_.resize(nb_fun_);
    bound_[0] = Xt + Hull (-width,width);
    bound_[1] = Yt + Hull (-width,width);
    bound_[2] = Zt + Hull (-width,width);
    dkin_->model->getTorqueLimit(tmax);
    int cpt = 3;
    for(int i=0;i<nb_dof_;i++)
        bound_[cpt++] = coeff_torque_* Hull(-tmax[i],tmax[i]);
}

void Problem3D_with_torque_limit::prepare_non_linear( std::vector<MogsInterval>& in)
{
    for (unsigned int i=0;i<nb_dof_;i++)
    {
        in[i].prepare_son(SON_COS_ERROR);
        in[i].prepare_son(SON_SIN_ERROR);
    }
}
