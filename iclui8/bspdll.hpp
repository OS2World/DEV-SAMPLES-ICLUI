#define IC_DEVELOP

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
    Boolean handleException (IException &e, IEvent &);
    IPushButton PressMe;
    Boolean command (ICommandEvent &e);
public :
    Dialog ();
};

