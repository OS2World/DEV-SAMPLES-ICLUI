#define IC_DEVELOP

#include <iapp.hpp>
#include <iexcept.hpp>
#include <iframe.hpp>
#include <imsgbox.hpp>
#include <istring.hpp>
#include <ipushbut.hpp>
#include <icanvas.hpp>
#include <icmdhdr.hpp>

class Dialog : public IFrameWindow,
               public ICommandHandler
{
    Boolean handleException (IException &e, IEvent &)
    {
        IMessageBox box (this);
        box.setTitle ("Exception");
        return box.show (IString (e.errorId ()) + " : " + e.text (),
                IMessageBox::retryCancelButton|
                IMessageBox::defButton1|
                IMessageBox::errorIcon|
                IMessageBox::applicationModal) == IMessageBox::retry;
    }
    IPushButton PressMe;
    Boolean command (ICommandEvent &e)
    {
        IASSERT (e.commandId () != IC_FRAME_CLIENT_ID);
        return false;
    }
public :
    Dialog () : 
        IFrameWindow ("Hello World"),
        PressMe (10, this, this)
        { 
            PressMe.setText ("DO NOT Press Me");
            setClient (&PressMe);
            handleEventsFor (this);
            show (); setFocus (); 
        }
};

main ()
{
    Dialog d;
    IApplication::current ().run ();
}
