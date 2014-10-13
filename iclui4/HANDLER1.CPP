#define INCL_DOS
#define INCL_PM
#include <os2.h>
#include <iapp.hpp>
#include <iexcept.hpp>
#include <icmdhdr.hpp>
#include <iframe.hpp>

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

main ()
{
        IFrameWindow dlg1 (4711);
        IFrameWindow dlg2 (4712);

        OKCancelHandler okhdl;
        okhdl.handleEventsFor (&dlg1);
        okhdl.handleEventsFor (&dlg2);

        if ( dlg1.showModally () == true )
                return dlg2.showModally ();
}

