#ifndef BIGINT_H
#define BIGINT_H

#include <iostream>
#include <vector>
#include <assert.h>
using namespace std;



struct bigint{
    vector<uint8_t> data;
    bool pos;
    bigint();
    bigint(long x);
    bigint(string s);
    bigint operator=(long x);
    bigint operator=(string s);
    bigint operator=(bigint a);
    void reduce();
    uint8_t at(int i);
    uint8_t set(int i,uint8_t c);
    void fromlong(long x);
    long tolong();
    void fromstring(string s);
    string tostring();
    int size();
    void shift(int i);
    int msb();
    bigint operator+=(bigint a);
    bigint operator-=(bigint a);
    bigint operator*=(bigint a);
    bigint operator/=(bigint a);
    bigint operator%=(bigint a);
    bigint operator+=(long a);
    bigint operator-=(long a);
    bigint operator*=(long a);
    bigint operator/=(long a);
    bigint operator%=(long a);
    bigint operator>>=(long x);
    bigint operator<<=(long x);
};

bool operator==(bigint a,bigint b);
bool operator!=(bigint a,bigint b);
bool operator> (bigint a,bigint b);
bool operator>=(bigint a,bigint b);
bool operator< (bigint a,bigint b);
bool operator<=(bigint a,bigint b);

bool operator==(long a,bigint b);
bool operator!=(long a,bigint b);
bool operator> (long a,bigint b);
bool operator>=(long a,bigint b);
bool operator< (long a,bigint b);
bool operator<=(long a,bigint b);

bool operator==(bigint a,long b);
bool operator!=(bigint a,long b);
bool operator> (bigint a,long b);
bool operator>=(bigint a,long b);
bool operator< (bigint a,long b);
bool operator<=(bigint a,long b);



bigint operator+(bigint a,bigint b);
bigint operator-(bigint a,bigint b);
bigint operator*(bigint a,bigint b);
bigint operator/(bigint a,bigint b);
bigint operator%(bigint a,bigint b);

bigint operator+(long a,bigint b);
bigint operator-(long a,bigint b);
bigint operator*(long a,bigint b);
bigint operator/(long a,bigint b);
bigint operator%(long a,bigint b);

bigint operator+(bigint a,long b);
bigint operator-(bigint a,long b);
bigint operator*(bigint a,long b);
bigint operator/(bigint a,long b);
bigint operator%(bigint a,long b);

bigint operator>>(bigint a,long x);
bigint operator<<(bigint a,long x);
bigint abs(bigint a);

#endif
