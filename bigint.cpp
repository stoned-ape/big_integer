#include "bigint.h"


//constructors
bigint::bigint(){
    pos=true;
    data.push_back(0);
}
bigint::bigint(long x){
    fromlong(x);
}
bigint::bigint(string s){
    fromstring(s);
}

//assignment
bigint bigint::operator=(long x){
    data.clear();
    fromlong(x);
    return *this;
}

bigint bigint::operator=(bigint a){
    data=a.data;
    pos=a.pos;
    return *this;
}

bigint bigint::operator=(string s){
    fromstring(s);
    return *this;
}


//removes extra zeros from the end of the number
void bigint::reduce(){
    for(int i=data.size()-1;i>=1;i--){
        if(data[i]==0) data.pop_back();
        else break;
    }
}

//read or modify a perticular byte
uint8_t bigint::at(int i){
    assert(i>=0);
    if(i>=data.size()) return 0;
    return data[i];
}
uint8_t bigint::set(int i,uint8_t c){
    assert(i>=0);
    while(i>=data.size()) data.push_back(0);
    data[i]=c;
}

long bigint::tolong(){
    uint8_t c[8];
    for(int i=0;i<8;i++) c[i]=at(i);
    long x=*(long*)c;
    x=pos?x:-x;
    return x;
}
void bigint::fromlong(long x){
    uint8_t *d=(uint8_t*)&x;
    pos=x>=0;
    x=pos?x:-x;
    for(int i=0;i<8;i++) data.push_back(d[i]);
    reduce();
}


void bigint::fromstring(string s){
    bigint x,powten=1;
    pos=true;
    if(s[0]=='-'){
        pos=false;
        s=s.substr(1);
    }
    for(int i=s.size()-1;i>=0;i--){
        x+=((long)(s[i]-'0'))*powten;
        powten*=10;
    }
    data=x.data;
    reduce();
}
string bigint::tostring(){
    string s;
    bigint x=*this;
    x.pos=true;
    if(x==0) return "0";
    while(x>0){
        s=to_string((x%10).tolong())+s;
        x/=10;
    }
    if(!pos) s="-"+s;
    return s;
}


int bigint::size(){return data.size();}

void bigint::shift(int x){
    if(x>0){
        auto zeros=vector<uint8_t>(x,0);
        data.insert(data.begin(),zeros.begin(),zeros.end());
    }
    if(x<0) data.erase(data.begin(),data.begin()+min(-x,(int)data.size()));
    assert(data.size()>=0);
}

int bigint::msb(){
    return data.back();
}


//comparison operators
bool operator==(bigint a,bigint b){
    if(a.pos!=b.pos) return false;
    for(int i=0;i<max(a.size(),b.size());i++){
        if(a.at(i)!=b.at(i)) return false;
    }
    return true;
}
bool operator!=(bigint a,bigint b){
    return !(a==b);
}
bool operator>(bigint a,bigint b){
    if(a.pos!=b.pos) return a.pos;
    if(a==b) return false;
    for(int i=max(a.size(),b.size())-1;i>=0;i--){
        if(a.at(i)!=b.at(i)) return (a.at(i)>b.at(i))!=(!a.pos);
    }
    return !a.pos;
}
bool operator>=(bigint a,bigint b){
    return a>b || a==b;
}
bool operator<(bigint a,bigint b){
    return !(a>=b);
}
bool operator<=(bigint a,bigint b){
    return !(a>b);
}


//absolute value
bigint abs(bigint a){
    a.pos=true;
    return a;
}


//returns abs(a)+abs(b)
bigint raw_add(bigint &a,bigint &b){
    bigint c=0;
    int c_in=0;
    int i;
    for(i=0;i<max(a.size(),b.size());i++){
        int ai=a.at(i),bi=b.at(i);
        int ci=ai+bi+c_in;
        c.set(i,ci%256);
        c_in=ci/256;
    }
    c.set(i,c_in);
    c.reduce();
    return c;
}

//returns abs(a)-abs(b)
//only works if abs(a)>=abs(b)
bigint raw_sub(bigint &a,bigint &b){
    bigint c=0;
    int c_in=0;
    for(int i=0;i<max(a.size(),b.size());i++){
        int ai=a.at(i),bi=b.at(i);
        int ci=ai-bi-c_in;
        if(ci<0){
            ci+=256;
            c_in=1;
        }else c_in=0;
        c.set(i,ci);
    }
    c.reduce();
    return c;
}


bigint operator+(bigint a,bigint b){
    bigint c;
    if(a.pos==b.pos){
        c=raw_add(a,b);
        c.pos=a.pos;
    }else{
        if(abs(a)>=abs(b)){
            c=raw_sub(a,b);
            c.pos=a.pos;
        }else{
            c=raw_sub(b,a);
            c.pos=b.pos;
        }
    }
    return c;
}

//a-b=a+(-b)
bigint operator-(bigint a,bigint b){
    b.pos=!b.pos;
    return a+b;
}



