#define INCL_DOS
#define INCL_PM
#include <os2.h>
#include <iapp.hpp>
#include <iexcept.hpp>
#include <icmdhdr.hpp>
#include <iframe.hpp>
#include <ievent.hpp>
#include <ihandler.hpp>
#include <ithread.hpp>

class OKCancelHandler : public ICommandHandler
{
        private :
                IHandler &handleEventsFor (IWindow *);
        protected :
                Boolean command (ICommandEvent &);
        public :
                IHandler &handleEventsFor (IFrameWindow *);
};

IHandler &OKCancelHandler::handleEventsFor (IWindow *)
{
        ITHROWLIBRARYERROR (IC_MEMBER_ACCESS_ERROR,
                            IErrorInfo::invalidRequest,
                            IException::recoverable);
        return *this;
}

IHandler &OKCancelHandler::handleEventsFor (IFrameWindow *w)
{
        return ICommandHandler::handleEventsFor (w);
}

Boolean OKCancelHandler::command (ICommandEvent &e)
{
        IFrameWindow *fw = (IFrameWindow *)e.window ();

        switch ( e.commandId () )
        {
                case DID_OK : 
                        fw->dismiss (true);
                        return true;
                case DID_CANCEL : 
                        fw->dismiss (false);
                        return true;
        }
        return false;
}

class TimeHandler : public IHandler
{
        private :
           IWindowHandle hwnd;
           long timerId;
        public:
           TimeHandler () { timerId = 0; }
           Boolean dispatchHandlerEvent( IEvent &evt );
           IHandler &handleEventsFor (IWindow *);
           IHandler &stopHandlingEventsFor (IWindow *);

           void startTimer (unsigned long milisec);
           void stopTimer ();
           long id () const { return timerId; }

           virtual Boolean tick (IEvent &) = 0;
};

IHandler &TimeHandler::handleEventsFor (IWindow *w)
{
        if ( w && w->isValid () )
                hwnd = w->handle ();
        return IHandler::handleEventsFor (w);
}

IHandler &TimeHandler::stopHandlingEventsFor (IWindow *w)
{
        if ( w && w->isValid () && w->handle () == hwnd && timerId != 0 )
        {
                WinStopTimer (IThread::current ().anchorBlock (), hwnd, timerId);
                hwnd = 0;
                timerId = 0;
        }
        return IHandler::stopHandlingEventsFor (w);
}

void TimeHandler::startTimer (unsigned long milisec)
{
        if ( hwnd == 0 ) return;
        timerId = WinQuerySysValue (HWND_DESKTOP, SV_CTIMERS);
        WinStartTimer(IThread::current().anchorBlock (), hwnd, timerId, milisec);
}

void TimeHandler::stopTimer ()
{
        if ( hwnd == 0 || timerId == 0 ) return;
        WinStopTimer (IThread::current ().anchorBlock (), hwnd, timerId);
        timerId = 0;
        
}

Boolean TimeHandler::dispatchHandlerEvent (IEvent &e)
{
        if ( e.eventId () == WM_TIMER &&
             e.parameter1 () == timerId )
        {
                return tick (e);
        }
        return false;
}


class LogoFenster : public IFrameWindow,
                    private TimeHandler
{
        private :
                Boolean tick (IEvent &);
        public :
                LogoFenster (const IResourceId &r, IWindow *o = 0) :
                        IFrameWindow (r, o)
                        { handleEventsFor (this); }
                Boolean logoExpected ();
};

Boolean LogoFenster::logoExpected ()
{
        long time = PrfQueryProfileInt (HINI_PROFILE, "PM_ControlPanel", "LogoDisplayTime", -1);
        if ( time > 0 )
                TimeHandler::startTimer (time);
        return time != 0;
}

Boolean LogoFenster::tick (IEvent &e)
{
        stopTimer ();
        dismiss (true);
        return true;
}

main ()
{
        IFrameWindow f ("Die Anwendung");
        LogoFenster logo (4711, &f);

        OKCancelHandler okhdl;
        okhdl.handleEventsFor (&logo);

        f.show ();

        if ( logo.logoExpected () )
                logo.showModally ();
        else
                f.setFocus ();

        IApplication::current ().run ();        
}
