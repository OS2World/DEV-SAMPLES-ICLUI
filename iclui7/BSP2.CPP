#define INCL_BASE
#include <os2.h>
#include <iapp.hpp>
#include <iframe.hpp>
#include <istattxt.hpp>
#include <istring.hpp>
#include <ithread.hpp>

void _Optlink countdown (void *t)
{
        IStaticText *text = (IStaticText *)t;
        for ( int i = 100; i; --i )
        {
                DosSleep (1000);
                text->setText (IString (i));
        }
}

main ()
{
        IFrameWindow w ("Countdown");
        IStaticText txt (10, &w, &w);
        w.setClient (&txt).show ().setFocus ();
        IThread (countdown, &txt);
        IApplication::current ().run ();
}
