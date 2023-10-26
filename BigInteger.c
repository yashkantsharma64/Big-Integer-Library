#include "BigInteger.h"
struct BigInteger initialize(char *s)
{
    int l=strlen(s);
    struct BigInteger A;
    A.length=l;
    A.L=(struct node*)malloc(sizeof(struct node));
    struct node *temp=A.L;
    int i=l-1;
    do
    {
        l-=1;
        temp->data=(int)(*(s+i)-48);
        if(l)
        {
            temp->next=(struct node*)malloc(sizeof(struct node));
            temp=temp->next;
        }
        i--;
    }while(l);
    temp->next=NULL;
    return A;
}

struct BigInteger rev(struct BigInteger A)
{
    struct node *curr=A.L, *next = A.L, *prev=NULL;
    while(curr)
    {
        next=curr->next;
        curr->next=prev;
        prev=curr;
        curr=next;
    }
    A.L=prev;
    return A;
}

struct BigInteger length(struct BigInteger A)
{
    int l=0;
    struct node *temp=A.L;
    while(temp)
    {
        l++;
        temp=temp->next;
    }
    A.length=l;
    return A;
}

void compare(struct BigInteger *A, struct BigInteger *B)
{
    if(A->L == NULL || B->L == NULL) return;
    if(A->length < B->length)
    {
        struct node *temp = A->L;
        A->L = B->L;
        B->L = temp; 
        int tempL=A->length;
        A->length=B->length;
        B->length=tempL;      
    }
}

void display(struct BigInteger A)
{
    if(!A.L)
    {
        printf("List is Empty!");
        return;
    }
    struct BigInteger temp=A;
    temp=rev(temp);
    struct node *tempNode=temp.L;
    while(tempNode!=NULL)
    {
        printf("%d",tempNode->data);
        tempNode=tempNode->next;
    }
    printf("\n");
    temp=rev(temp);
}

