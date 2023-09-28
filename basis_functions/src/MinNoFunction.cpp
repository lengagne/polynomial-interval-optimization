#ifdef IPOPT_FOUND

#include "MinNoFunction.h"
       
MinNoFunction::MinNoFunction(bool re_order): OptimizedBasisFunction("MinNo",re_order)
{
    
    for (uint i=2;i<=6;i++)
    {
        Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> M(i+1,i+1);
        switch(i)
        {
            case(2):
                M(0,0) = 0.178806309606262398004262;
                M(0,1) = 0.64238738078747537052493;
                M(0,2) = 0.178806309606262314737535;
                M(1,0) = -0.479297229068229135862111;
                M(1,1) = -0;
                M(1,2) = 0.479297229068228969328658;
                M(2,0) = 0.321193690393737629751314;
                M(2,1) = -0.64238738078747537052493;
                M(2,2) = 0.321193690393737518729012;
                break;
            case(3):
                M(0,0) = 0.471677012817166452229856;
                M(0,1) = 0.0283229871828334020533724;
                M(0,2) = 0.0283229871828334055228193;
                M(0,3) = 0.471677012817166507741007;
                M(1,0) = 0.572949556791976566394453;
                M(1,1) = 0.208383836422600582372056;
                M(1,2) = -0.208383836422600637883207;
                M(1,3) = -0.572949556791976677416756;
                M(2,0) = -0.466241021723455162373284;
                M(2,1) = 0.466241021723455051350982;
                M(2,2) = 0.466241021723455106862133;
                M(2,3) = -0.466241021723455217884435;
                M(3,0) = -0.578385547885687856251025;
                M(3,1) = 0.286180172483687822460041;
                M(3,2) = -0.286180172483687877971192;
                M(3,3) = 0.578385547885687967273327;            
                break;
            case(4):
                M(0,0) = 0.00180793144642537261489179;
                M(0,1) = 0.232346382962395653359877;
                M(0,2) = 0.531691371182357874758395;
                M(0,3) = 0.23234638296239557009315;
                M(0,4) = 0.00180793144642537174753005;
                M(1,0) = -0.0407692760239636298180699;
                M(1,1) = -0.719547015917208487856271;
                M(1,2) = -0;
                M(1,3) = 0.719547015917208265811666;
                M(1,4) = 0.0407692760239636090013882;
                M(2,0) = 0.270116312225903154597972;
                M(2,1) = 0.324739874970824404787351;
                M(2,2) = -1.18971237439345478570374;
                M(2,3) = 0.324739874970824293765048;
                M(2,4) = 0.27011631222590304357567;
                M(3,0) = -0.454129454425434397890626;
                M(3,1) = 0.719547015917208487856271;
                M(3,2) = -0;
                M(3,3) = -0.719547015917208265811666;
                M(3,4) = 0.454129454425434175846021;
                M(4,0) = 0.224323773318000824206209;
                M(4,1) = -0.557086257933220085902803;
                M(4,2) = 0.665524969230437912770526;
                M(4,3) = -0.557086257933219863858199;
                M(4,4) = 0.224323773318000713183906;            
                break;
            case(5):
                M(0,0) = 0.412411538155208812206354;
                M(0,1) = 0.0870400370960398850339601;
                M(0,2) = 0.000548424748751144032488103;
                M(0,3) = 0.000548424748751144466168972;
                M(0,4) = 0.0870400370960398989117479;
                M(0,5) = 0.412411538155208923228656;
                M(1,0) = -0.611879357708125515280528;
                M(1,1) = -0.484329754243574495120583;
                M(1,2) = -0.0131845745763896366603563;
                M(1,3) = 0.0131845745763896453339736;
                M(1,4) = 0.484329754243574606142886;
                M(1,5) = 0.611879357708125737325133;
                M(2,0) = -0.879042152912897334360309;
                M(2,1) = 0.774368009443120786094994;
                M(2,2) = 0.104674143469776548265315;
                M(2,3) = 0.104674143469776617654254;
                M(2,4) = 0.774368009443121008139599;
                M(2,5) = -0.879042152912897667427217;
                M(3,0) = 1.34515982055804750316952;
                M(3,1) = 0.00640665206769360125182988;
                M(3,2) = -0.313819780144832527479792;
                M(3,3) = 0.313819780144832749524397;
                M(3,4) = -0.00640665206769360298655336;
                M(3,5) = -1.34515982055804794725873;
                M(4,0) = 0.470350694858960305388962;
                M(4,1) = -0.861075117492119956885688;
                M(4,2) = 0.390724422633159484963272;
                M(4,3) = 0.390724422633159762519028;
                M(4,4) = -0.861075117492120178930293;
                M(4,5) = 0.470350694858960416411264;
                M(5,0) = -0.737000542951193771123997;
                M(5,1) = 0.47825603122292181801356;
                M(5,2) = -0.168942636130465007049395;
                M(5,3) = 0.168942636130465118071697;
                M(5,4) = -0.478256031222921929035863;
                M(5,5) = 0.737000542951193993168602;
                break;
            case(6):
                M(0,0) = 0.000245971719213065236151611;
                M(0,1) = 0.0395409565634631601782623;
                M(0,2) = 0.234007455404784098229598;
                M(0,3) = 0.452411232625078651992112;
                M(0,4) = 0.234007455404784264763052;
                M(0,5) = 0.0395409565634632018116257;
                M(0,6) = 0.000245971719213064802470742;
                M(1,0) = 0.00499560392293574729444261;
                M(1,1) = -0.286015583767700254380628;
                M(1,2) = -0.76952906145843147367458;
                M(1,3) = -0;
                M(1,4) = 0.769529061458432139808394;
                M(1,5) = 0.286015583767700531936384;
                M(1,6) = -0.00499560392293573862082523;
                M(2,0) = 0.0402896296974870546381275;
                M(2,1) = 0.715107584995175016295832;
                M(2,2) = 0.132833272199762986165439;
                M(2,3) = -1.77646097378485046114349;
                M(2,4) = 0.132833272199763097187741;
                M(2,5) = 0.715107584995175793451949;
                M(2,6) = 0.0402896296974869783102946;
                M(3,0) = 0.162385419721291834882493;
                M(3,1) = -0.572705572285580943869832;
                M(3,2) = 1.64371245707376156808266;
                M(3,3) = -0;
                M(3,4) = -1.64371245707376290035029;
                M(3,5) = 0.572705572285581610003646;
                M(3,6) = -0.162385419721291529571161;
                M(4,0) = 0.336334715216080215594019;
                M(4,1) = -0.398220303779517947884159;
                M(4,2) = -1.08451820285275091215738;
                M(4,3) = 2.29280758283237862116266;
                M(4,4) = -1.0845182028527518003358;
                M(4,5) = -0.398220303779518336462218;
                M(4,6) = 0.336334715216079549460204;
                M(5,0) = 0.328442082896140075121139;
                M(5,1) = 0.858721156053281253761611;
                M(5,2) = -0.877741585342543095826784;
                M(5,3) = -0;
                M(5,4) = 0.877741585342543761960599;
                M(5,5) = -0.858721156053282141940031;
                M(5,6) = -0.328442082896139464498475;
                M(6,0) = 0.119119240444766419373401;
                M(6,1) = -0.356428237779120193895466;
                M(6,2) = 0.721687918170657027872039;
                M(6,3) = -0.968757841672606700988979;
                M(6,4) = 0.721687918170657582983551;
                M(6,5) = -0.356428237779120582473524;
                M(6,6) = 0.119119240444766197328796;            
                break;
            default:
                break;
        }
        mat_order_[i]= M;
        mat_inverse_order_[i] = M.inverse();
    }    
}
       
#endif // IPOPT_FOUND
