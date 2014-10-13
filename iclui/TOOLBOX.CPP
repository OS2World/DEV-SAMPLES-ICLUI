/* Toolbox und Infozeile */
#include <iapp.hpp>                                                                                                                                               
#include <iframe.hpp>                                                                                                                                             
#include <isetcv.hpp>                                                                                                                                             
#include <igraphbt.hpp>                                                                                                                                           
#include <ihandle.hpp>                                                                                                                                            
#include <iinfoa.hpp>                                                                                                                                             
#include <ifont.hpp>                                                                                                                                              
// Klasse fuer Toolbox-Knoepfe                                                                                                                                    
class ToolButton : public IGraphicPushButton                                                                                                                      
{                                                                                                                                                                 
        public :                                                                                                                                                  
                ToolButton (unsigned long id, IWindow *parent,                                                                                                    
                                                            const IPointerHandle &pict) :                                                                         
                        IGraphicPushButton (id, parent, parent, pict,                                                                                             
                                IRectangle (IPoint (), ISize (20, 20)),                                                                                           
                                // verkleinere Grafik falls n”tig                                                                                                 
                                defaultStyle () | sizeToGraphic)                                                                                                  
                // ISetCanvas benutzt Minimumgr”áe um Controls zu                                                                                                 
                // Positionieren                                                                                                                                  
                { setMinimumSize (ISize (30, 30)); }                                                                                                              
};                                                                                                                                                                
// die Toolbox (Schalterleiste)                                                                                                                                   
class Toolbox : public ISetCanvas                                                                                                                                 
{                                                                                                                                                                 
        private :                                                                                                                                                 
                // erstelle zwei Kn”pfe                                                                                                                           
                ToolButton Frage;                                                                                                                                 
                ToolButton Antwort;                                                                                                                               
       public :                                                                                                                                                   
                Toolbox (IFrameWindow *f) :                                                                                                                       
                        ISetCanvas (10, f, f),                                                                                                                    
                        // System-Iconen werden benutzt                                                                                                           
                        Frage (10, this,                                                                                                                          
                                ISystemPointerHandle                                                                                                              
                                       (ISystemPointerHandle::question)),                                                                                         
                        Antwort (20, this,                                                                                                                        
                                ISystemPointerHandle                                                                                                              
                                       (ISystemPointerHandle::information))                                                                                       
                // einstellungen des Layouts                                                                                                                      
                { setMargin (ISize ()); setPad (ISize (3, 0)); }                                                                                                  
};                                                                                                                                                                
// Anwendungsfenster mit Toolbox und Info-Zeile                                                                                                                   
class ToolsFrame : public IFrameWindow                                                                                                                            
{                                                                                                                                                                 
        private :                                                                                                                                                 
                // Toolbox-Objekt                                                                                                                                 
                Toolbox toolbox;                                                                                                                                  
                // Infozeilen-Objekt                                                                                                                              
                IInfoArea info;                                                                                                                                   
                // Client-Fenster                                                                                                                                 
                ICanvas clientWindow;                                                                                                                             
        public :                                                                                                                                                  
                ToolsFrame (const char *s) :                                                                                                                      
                        IFrameWindow (s),                                                                                                                         
                        toolbox (this),                                                                                                                           
                        info (this),                                                                                                                              
                        clientWindow (10, this, this)                                                                                                             
                {                                                                                                                                                 
                        // Toolbox als Rahmen-Erweiterung hinzufgen                                                                                              
                        addExtension (&toolbox, aboveClient, 30);                                                                                                 
                        // H”he der Infozeile auf System-Font anpassen                                                                                            
                        setExtensionSize (&info,                                                                                                                  
                                                           (int)IFont ().maxCharHeight ());                                                                       
                        // Text in der Infozeile ausgeben                                                                                                         
                        info.setInactiveText ("Bereit...");                                                                                                       
                        setClient (&clientWindow);                                                                                                                
                }                                                                                                                                                 
};                                                                                                                                                                
main ()                                                                                                                                                           
{                                                                                                                                                                 
        ToolsFrame *f = new ToolsFrame ("Tools");                                                                                                                 
        f->show ().setFocus ();                                                                                                                                   
        IApplication::current ().run ();                                                                                                                          
}                                                                                                                                                                 
