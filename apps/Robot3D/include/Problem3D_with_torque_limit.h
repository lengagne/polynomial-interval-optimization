#ifndef Problem3D_with_torque_limit_H
#define Problem3D_with_torque_limit_H

#include <AbstractCSP.h>
#include <vector>
#include <Eigen/Dense>
#include <string>
#include "MogsDynamics.h"

class Problem3D_with_torque_limit : public AbstractCSP
{
    public:
        Problem3D_with_torque_limit(const mogs_string &filename,
                                    const mogs_string &effector,
                                    double coeff_torque = 1.0,
                                    unsigned int target = 0,
                                    unsigned int crit = 0);

        template < typename T >
        void functions( std::vector<T> & in,
                        std::vector<T> & out,
                        MogsDynamics<T>* kin);

        void function( std::vector<double>& input,
                      std::vector<double>& output)
        {
            functions<double>(input,output,dkin_);
        }

        void function( std::vector<Interval>& input,
                      std::vector<Interval>& output)
        {
            functions<Interval>(input,output,Ikin_);
        }

        void function( std::vector<MogsInterval>& input,
                      std::vector<MogsInterval>& output)
        {
            functions<MogsInterval>(input,output,MIkin_);
        }
        void function( std::vector<IntervalsContainers>& input,
                       std::vector<IntervalsContainers>& output)
        {
            functions<IntervalsContainers>(input,output,ICkin_);
        }

        virtual void init();

        void prepare_non_linear( std::vector<MogsInterval>& in);

    protected:

    private:

    unsigned int nb_dof_;

    unsigned int eff_id_;
    double Xt,Yt,Zt,width;
    double coeff_torque_;

    unsigned int crit_;

    mogs_string filename_, eff_name_;

    RigidBodyDynamics::MogsRobotProperties* robot_;

    RigidBodyDynamics::MogsDynamics<double> * dkin_;
    RigidBodyDynamics::MogsDynamics<Interval> * Ikin_;
    RigidBodyDynamics::MogsDynamics<IntervalsContainers> * ICkin_;
    RigidBodyDynamics::MogsDynamics<MogsInterval> * MIkin_;

    Eigen::Matrix<double,3,1> offset_;

};

#include "Problem3D_with_torque_limit.hxx"

#endif // Problem3D_with_torque_limit_H
