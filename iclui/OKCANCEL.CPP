/* OKCancelHandler */
#include <os2.h>                                                                                                                                                 
#include <icmdhdr.hpp>                                                                                                                                           
#include <iframe.hpp>                                                                                                                                            
class OKCancelHandler : private                                           ICommandHandler                                                                        
{                                                                                                                                                                
   private :                                                                                                                                                     
     IFrameWindow *frame;                                                                                                                                        
     Boolean command (ICommandEvent &e)                                                                                                                          
      {                                                                                                                                                          
          switch ( e.commandId () )                                                                                                                              
          {                                                                                                                                                      
              case DID_OK : frame->dismiss (1);                                                                                                                  
                                  return true;                                                                                                                   
             case DID_CANCEL :                                                                                                                                   
                                          frame->dismiss (0);                                                                                                    
                                  return true;                                                                                                                   
          }                                                                                                                                                      
          return false;                                                                                                                                          
      }                                                                                                                                                          
   public :                                                                                                                                                      
     OKCancelHandler (IFrameWindow *f) :                                                                                                                         
           frame (f) { handleEventsFor (f); }                                                                                                                    
}                                                                                                                                                               
