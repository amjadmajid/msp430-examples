//https://proprogramming.org/program-to-implement-rsa-algorithm-in-c/

// #include<stdio.h>
#include <msp430.h>
#include <stdlib.h>
#include <math.h>
#include <uart-debugger.h>

//#define DEBUG 1

#define MSG "hello"
#define MSG_LEN 5

 int p,q,n,t,flag,e[20],d[20],temp[20],j,m[20],en[20],i;

char * msg = MSG;

int prime( int);
void ce();
 int cd( int);

void encrypt();
void decrypt();

int main()
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;
#ifdef DEBUG
    uart_init();
#endif

    // first prime number
    p =  5;
    flag=prime(p);
    if(flag==0)
    {
        exit(1);
    }

    // second prime number
    q = 7;
    flag=prime(q);
    if(flag==0||p==q)
    {
        exit(1);
    }

    // plain text message
    for(i=0; i < MSG_LEN  ;i++)
    {
        // hardware multiplier
        m[i]= *msg;
#ifdef DEBUG
        uart_sendChar(*msg);
#endif
        msg++;
    }

#ifdef DEBUG
     uart_sendText("\n\r", 2);
#endif

    n = p * q ;
    t = (p-1) * (q-1);

    ce();
    encrypt();
    decrypt();

    while(1);

    return 0;
}


int prime( int pr)
{
    int i;
    j=sqrt(pr);
    for(i=2;i<=j;i++)
    {
        if(pr%i==0)
            return 0;
    }
    return 1;
}
void ce()
{
    int k;
    k=0;
    for(i=2;i<t;i++)
    {
        if(t%i==0)
            {
                continue;
            }

        flag=prime(i);
        if(flag==1&&i!=p&&i!=q)
        {
            e[k]=i;
            flag=cd(e[k]);
            if(flag>0)
            {
                d[k]=flag;
                k++;
            }
            if(k==99)
                break;
        }
    }
}
 int cd( int x)
{
     int k=1;
    while(1)
    {
        k=k+t;
        if(k%x==0)
            return(k/x);
    }
}
void encrypt()
{
     int pt,ct,key=e[0],k;
    i=0;

    while(i!=MSG_LEN)
    {
        pt=m[i];
        pt=pt-96;
        k=1;
        for(j=0;j<key;j++)
        {
            k=k*pt;
            k=k%n;
        }
        temp[i]=k;
        ct=k+96;
        en[i]=ct;
        i++;
    }
    en[i]=-1;

#ifdef DEBUG
    uart_sendText("THE_ENCRYPTED_MESSAGE_IS\n\r", 26);
     for(i=0;en[i]!=-1;i++){
         uart_sendChar(en[i]);
     }
     uart_sendText("\n\r", 2);
#endif

}
void decrypt()
{
     int pt,ct,key=d[0],k;
    i=0;
    while(en[i]!=-1)
    {
        ct=temp[i];
        k=1;
        for(j=0;j<key;j++)
        {
            k=k*ct;
            k=k%n;
        }
        pt=k+96;
        m[i]=pt;
        i++;
    }
#ifdef DEBUG

    uart_sendText("THE_DECRYPTED_MESSAGE_IS\n\r", 26);
     for(i=0;i < MSG_LEN ;i++){
         uart_sendChar(m[i]);
     }
    uart_sendText("\n\r", 2);

#endif
}
