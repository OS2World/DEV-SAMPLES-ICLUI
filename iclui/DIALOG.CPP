/* Dialog aus Ressource laden */
#include <iapp.hpp>                                                        
#include <iframe.hpp>                                                      
#include <ientryfd.hpp>                                                    
#include <istring.hpp>                                                     
#include "okcnclhdr.hpp"                                                   
#include "dlg.h"                                                           
class ZielverzDialog : public IFrameWindow   {                             
   private :                                                               
      OKCancelHandler stdCmdHandler;                                       
      IEntryField Verzeichnis;                                             
   public :                                                                
      ZielverzDialog (IWindow *p = 0) :                                    
        IFrameWindow (DLG_ZIELVERZ, p),                                    
        stdCmdHandler (this)  {}                                           
      IString verzeichnis () const                                         
                  { return Verzeichnis.text (); }                          
};                                                                         