void free_list(struct node *head) {
    struct node *temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

struct BigInteger add(struct BigInteger a, struct BigInteger b)
{
    if(a.L==NULL) return b;
    if(b.L==NULL) return a;
    compare(&a,&b);
    int c=0;
    struct BigInteger res; 
    res.L=NULL;
    struct node *sum=NULL;
    while(b.L!=NULL)
    {
        if(!res.L)
        {
            res.L=(struct node*)malloc(sizeof(struct node));
            sum=res.L;
        }
        else
        {
            sum->next=(struct node*)malloc(sizeof(struct node));
            sum=sum->next;
        }
        sum->data=(c + a.L->data + b.L->data)%10;
        c=(c + a.L->data + b.L->data)/10;
        a.L=a.L->next;
        b.L=b.L->next;
    }
    while(a.L!=NULL)
    {
        sum->next=(struct node*)malloc(sizeof(struct node));
        sum=sum->next;
        sum->data=(a.L->data + c)%10;
        c=(a.L->data + c)/10;
        a.L=a.L->next;
    }
    if(c!=0)
    {
        sum->next=(struct node*)malloc(sizeof(struct node));
        sum=sum->next;
        sum->data=c;
    }
    sum->next=NULL;
    return res;
}

struct BigInteger sub(struct BigInteger a, struct BigInteger b)
{
    if(comp(a, b)==1)
    {
        struct node *temp = a.L;
        a.L = b.L;
        b.L = temp; 
        int tempL=a.length;
        a.length=b.length;
        b.length=tempL; 
    }
    int bor=0;
    struct BigInteger res;
    res.L=NULL;
    struct node *sub=NULL, *prev=NULL;
    while(b.L!=NULL)
    {
        int v=a.L->data;
        if(!res.L)
        {
            res.L=(struct node*)malloc(sizeof(struct node));
            prev=sub;
            sub=res.L;
        }
        else
        {
            prev=sub;
            sub->next=(struct node*)malloc(sizeof(struct node));
            sub=sub->next;
        }
        if(bor==1) --v;
        if(v < b.L->data)
        {
            sub->data=(10+v)-b.L->data;
            bor=1;
        }
        else
        {
            sub->data=v - b.L->data;
            bor=0;
        }
        a.L=a.L->next;
        b.L=b.L->next;
    }
    while(a.L!=NULL)
    {
        int v=a.L->data;
        prev=sub;
        sub->next=(struct node*)malloc(sizeof(struct node));
        sub=sub->next;
        if(bor==1) --v;
        if(a.L->next==NULL && a.L->data==0)
        {
            free(sub);
            prev->next=NULL;
        }
        else if(v==-1)
        {
            sub->data=10+v;
            bor=1;
        }
        else
        {
            sub->data=v;
            bor=0;
        }
        a.L=a.L->next;
    }
    sub->next=NULL;
    res=rev(res);
    struct node *u=res.L;
    while(u->data==0 && u->next!=NULL)
    {
        u=u->next;
    }
    res.L=u;
    res=rev(res);
    return res;
}

struct BigInteger mul(struct BigInteger a, struct BigInteger b)
{
    compare(&a, &b);
    struct BigInteger res;
    if(b.L->data == 0 && b.L->next == NULL) 
    {
        res=initialize("0");
        return res;
    }
    int carry=0, c=0, temp=0;
    struct BigInteger n1 = a, n2 = b, m = initialize("0");
    while(n2.L)
    {
        n1=a;
        res.L=NULL;
        struct BigInteger temp_res;
        while(n1.L)
        {
            int v1 = n1.L->data;
            int v2 = n2.L->data;
            if(!res.L)
            {
                res.L=(struct node*)malloc(sizeof(struct node));
                temp_res=res;
            }
            else
            {
                res.L->next=(struct node*)malloc(sizeof(struct node));
                res.L=res.L->next;
            }
            res.L->data=(v1*v2+carry)%10;
            carry = (v1*v2+carry)/10;
            n1.L=n1.L->next;
        }
        if(carry)
        {
            res.L->next=(struct node*)malloc(sizeof(struct node));
            res.L=res.L->next;
            res.L->data=carry;
        }
        res.L->next=NULL;
        temp=c;
        while(temp--)
        {
            struct BigInteger new=initialize("0");
            new.L->next=temp_res.L;
            temp_res.L=new.L;
        }
        c++;
        m=add(m, temp_res);
        m=length(m);
        n2.L=n2.L->next;
    }
    return m;
}

int comp(struct BigInteger A, struct BigInteger B)
{
    if(A.length!=B.length)
        return (A.length<B.length?1:0);
    else{
        struct node *itr1=A.L, *itr2=B.L;
        while(itr1){
            if(itr1->data < itr2->data)
            {
                return 1;
            }
            if(itr1->data > itr2->data)
            {
                return 0;
            }
            itr1=itr1->next;
            itr2=itr2->next;
        }
    }
    return 2;
}

struct BigInteger div1(struct BigInteger A, struct BigInteger B)
{
    struct BigInteger C;
    C.L=(struct node*)malloc(sizeof(struct node));
    C.L->data=0;
    C.L->next=NULL;
    C.length=1;
    C=rev(C);
    struct BigInteger count;
    count.L=(struct node*)malloc(sizeof(struct node));
    count.L->data=1;
    count.L->next=NULL;
    count.length=1;
    struct BigInteger res=B;
    int l=0;
    while((comp(res=rev(res), A=rev(A))))
    {
        res=rev(res);
        A=rev(A);
        C=add(C,count);
        C=length(C);
        res=add(res, B);
        res=length(res);
    }
    res=rev(res);
    A=rev(A);
    //free_list(count.L);
    C=rev(C);
    struct node *u=C.L;
    while(u->data==0 && u->next!=NULL)
    {
        u=u->next;
    }
    C.L=u;
    C=rev(C);
    return C;
}

struct BigInteger mod(struct BigInteger A, struct BigInteger B)
{
    struct BigInteger C;
    C.L=(struct node*)malloc(sizeof(struct node));
    C.L->data=0;
    C.L->next=NULL;
    C.length=1;
    C=rev(C);
    struct BigInteger count;
    count.L=(struct node*)malloc(sizeof(struct node));
    count.L->data=1;
    count.L->next=NULL;
    count.length=1;
    struct BigInteger res=B, prev;
    int l=0;
    while((comp(res=rev(res), A=rev(A))))
    {
        res=rev(res);
        A=rev(A);
        C=add(C,count);
        C=length(C);
        prev=res;
        res=add(res, B);
        res=length(res);
    }
    res=rev(res);
    A=rev(A);
    struct BigInteger s;
    s=sub(A, prev);
    return s;
}