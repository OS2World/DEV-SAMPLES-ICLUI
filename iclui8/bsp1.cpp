#include <iapp.hpp>
#include <iexcept.hpp>
#include <iframe.hpp>
#include <imsgbox.hpp>
#include <istring.hpp>
main ()
{
        IFrameWindow *f = 0;
        try
        {
                f = new IFrameWindow ("Hello world");
                f->show ().setFocus ();
                IApplication::current ().run ();
        }
        catch ( IException &e )
        {
                IMessageBox abort ((f && f->isValid ()) ? (IWindow *)f : 
                                                IWindow::desktopWindow ());
                abort.setTitle ("Exception Caught");
                IString err = IString ("error id : ") +
                              IString (e.errorId ()) + "\n" + e.text ();
                for ( int i = 0; i < e.locationCount (); ++i )
                {
                        const IExceptionLocation *l = e.locationAtIndex (i);
                        err += "\n" + IString (l->fileName ()) + "(" + 
                                IString (l->lineNumber ()) + ") " + IString (l->functionName ());
                }
                abort.show (err,
                     IMessageBox::okButton|IMessageBox::defButton1|IMessageBox::errorIcon|
                        ((f && f->isValid ()) ? IMessageBox::applicationModal : IMessageBox::systemModal));
        }
        IApplication::current ().exit ();
}
