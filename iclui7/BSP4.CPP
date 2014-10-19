#include <iostream.h>
#include <icritsec.hpp>
#include <ireslock.hpp>

class Stack : public IPrivateResource
{
        class Element
        {
            public :
                Element *next;
                int wert;
                Element (int w) { next = 0; wert = w; }
        } *sp;
    public :
        Stack () { sp = new Element(0); }
        ~Stack ();
        void push (int);
        int pop ();
        void print ();
        void move (Stack &);
};

Stack::~Stack ()
{
        while ( sp->next )
                pop ();
        delete sp;
}

void Stack::push (int n)
{
        ICritSec s;

        Element *e = new Element (n);
        e->next = sp->next;
        sp->next = e;
}

int Stack::pop ()
{
        IResourceLock l (*this);

        Element *e = sp->next;
        sp->next = e->next;
        int n = e->wert;
        delete e;
        return n;
}

void Stack::move (Stack &nach)
{
        IResourceLock l1 (*this);
        IResourceLock l2 (nach);
        nach.push (pop ());
}

void Stack::print ()
{
        Element *e = sp->next;
        while ( e )
        {
                cout << e->wert << " ";
                e = e->next;
        }
        cout << "\n";
}

main ()
{
        Stack a, b;
        a.push (1);
        a.push (2);
        a.push (3);
        a.move (b);
        a.print ();
        b.print ();
}
