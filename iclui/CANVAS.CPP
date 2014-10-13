/* Ein Dialogfenster mit Canvas-Klassen */
#include <iapp.hpp>                                                                                                                                                       
#include <iframe.hpp>                                                                                                                                                     
#include <imcelcv.hpp>                                                                                                                                                    
#include <isetcv.hpp>                                                                                                                                                     
#include <istattxt.hpp>                                                                                                                                                   
#include <ientryfd.hpp>                                                                                                                                                   
#include <icombobx.hpp>                                                                                                                                                   
#include <ilistbox.hpp>                                                                                                                                                   
#include <ipushbut.hpp>                                                                                                                                                   
class OeffnenDialogClient : public IMultiCellCanvas                                                                                                                       
{ // Client-Fenster des Dialoges                                                                                                                                          
        private :                                                                                                                                                         
                IStaticText tDateiname; // FÅhrungstexte                                                                                                                  
                IStaticText tDateityp;                                                                                                                                    
                IStaticText tLaufwerke;                                                                                                                                   
                IStaticText tDateien;                                                                                                                                     
                IEntryField eDateiname; // Eingabefelder                                                                                                                  
                IComboBox cDateityp;                                                                                                                                      
                IComboBox cLaufwerke;                                                                                                                                     
                IListBox    lDateien;                                                                                                                                     
                IStaticText tVerzeichnisse;                                                                                                                               
                IListBox    lVerzeichnisse;                                                                                                                               
                class Knoepfe : public ISetCanvas                                                                                                                         
                { // Hilfsklasse fÅr die Anordnung der Knîpfe                                                                                                             
                        private :                                                                                                                                         
                                IPushButton bOeffnen;                                                                                                                     
                                IPushButton bAbbruch;                                                                                                                     
                                IPushButton bHilfe;                                                                                                                       
                        public :                                                                                                                                          
                                Knoepfe (unsigned long id, IWindow *parent) :                                                                                             
                                        ISetCanvas (id, parent, parent),                                                                                                  
                                        bOeffnen (10, this, this),                                                                                                        
                                        bAbbruch (20, this, this),                                                                                                        
                                        bHilfe (30, this, this)                                                                                                           
                                {                                                                                                                                         
                                        bOeffnen.setText ("~ôffnen");                                                                                                     
                                        bAbbruch.setText ("Abbruch");                                                                                                     
                                        bHilfe.setText ("Hilfe");                                                                                                         
                                }                                                                                                                                         
                } sKnoepfe;                                                                                                                                               
        public :                                                                                                                                                          
                OeffnenDialogClient (unsigned long id, IWindow *parent) :                                                                                                 
                        IMultiCellCanvas (id, parent, parent),                                                                                                            
                        tDateiname (10, this, this),                                                                                                                      
                        eDateiname (20, this, this),                                                                                                                      
                        tDateityp (30, this, this),                                                                                                                       
                        cDateityp (40, this, this, IRectangle (),                                                                                                         
                                IComboBox::defaultStyle ()                                                                                                                
                                                     &~IComboBox::simpleType                                                                                              
                                                     | IComboBox::dropDownListType),                                                                                      
                        tLaufwerke (50, this, this),                                                                                                                      
                        cLaufwerke (60, this, this, IRectangle (),                                                                                                        
                                IComboBox::defaultStyle ()                                                                                                                
                                                    &~IComboBox::simpleType                                                                                               
                                                     | IComboBox::dropDownListType),                                                                                      
                        tDateien (70, this, this),                                                                                                                        
                        lDateien (80, this, this),                                                                                                                        
                        tVerzeichnisse (90, this, this),                                                                                                                  
                        lVerzeichnisse (100, this, this),                                                                                                                 
                        sKnoepfe (110, this)                                                                                                                              
                {                                                                                                                                                         
                        tDateiname.setText ("Dateiname :");                                                                                                               
                        tDateityp.setText ("Dateityp");                                                                                                                   
                        tLaufwerke.setText ("Laufwerke");                                                                                                                 
                        tDateien.setText ("Dateien");                                                                                                                     
                        tVerzeichnisse.setText ("Verzeichnisse");                                                                                                         
                        // addToCell (Fenster, spalte, zeile,                                                                                                             
                        //                                       anzahl-spalten, anzahl-zeilen)                                                                           
                        addToCell (&tDateiname, 1, 1);                                                                                                                    
                        addToCell (&eDateiname, 1, 2, 3, 1);                                                                                                              
                        addToCell (&tDateityp, 1, 3);                                                                                                                     
                        addToCell (&tLaufwerke, 3, 3);                                                                                                                    
                        addToCell (&cDateityp, 1, 4);                                                                                                                     
                        addToCell (&cLaufwerke, 3, 4);                                                                                                                    
                        addToCell (&tDateien, 1, 5);                                                                                                                      
                        addToCell (&tVerzeichnisse, 3, 5);                                                                                                                
                        addToCell (&lDateien, 1, 6);                                                                                                                      
                        addToCell (&lVerzeichnisse, 3, 6);                                                                                                                
                        addToCell (&sKnoepfe, 1, 7, 3, 1);                                                                                                                
                        // hiermit werden zeile 6 und spalten 1 und 3 grî·enverÑnderbar                                                                                   
                        setRowHeight (6, 0, true);                                                                                                                        
                        setColumnWidth (1, 0, true);                                                                                                                      
                        setColumnWidth (3, 0, true);                                                                                                                      
                }                                                                                                                                                         
};                                                                                                                                                                        
class OeffnenDialog : public IFrameWindow                                                                                                                                 
{ // Rahmen-Fenster-Klasse                                                                                                                                                
        private :                                                                                                                                                         
                OeffnenDialogClient Client;                                                                                                                               
        public :                                                                                                                                                          
                OeffnenDialog () :                                                                                                                                        
                        IFrameWindow ("ôffnen", IC_DEFAULT_FRAME_ID,                                                                                                      
                                // Dialog mit grî·enverÑnderbarem Rahmen                                                                                                  
                                isible|systemMenu|                                                                                                                        
                                titleBar|sizingBorder|dialogBackground),                                                                                                  
                        Client (10, this)                                                                                                                                 
               { setClient (&Client); show (); setFocus (); }                                                                                                             
};                                                                                                                                                                        
main ()                                                                                                                                                                   
{                                                                                                                                                                         
        OeffnenDialog aDialog;                                                                                                                                            
        IApplication::current ().run ();                                                                                                                                  
}                                                                                                                                                                         
