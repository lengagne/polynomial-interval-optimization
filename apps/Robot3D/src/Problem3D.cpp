#include "Problem3D.h"

Problem3D::Problem3D(   const mogs_string &filename,
                        const mogs_string &effector)
{
    filename_ = filename;
    eff_name_ = effector;

    robot_ = new RigidBodyDynamics::MogsRobotProperties();
    robot_->SetRobotFile(filename_);

    dkin_ = new RigidBodyDynamics::MogsKinematics<double>(robot_);
    Ikin_ = new RigidBodyDynamics::MogsKinematics<Interval>(robot_);
    ICkin_ = new RigidBodyDynamics::MogsKinematics<IntervalsContainers>(robot_);
    MIkin_ = new RigidBodyDynamics::MogsKinematics<MogsInterval>(robot_);

    nb_dof_ = dkin_->getNDof();
    eff_id_ = robot_->GetBodyId(eff_name_);
    std::cout<<"eff_id_ = "<< eff_id_ <<std::endl;
    width = 0.1;
    Xt = 0.4;
    Yt = 0.4;
    Zt = 0.7;
    get_criteria_ = true;
}

void Problem3D::init()
{
    nb_var_ = nb_dof_;
    nb_fun_ = 5;

    input_.resize(nb_dof_);
    std::vector<double> qmin(nb_dof_),qmax(nb_dof_);
    dkin_->model->getPositionLimit(qmin,qmax);
    for (int i=0;i<nb_dof_;i++)
    {
		input_[i] = Hull (qmin[i],qmax[i]);
//		input_[i] = Hull (0,0);
    }
//    input_[0] = Hull(0.5,0.5);
//    input_[1] = Hull(0.5,0.5);

    bound_.resize(nb_fun_);
    bound_[0] = Xt + Hull (-width,width);
    bound_[1] = Yt + Hull (-width,width);
    bound_[2] = Zt + Hull (-width,width);
    bound_[3] = Hull (-0.1,0.1);
    bound_[4] = Hull (-0.1,0.1);
}

void Problem3D::prepare_non_linear( std::vector<MogsInterval>& in)
{
    for (unsigned int i=0;i<nb_dof_;i++)
    {
        in[i].prepare_son(SON_COS_ERROR);
        in[i].prepare_son(SON_SIN_ERROR);
    }
}
