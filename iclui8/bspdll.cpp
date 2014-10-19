#include "bspdll.hpp"

Boolean Dialog::handleException (IException &e, IEvent &)
{
        IMessageBox box (this);
        box.setTitle ("Exception");
        return box.show (IString (e.errorId ()) + " : " + e.text (),
                IMessageBox::retryCancelButton|
                IMessageBox::defButton1|
                IMessageBox::errorIcon|
                IMessageBox::applicationModal) == IMessageBox::retry;
}

Boolean Dialog::command (ICommandEvent &e)
{
    IASSERT (e.commandId () != IC_FRAME_CLIENT_ID);
    return false;
}

Dialog::Dialog () : 
    IFrameWindow ("Hello World"),
    PressMe (10, this, this)
{ 
    PressMe.setText ("DO NOT Press Me");
    setClient (&PressMe);
    handleEventsFor (this);
    show (); setFocus (); 
}
