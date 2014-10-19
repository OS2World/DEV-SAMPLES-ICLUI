#define INCL_BASE
#define INCL_WIN
#include <os2.h>
#include <iapp.hpp>
#include <iframe.hpp>
#include <iwindow.hpp>
#include <ihandler.hpp>
#include <ievent.hpp>
#include <ithread.hpp>
#include <iobjwin.hpp>


#define EXECUTE (WM_USER + 1)
#define DONE    (WM_USER + 2)

class UserHandler : public IHandler
{
        private :
                IRange Range;
        protected :
                Boolean dispatchHandlerEvent (IEvent &e)
                {
                        if ( Range.includes (e.eventId ()) )
                        {
                                e.setResult (user (e));
                                return true;
                        }
                        return false;
                }
        public :
                UserHandler (IRange const &r = IRange ()) :
                        Range (r) {}

                virtual Boolean user (IEvent const &) = 0;
};

class Aufgabe : public IRefCounted
{
        public :
                virtual Boolean loese () = 0;
                virtual void melde (Boolean) = 0;
};

class Rueckmelder : public UserHandler
{
                Boolean user (IEvent const &e)
                {
                        Aufgabe *a = (Aufgabe *)(void *)e.parameter1 ();
                        a->melde (e.parameter2 ());
                        return true;
                }
        public :
                Rueckmelder () :
                        UserHandler (IRange (DONE, DONE))
                {}
};

class Dienstobjekt : public IObjectWindow,
                      public UserHandler
{
                IWindowHandle aClient;
                Boolean user (IEvent const &e)
                {
                      Aufgabe *a = (Aufgabe *)(void *)e.parameter1 ();
                      Boolean m = a->loese ();
                      aClient.postEvent (DONE, a, m);
                      return true;
                }
        public :
                Dienstobjekt::Dienstobjekt (IWindowHandle c) :
                      UserHandler (IRange (EXECUTE,EXECUTE))
                {
                        aClient = c;
                        handleEventsFor (this);
                }
};

class Server : public IThread
{
        private :
                IWindowHandle  aClient;
                Dienstobjekt* volatile pServer;
                Rueckmelder    aMelder;
                void run ()
                {
                        pServer = new Dienstobjekt (aClient);
                        current ().processMsgs ();
                }
        public :
                Server (IWindow *client)
                {
                        aClient = client->handle ();
                        aMelder.handleEventsFor (client);
                        pServer = 0;
                        start (new IThreadMemberFn<Server> (*this, run));
                }
                ~Server ()
                {
                        pServer->sendEvent (IEvent (pServer, WM_QUIT, 0, 0));
                        delete pServer;
                }
                void erledige (Aufgabe *a)
                {
                        while ( pServer == 0 ) current ().sleep (1);
                        pServer->postEvent (IEvent (pServer, EXECUTE, a, 0));
                }
};

template <class Klasse>
class Hintergrund : public Aufgabe
{
                Klasse &object;
                Boolean (Klasse::*aufgabe)();
                void (Klasse::*rueckmeldung)(Boolean);
                
                Boolean loese () { return (object.*aufgabe)(); }
                void melde (Boolean m) { (object.*rueckmeldung)(m); }
        public :
                Hintergrund (Klasse &o, Boolean (Klasse::*a)(), 
                        void (Klasse::*r)(Boolean)) :
                        object (o), aufgabe (a), rueckmeldung (r)
                {}
};

class MeinFenster : public IFrameWindow,
                    public Server
{
        public :
                Boolean rechne ()
                {
                        beginFlashing ();
                        DosSleep (5000);
                        return true;
                }
                void melde (Boolean erfolg)
                {
                        endFlashing ();
                }
                MeinFenster () :
                        IFrameWindow ("Aufgabe"),
                        Server (this)
                {}
};

main ()
{
        MeinFenster f;
        f.show ().setFocus ();
        f.erledige (new Hintergrund<MeinFenster> 
                (f, MeinFenster::rechne, MeinFenster::melde));
        IApplication::current ().run ();
}
