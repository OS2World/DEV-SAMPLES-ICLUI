#include <iapp.hpp>
#include <iframe.hpp>
#include <istattxt.hpp>
#include <istring.hpp>

main ()
{
        ICurrentApplication &a = IApplication::current ();
        IFrameWindow w ("Mein Programm");
        IStaticText txt (10, &w, &w);
        txt.setText ("Meine Nummer ist " + IString (a.id ()));
        w.setClient (&txt).show ().setFocus ();
        a.run ();
}
