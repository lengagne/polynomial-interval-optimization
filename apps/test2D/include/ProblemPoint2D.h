#ifndef ProblemPoint2D_H
#define ProblemPoint2D_H

#include <AbstractCSP.h>
#include <vector>


class ProblemPoint2D : public AbstractCSP
{
    public:
        ProblemPoint2D():AbstractCSP()
        {
            pb_name_ = "ProblemPoint2D";
        }

        template < typename T >
        void functions( std::vector<T> & in,
                        std::vector<T> & out);

        void function( std::vector<double>& input,
                      std::vector<double>& output)
        {
            functions<double>(input,output);
        }

        void function( std::vector<Interval>& input,
                      std::vector<Interval>& output)
        {
            functions<Interval>(input,output);
        }

        void function( std::vector<MogsInterval>& input,
                      std::vector<MogsInterval>& output)
        {
            functions<MogsInterval>(input,output);
        }
        void function( std::vector<IntervalsContainers>& input,
                       std::vector<IntervalsContainers>& output)
        {
            functions<IntervalsContainers>(input,output);
        }

        virtual void init();

        virtual void prepare_non_linear( std::vector<MogsInterval>& in);

    protected:

    private:
        std::vector<double> P1;
        std::vector<double> P2;
        std::vector<double> P3;

};

#include "ProblemPoint2D.hxx"

#endif // ProblemPoint2D_H
