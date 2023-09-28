#ifdef IPOPT_FOUND

#include "MinVoFunction.h"
       
MinVoFunction::MinVoFunction(bool re_order): OptimizedBasisFunction("MinVo",re_order)
{
    for (uint i=2;i<=6;i++)
    {
        Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> M(i+1,i+1);
        switch(i)
        {
            case(2):    
                M(0,0) = 0.124999999989722193616259;
                M(0,1) = 0.750000000020555779300935;
                M(0,2) = 0.124999999989722165860684;
                M(1,0) = 0.43301270188035156927242;
                M(1,1) = -0;
                M(1,2) = -0.433012701880351513761269;
                M(2,0) = 0.375000000010277889650467;
                M(2,1) = -0.750000000020555779300935;
                M(2,2) = 0.375000000010277834139316;                
                break;
            case(3):    
                M(0,0) = 0.499589700832236194294467;
                M(0,1) = 0.000410299167763511778324326;
                M(0,2) = 0.000410299167763511669904108;
                M(0,3) = 0.499589700832236305316769;
                M(1,0) = 0.79209309517640580367015;
                M(1,1) = 0.0269818845542093600775591;
                M(1,2) = -0.0269818845542093531386652;
                M(1,3) = -0.792093095176405914692452;
                M(2,0) = -0.456775449677571498963147;
                M(2,1) = 0.456775449677571609985449;
                M(2,2) = 0.456775449677571443451995;
                M(2,3) = -0.456775449677571609985449;
                M(3,0) = -0.834907346331070443490319;
                M(3,1) = 0.430203864291125726015963;
                M(3,2) = -0.430203864291125614993661;
                M(3,3) = 0.834907346331070665534924;                
                break;
            case(4):    
                M(0,0) = 0.030226624286642647665424;
                M(0,1) = 0.148258749045392629328788;
                M(0,2) = 0.643029253335929640300606;
                M(0,3) = 0.148258749045392629328788;
                M(0,4) = 0.0302266242866426684821057;
                M(1,0) = -0.138090559576529381402565;
                M(1,1) = 0.810774978494443132603919;
                M(1,2) = -0;
                M(1,3) = -0.810774978494443132603919;
                M(1,4) = 0.138090559576529464669292;
                M(2,0) = -0.0943519981549797992492756;
                M(2,1) = 0.960202083764212677152727;
                M(2,2) = -1.73170017121846542274;
                M(2,3) = 0.960202083764212677152727;
                M(2,4) = -0.0943519981549798547604269;
                M(3,0) = 0.575789414387923303983996;
                M(3,1) = -0.810774978494443132603919;
                M(3,2) = -0;
                M(3,3) = 0.810774978494443132603919;
                M(3,4) = -0.575789414387923748073206;
                M(4,0) = 0.525519724873835025746871;
                M(4,1) = -1.10846083280960527872594;
                M(4,2) = 1.16588221587154006186893;
                M(4,3) = -1.10846083280960527872594;
                M(4,4) = 0.525519724873835358813778;
                break;
            case(5):    
                M(0,0) = 0.0458942584273195550159663;
                M(0,1) = 0.00289529674930036990884719;
                M(0,2) = 0.451210444823380407708413;
                M(0,3) = 0.451210444823380851797623;
                M(0,4) = 0.00289529674930037988350717;
                M(0,5) = 0.04589425842731972154942;
                M(1,0) = -0.0365007397331579760968445;
                M(1,1) = -0.120976965272899320003042;
                M(1,2) = -1.0644033477187497815919;
                M(1,3) = 1.06440334771875089181492;
                M(1,4) = 0.120976965272899722458888;
                M(1,5) = 0.0365007397331581079358287;
                M(2,0) = -0.37728448430909883315465;
                M(2,1) = 1.33300268701657453895848;
                M(2,2) = -0.955718202707476760515704;
                M(2,3) = -0.955718202707477759716426;
                M(2,4) = 1.33300268701657897985058;
                M(2,5) = -0.377284484309100165422279;
                M(3,0) = 0.330192287467862999328361;
                M(3,1) = -1.36554987757336165898892;
                M(3,2) = 2.77654338993122351553211;
                M(3,3) = -2.77654338993122662415658;
                M(3,4) = 1.36554987757336632192562;
                M(3,5) = -0.330192287467864220573688;
                M(4,0) = 0.776885122348896950406072;
                M(4,1) = -1.31931846323575241441972;
                M(4,2) = 0.542433340886855797080557;
                M(4,3) = 0.542433340886856352192069;
                M(4,4) = -1.31931846323575685531182;
                M(4,5) = 0.776885122348899836985936;
                M(5,0) = -0.739186444201822778765631;
                M(5,1) = 1.50310636337638370285674;
                M(5,2) = -1.75006562521523312270233;
                M(5,3) = 1.75006562521523489905917;
                M(5,4) = -1.50310636337638880988266;
                M(5,5) = 0.739186444201825443300891;                
                break;
            case(6):    
                M(0,0) = 0.0183279729346426667468961;
                M(0,1) = 0.0312090154153383432522606;
                M(0,2) = 0.152009417528717161172125;
                M(0,3) = 0.596907188242602138039672;
                M(0,4) = 0.152009417528717161172125;
                M(0,5) = 0.0312090154153383571300484;
                M(0,6) = 0.0183279729346426598080022;
                M(1,0) = 0.136709395362293983922086;
                M(1,1) = 0.243262947137719537371581;
                M(1,2) = -1.08091492402073763301473;
                M(1,3) = -0;
                M(1,4) = 1.08091492402073763301473;
                M(1,5) = -0.243262947137719648393883;
                M(1,6) = -0.136709395362293928410935;
                M(2,0) = 0.105858626067640002044179;
                M(2,1) = -0.0845381745057780659147184;
                M(2,2) = 1.58029133191595128060669;
                M(2,3) = -3.20322356695562682205036;
                M(2,4) = 1.58029133191595128060669;
                M(2,5) = -0.0845381745057781075480818;
                M(2,6) = 0.105858626067639960410816;
                M(3,0) = -0.834817379796876313235998;
                M(3,1) = -2.29857525920596028612408;
                M(3,2) = 2.46818359280614707529367;
                M(3,3) = -0;
                M(3,4) = -2.46818359280614707529367;
                M(3,5) = 2.2985752592059611743025;
                M(3,6) = 0.834817379796875980169091;
                M(4,0) = -0.736850477249668234236424;
                M(4,1) = 2.28116808314185837147647;
                M(4,2) = -4.26999281291672705407336;
                M(4,3) = 5.45135041404907116913137;
                M(4,4) = -4.26999281291672705407336;
                M(4,5) = 2.28116808314185925965489;
                M(4,6) = -0.736850477249668012191819;
                M(5,0) = 1.13401922178491165738023;
                M(5,1) = 2.05531231206824038793002;
                M(5,2) = -1.40857669894556636336347;
                M(5,3) = -0;
                M(5,4) = 1.40857669894556636336347;
                M(5,5) = -2.05531231206824127610844;
                M(5,6) = -1.13401922178491121329102;
                M(6,0) = 1.0606256632553843211042;
                M(6,1) = -2.22783892405141870085572;
                M(6,2) = 2.58973027846405834395682;
                M(6,3) = -2.84503403533604659614298;
                M(6,4) = 2.58973027846405834395682;
                M(6,5) = -2.22783892405141958903414;
                M(6,6) = 1.06062566325538387701499;                
                break;

                //                 M(0,0) = 0.124999999969926764431527;
//                 M(0,1) = 0.750000000060146332359068;
//                 M(0,2) = 0.124999999969926750553739;
//                 M(1,0) = -0.433012701857493742529925;
//                 M(1,1) = -0;
//                 M(1,2) = 0.433012701857493687018774;
//                 M(2,0) = 0.375000000030073221690685;
//                 M(2,1) = -0.750000000060146332359068;
//                 M(2,2) = 0.375000000030073166179534;                
//                 break;
            default:
                break;
        }
        mat_order_[i]= M;
        mat_inverse_order_[i] = M.inverse();
    }    
}
       
#endif // IPOPT_FOUND