bigint operator*(bigint a,bigint b){
    bigint c;
    for(int i=0;i<a.size();i++){
        bigint ci;
        int carry=0;
        for(int j=0;j<b.size()+1;j++){
            int x=(int)a.at(i)*(int)b.at(j)+carry;
            ci.set(i+j,x%256);
            carry=x/256;
        }
        c=c+ci;
    }
    c.pos=!(a.pos!=b.pos);
    c.reduce();
    return c;
}

bigint operator>>(bigint a,long x){
    assert(x>=0);
    a.shift(-(x/8));
    int rem=x%8;
    if(rem==0) return a;
    int out=0;
    for(int i=a.size()-1;i>=0;i--){
        int tmp=(a.data[i]%(1<<rem))<<(8-rem);
        a.data[i]>>=rem;
        a.data[i]|=out;
        out=tmp;
    }
    a.reduce();
    return a;
}

bigint operator<<(bigint a,long x){
    assert(x>=0);
    int rem=x%8;
    int out=0;
    for(int i=0;i<a.size();i++){
        int tmp=a.data[i]<<rem;
        a.data[i]=tmp;
        a.data[i]|=out;
        out=tmp>>8;
    }
    if(out) a.data.push_back(out);
    a.shift(x/8);
    return a;
}





#define GET_BIT(i,a) (((a)>>(i))&1)
#define SET_BIT(i,a,b) (a)|=(b)<<(i);
//from wikipedia
//Q := 0                  -- Initialize quotient and remainder to zero
//R := 0
//for i := n − 1 .. 0 do  -- Where n is number of bits in N
//  R := R << 1           -- Left-shift R by 1 bit
//  R(0) := N(i)          -- Set the least-significant bit of R equal to bit i of the numerator
//  if R ≥ D then
//    R := R − D
//    Q(i) := 1
//  end
//end

//quot=a/b; rem=a%b;
void divmod(bigint &a,bigint &b,bigint &quot,bigint &rem){
    quot=0;
    rem=0;
    quot.pos=!(a.pos != b.pos);
    bool bpos=b.pos;
    b.pos=true;
    for(int i=8*a.size()-1;i>=0;i--){
        rem<<=1;
        int biti=GET_BIT(i%8,a.data[i/8]);
        rem.data[0]|=biti;
        if(rem>=b){
            rem-=b;
            int qbyte=quot.at(i/8);
            SET_BIT(i%8,qbyte,1);
            quot.set(i/8,qbyte);
        }
    }
    b.pos=bpos;
    rem=a-b*quot;
    quot.reduce();
    rem.reduce();
}

bigint operator/(bigint a,bigint b){
    bigint quot,rem;
    divmod(a,b,quot,rem);
    return quot;
}
bigint operator%(bigint a,bigint b){
    bigint quot,rem;
    divmod(a,b,quot,rem);
    return rem;
}

bigint bigint::operator>>=(long x){
    *this=(*this)>>x;
    return *this;
}
bigint bigint::operator<<=(long x){
    *this=(*this)<<x;
    return *this;
}


#define MODIFYING_OP(opeq,op) \
    bigint bigint::operator opeq (bigint a){ \
        *this=(*this) op a; \
        return *this; \
    }

MODIFYING_OP(+=,+)
MODIFYING_OP(-=,-)
MODIFYING_OP(*=,*)
MODIFYING_OP(/=,/)
MODIFYING_OP(%=,%)

#define MODIFYING_OP_LONG(opeq,op) \
    bigint bigint::operator opeq (long a){ \
        *this=(*this) op a; \
        return *this; \
    }

MODIFYING_OP_LONG(+=,+)
MODIFYING_OP_LONG(-=,-)
MODIFYING_OP_LONG(*=,*)
MODIFYING_OP_LONG(/=,/)
MODIFYING_OP_LONG(%=,%)



#define LONG_OP_BIGINT(op,ret) \
    ret operator op (long a,bigint b){return bigint(a) op b;}

LONG_OP_BIGINT(==,bool)
LONG_OP_BIGINT(!=,bool)
LONG_OP_BIGINT(> ,bool)
LONG_OP_BIGINT(>=,bool)
LONG_OP_BIGINT(< ,bool)
LONG_OP_BIGINT(<=,bool)

LONG_OP_BIGINT(+,bigint)
LONG_OP_BIGINT(-,bigint)
LONG_OP_BIGINT(*,bigint)
LONG_OP_BIGINT(/,bigint)
LONG_OP_BIGINT(%,bigint)

#define BIGINT_OP_LONG(op,ret) \
    ret operator op (bigint a,long b){return a op bigint(b);}

BIGINT_OP_LONG(==,bool)
BIGINT_OP_LONG(!=,bool)
BIGINT_OP_LONG(> ,bool)
BIGINT_OP_LONG(>=,bool)
BIGINT_OP_LONG(< ,bool)
BIGINT_OP_LONG(<=,bool)

BIGINT_OP_LONG(+,bigint)
BIGINT_OP_LONG(-,bigint)
BIGINT_OP_LONG(*,bigint)
BIGINT_OP_LONG(/,bigint)
BIGINT_OP_LONG(%,bigint)
