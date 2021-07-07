#include "bigint.h"
#include <sys/time.h>
using namespace std;


#define COMP_OP_TEST(op,opstr) \
    if((a op b)  != (i op j) ) \
        cout<<"error: "<<i<<" "<<opstr<<" "<<j<<"\n";

#define MATH_OP_TEST(op,opstr) \
    if((a op b).tolong()!=i op j) \
        cout<<"error: "<<i<<" "<<opstr<<" "<<j<<" != "<<(a op b).tolong()<<endl;

#define SHIFT_OP_TEST(op,opstr) \
    if((a op j).tolong() != (i op j)) \
        cout<<"error: "<<i<<" "<<opstr<<" "<<j<<" != "<<(a>>j).tolong()<<endl;

#define DOUBLE_SHIFT_TEST() \
    if((a<<j)>>j != a) \
        cout<<"error: ("<<i<<"<<"<<j<<")>>"<<j<<" != "<<((a<<j)>>j).tolong()<<endl;

struct timer{
    float start=0;
    void begin(){
        struct timeval tp;
        assert(-1!=gettimeofday(&tp,nullptr));
        cout<<"timer started\n";
        start=tp.tv_sec%(60*60*60);
        start+=tp.tv_usec/1E6;
    }
    float end(){
        struct timeval tp;
        assert(-1!=gettimeofday(&tp,nullptr));
        float stop=tp.tv_sec%(60*60*60);
        stop+=tp.tv_usec/1E6;
        cout<<stop-start<<" seconds elapsed\n";
        return stop-start;
    }
};

int main(){
    timer timer;
    cout<<"bigint\n";
    
    for(long i=-300;i<=300;i+=1){
        bigint a=i;
        if(a.tostring()!=to_string(i))
            cout<<"error: "<<a.tostring()<<" != "<<to_string(i)<<endl;
        bigint b=to_string(i);
        if(b.tolong()!=i)
            cout<<"error: "<<b.tolong()<<" != "<<i<<" string constuctor"<<endl;
    }
    
    cout<<"small tests \n";
    timer.begin();
    for(long i=-300;i<=300;i+=2){
        for(long j=-300;j<=300;j+=2){
            bigint a=i,b=j;
            
            MATH_OP_TEST(+,"+")
            MATH_OP_TEST(-,"-")
            MATH_OP_TEST(*,"*")
            if(j!=0){
                MATH_OP_TEST(/,"/")
                MATH_OP_TEST(%,"%")
            }
            if(j>=0 && i>=0){
                if(j<64){
                    SHIFT_OP_TEST(>>,">>")
                    SHIFT_OP_TEST(<<,"<<")
                }
                DOUBLE_SHIFT_TEST()
            }
            COMP_OP_TEST(>,">")
            COMP_OP_TEST(<,"<")
            COMP_OP_TEST(>=,">=");
            COMP_OP_TEST(<=,"<=");
            COMP_OP_TEST(==,"==");
            COMP_OP_TEST(!=,"!=");
        }
    }
    timer.end();
//    return 0;
    cout<<"medium tests \n";
    timer.begin();
    for(long i=INT_MIN;i<=INT_MAX;i+=20000000){
        for(long j=INT_MIN;j<=INT_MAX;j+=20000000){
            bigint a=i,b=j;
            
            MATH_OP_TEST(+,"+")
            MATH_OP_TEST(-,"-")
            MATH_OP_TEST(*,"*")
            if(j!=0){
                MATH_OP_TEST(/,"/")
                MATH_OP_TEST(%,"%")
            }
            if(j>=0 && i>=0){
                if(j<64){
                    SHIFT_OP_TEST(>>,">>")
                    SHIFT_OP_TEST(<<,"<<")
                }

            }
            COMP_OP_TEST(>,">")
            COMP_OP_TEST(<,"<")
            COMP_OP_TEST(>=,">=");
            COMP_OP_TEST(<=,"<=");
            COMP_OP_TEST(==,"==");
            COMP_OP_TEST(!=,"!=");
        }
    }
    timer.end();
    
}
