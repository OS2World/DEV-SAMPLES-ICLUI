/* MDI-Fensterklasse */
#include <iapp.hpp>                                                                                                                                                    
#include <iframe.hpp>                                                                                                                                                  
#include <icanvas.hpp>                                                                                                                                                 
                                                                                                                                                                       
class MDIFrame : public IFrameWindow                                                                                                                                   
{                                                                                                                                                                      
 private :                                                                                                                                                             
  ICanvas Client;                                                                                                                                                      
 public :                                                                                                                                                              
    // Konsttruktor fr Hauptfenster                                                                                                                                   
    MDIFrame (const char *title,    const IResourceId &r = IC_DEFAULT_FRAME_ID,                                                                                        
    const IFrameWindow::Style &s = defaultStyle ()) :                                                                                                                  
       IFrameWindow (title, r, s),                                                                                                                                     
       Client (0x8008, this, this)                                                                                                                                     
    {                                                                                                                                                                  
        setClient (&Client);                                                                                                                                           
    }                                                                                                                                                                  
    // Konstruktor fr geschachtelte MDI-Fenster                                                                                                                       
    MDIFrame (const IResourceId &r,  IFrameWindow *parent,                                                                                                             
    const char *title = 0,   const IRectangle &rec = IRectangle (),                                                                                                    
    const IFrameWindow::Style &s =  defaultStyle () &~ windowList | alignNoAdjust) :                                                                                   
       IFrameWindow (r, parent->client (), parent->client (),                                                                                                          
       rec == IRectangle () ?   IRectangle (IPoint (), parent->client ()->size ()) : rec, s, title),                                                                   
       Client (0x8008, this, this)                                                                                                                                     
    {                                                                                                                                                                  
        setClient (&Client);                                                                                                                                           
    }                                                                                                                                                                  
};                                                                                                                                                                     
                                                                                                                                                                       
main ()                                                                                                                                                                
{                                                                                                                                                                      
 MDIFrame *frame = new MDIFrame ("MDI Main Frame");                                                                                                                    
 frame->show ().setFocus ();                                                                                                                                           
 MDIFrame *achild = new MDIFrame (4711, frame, "a Child");                                                                                                             
 achild->show ().setFocus ();                                                                                                                                          
 IApplication::current ().run ();                                                                                                                                      
}                                                                                                                                                                      
