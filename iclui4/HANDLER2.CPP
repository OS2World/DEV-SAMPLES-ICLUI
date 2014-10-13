#define INCL_PM
#define INCL_DOS
#include <os2.h>
#include <iapp.hpp>
#include <iframe.hpp>
#include <ievent.hpp>
#include <ihandler.hpp>
#include <ipoint.hpp>

/*
        eventId wird als WM_USER+eventId benutzt.
*/
class UserEvent : public IEvent
{
 public :
 UserEvent(
    const IHandle &handle,
    unsigned long eventId = 0,
    const IEventParameter1 &parameter1 = 0,
    const IEventParameter2 &parameter2 = 0);
   

 UserEvent(
    IWindow *window,
    unsigned long eventId = 0,
    const IEventParameter1 &parameter1 = 0,
    const IEventParameter2 &parameter2 = 0) : 
        IEvent (window, eventId + WM_USER, parameter1, parameter2) {}

 UserEvent(const IEvent &event) : IEvent (event) {}

    unsigned long userId () const { return eventId () - WM_USER; }
};

/*
        ordne zu einem Window mit
        handleEventsFor (..) zu
        ruft 
                virtual Boolean user (UserEvent &event) fur jede Meldung wo
                MeldungId - WM_USER ist im Range
        mit 
                event.userId () kann der Inkrement zu WM_USER abgefragt werden.
        (WM_USER sollte hiermit verdeckt werden)
*/
class UserHandler : public IHandler
{
        private :
                IRange Range;
        protected :
                Boolean dispatchHandlerEvent (IEvent &e);
        public :
                UserHandler (const IRange &r = IRange ())
                { Range = r + WM_USER; }

                void setRange (const IRange &);
                const IRange &range () const { return Range; }

                virtual Boolean user (UserEvent &) = 0;
};


UserHandler::dispatchHandlerEvent (IEvent &e)
{
        if ( Range.includes (e.eventId ()) )
        {
                UserEvent ue (e);
                Boolean rev = user (ue);
                e.setResult (ue.result ());
                return rev;
        }
        return false;
}


class BeepHandler : public UserHandler
{
        private :
                Boolean user (UserEvent &e);
        public :
                BeepHandler () : UserHandler (IRange (1, 1)) 
                 {}
};

Boolean BeepHandler::user (UserEvent &e)
{
        DosBeep (e.parameter1 (), e.parameter2 ());
        return true;
}

main ()
{
        IFrameWindow f ("MusikFenster");
        BeepHandler beep;
        beep.handleEventsFor (&f);

        f.show ().setFocus ();
        f.postEvent (UserEvent (&f, 1, 100, 100));
        f.postEvent (UserEvent (&f, 1, 200, 100));
        f.postEvent (UserEvent (&f, 1, 300, 100));

        IApplication::current ().run ();
}

