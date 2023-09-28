#ifndef ProblemPoly_H
#define ProblemPoly_H

#include <AbstractCSP.h>
#include <vector>

class ProblemPoly : public AbstractCSP
{
    public:
        ProblemPoly( );

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

        void prepare_non_linear( std::vector<MogsInterval>& in);

    protected:

    private:

};

#include "ProblemPoly.hxx"

#endif // ProblemPoly_H
