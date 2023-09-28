#ifndef Problem3D_H
#define Problem3D_H

#include <AbstractCSP.h>
#include <vector>
#include <Eigen/Dense>
#include <string>
#include "MogsKinematics.h"

class Problem3D : public AbstractCSP
{
    public:
        Problem3D(const mogs_string &filename,
                  const mogs_string &effector);

        template < typename T >
        void functions( std::vector<T> & in,
                        std::vector<T> & out,
                        MogsKinematics<T>* kin);

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

    mogs_string filename_, eff_name_;

    RigidBodyDynamics::MogsRobotProperties* robot_;

    RigidBodyDynamics::MogsKinematics<double> * dkin_;
    RigidBodyDynamics::MogsKinematics<Interval> * Ikin_;
    RigidBodyDynamics::MogsKinematics<IntervalsContainers> * ICkin_;
    RigidBodyDynamics::MogsKinematics<MogsInterval> * MIkin_;

};

#include "Problem3D.hxx"

#endif // Problem3D_H
