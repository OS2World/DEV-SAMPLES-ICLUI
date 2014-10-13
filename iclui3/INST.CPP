#define INCL_DOS
#define INCL_PM
#include <os2.h>
#include <iseq.h>
#include <iapp.hpp>
#include <iframe.hpp>
#include <imcelcv.hpp>
#include <istattxt.hpp>
#include <islider.hpp>
#include <istring.hpp>
#include <ipushbut.hpp>
#include <icmdhdr.hpp>
#include <imle.hpp>
#include <ientryfd.hpp>
#include <ispinbt.hpp>
#include <inotebk.hpp>
#include <imenubar.hpp>
#include <iradiobt.hpp>
#include <icheckbx.hpp>
#include <igroupbx.hpp>
#include <iselhdr.hpp>
#include <ilistbox.hpp>
#include <ithread.hpp>
#include <isubmenu.hpp>

#include "inst.hpp"

Fortschrittsanzeige::Fortschrittsanzeige (unsigned long id, IWindow *o) :
        IFrameWindow (id, IWindow::desktopWindow (), o,
                IRectangle (), titleBar|dialogBorder|dialogBackground,
                "Installationsfortschritt"),
        Client (FID_CLIENT, this, this),
        Ueberschrift (10, &Client, &Client),
        Datei (20, &Client, &Client),
        Fortschritt (30, &Client, &Client, IRectangle (), 100),
        Abbruch (DID_CANCEL, &Client, &Client)
{
        Ueberschrift.setText ("Kopiere :");
        Datei.setLimit (50);
        Abbruch.enableDefault ().
                setText ("~Abbruch");
        Fortschritt.setTickText (0, "0 %").
                    setTickText (49, "50 %").
                    setTickText (99, "100 %").
                    setShaftBreadth (25);
        for ( int i = 0; i <= 100; i += 10 )
        {
                Fortschritt.setTickLength (i > 0 ? i - 1 : 0, 4);
        }
        Client.setColumnWidth (3, 0, true).
               setColumnWidth (4, IMultiCellCanvas::defaultCell ().width ()).
               setRowHeight (4, 65).
               setRowHeight (7, IMultiCellCanvas::defaultCell ().height ()).
               addToCell (&Ueberschrift, 2, 2).
               addToCell (&Datei, 3, 2).
               addToCell (&Fortschritt, 2, 4, 2).
               addToCell (&Abbruch, 2, 6);

        setClient (&Client);
        moveSizeToClient (IRectangle (o->position () + IPoint (30,30), 
                                Client.minimumSize ()));

        ICommandHandler::handleEventsFor (this);
}

Boolean Fortschrittsanzeige::command (ICommandEvent &e)
{
        if ( e.commandId () == DID_CANCEL )
        {
                abgebrochen = true;
                hide ();
                return true;
        }
        return false;
}

void Fortschrittsanzeige::zeige (const IString &name, int prozent)
{
        abgebrochen = false;
        Datei.setText (name);
        Fortschritt.moveArmToTick (prozent - 1);
}

PfadSeite::PfadControl::PfadControl (unsigned long id, IWindow *p, const IString &text) :
        IMultiCellCanvas (id, p, p),
        Ueberschrift (10, this, this),
        Laufwerk (20, this, this),
        Verzeichnis (30, this, this)
{
        Ueberschrift.setText (text);
        Laufwerk.setLimit (1);
        ISpinButton::Cursor dummy (Laufwerk);
        IString lw = "A";
        for ( ;lw [1] <= 'Z'; ++lw [1] )
        {
                Laufwerk.addAsLast (lw, dummy);
        }
        Laufwerk.refreshText ();
        Verzeichnis.setLimit (50);
        setColumnWidth (2, 0, true);
        addToCell (&Ueberschrift, 1, 1, 2);
        addToCell (&Laufwerk, 1, 2);
        addToCell (&Verzeichnis, 2, 2);
}

IString PfadSeite::PfadControl::pfad () const
{
        return Laufwerk.text () + "\\" + Verzeichnis.text ();
}

PfadSeite::PfadSeite (unsigned long id, IWindow *p) :
        IMultiCellCanvas (id, p, p),
        Quelle (10, this, "Quelllaufwerk und Verzeichnis"),
        Ziel (20, this, "Ziellaufwerk und Verzeichnis")
{
        setRowHeight (5, defaultCell ().height ());
        setColumnWidth (3, defaultCell ().width ());
        addToCell (&Quelle, 2, 2);
        addToCell (&Ziel, 2, 4);
}


SteuerSeite::SteuerSeite (unsigned long id, IWindow *p) :
        IMultiCellCanvas (id, p, p),
        Standard (Optionen::standard, this, this),
        Minimal (Optionen::minimal, this, this),
        Benutzer (Optionen::benutzer, this, this),
        Runtime (Optionen::runtime, this, this),
        Help (Optionen::help, this, this),
        Anwendung (Optionen::anwendung, this, this),
        Grp1 (Optionen::grp1, this, this),
        Grp2 (Optionen::grp2, this, this)
{
        Grp1.setText ("Art");
        Standard.setText ("standard").
                 enableTabStop ().
                 enableGroup ();
        Minimal.setText ("minimal");
        Benutzer.setText ("Benutzer");
        Grp2.setText ("Pakete");
        Runtime.setText ("Laufzeitbibliotheken").
                enableTabStop ().
                enableGroup ();
        Help.setText ("Hilfedateien").
                enableTabStop ().
                enableGroup ();
        Anwendung.setText ("Produkt v0.99z").
                enableTabStop ().
                enableGroup ();
        addToCell (&Grp1, 2, 2, 5, 7);
        addToCell (&Standard, 4, 4);
        addToCell (&Minimal, 4, 5);
        addToCell (&Benutzer, 4, 6);
        addToCell (&Grp2, 2, 10, 5, 7);
        addToCell (&Runtime, 4, 12);
        addToCell (&Help, 4, 13);
        addToCell (&Anwendung, 4, 14);
        setRowHeight (17, defaultCell ().height ());
        setColumnWidth (7, defaultCell ().width ());
}

DateiSeite::DateiSeite (unsigned long id, IWindow *p) :
        IMultiCellCanvas (id, p, p),
        Dateien (10, this, this, IRectangle (),
                IListBox::defaultStyle ()|IListBox::multipleSelect)
{
        addToCell (&Dateien, 2, 2);
        setRowHeight (2, 0, true);
        setColumnWidth (2, 0, true);
        setRowHeight (3, defaultCell ().height ());
        setColumnWidth (3, defaultCell ().width ());
}

void DateiSeite::zeige (const Konfiguration &k)
{
        Dateien.removeAll ();
        Konfiguration::Cursor kc (k);
        forCursor (kc)
        {
                Paket::Cursor pc (kc.element ());
                forCursor (pc)
                {
                        Dateien.select (
                                Dateien.addAsLast (pc.element ()));
                }
        }       
}

Paket DateiSeite::auswahl ()
{
        Paket paket;
        IListBox::Cursor ausgewaehlt (Dateien);
        forCursor (ausgewaehlt)
                paket.add (Dateien.itemText (ausgewaehlt.asIndex ()));
        return paket;
}

Optionen::Optionen (unsigned long id, IWindow *p) :
        INotebook (id, p, p),
        Pfade (10, this),
        Steuer (20, this),
        Datei (30, this)
{
        Laufzeitbibliothek.add ("crtl.dll");
        Laufzeitbibliothek.add ("iclui.dll");
        Hilfedateien.add ("deutsch.inf");
        Hilfedateien.add ("english.inf");
        Anwendung.add ("anwendung.exe");
        Standard.add (Anwendung);
        Standard.add (Hilfedateien);
        Standard.add (Laufzeitbibliothek);
        Minimal.add (Anwendung);
        Minimal.add (Laufzeitbibliothek);

        IRectangle max;

        Seite1 = addFirstPage (PageSettings ("Pfad", "Ziel- und Quellpfadangaben", 
                        PageSettings::majorTab|
                        PageSettings::autoPageSize|
                        PageSettings::statusTextOn), &Pfade);
        max = IRectangle (IPoint (), Pfade.minimumSize ());
        Seite2 = addPageAfter (PageSettings ("Paket", "Installationsart und Pakete", 
                        PageSettings::majorTab|
                        PageSettings::autoPageSize|
                        PageSettings::statusTextOn), Seite1, &Steuer);
        max |= IRectangle (IPoint (), Steuer.minimumSize ());

        Seite3 = addPageAfter (PageSettings ("Datei", "Liste der Dateien", 
                        PageSettings::majorTab|
                        PageSettings::autoPageSize|
                        PageSettings::statusTextOn), Seite2, &Datei);

        max |= IRectangle (IPoint (), Steuer.minimumSize ());
        RECTL r = max.asRECTL ();
        sendEvent (BKM_CALCPAGERECT, &r, false);
        setMinimumSize (IRectangle (r).size ());

        
        ISelectHandler::handleEventsFor (&Steuer);
}

Boolean Optionen::selected (IControlEvent &e)
{
        switch ( e.controlId () )
        {
                case standard :
                        Steuer.Runtime.select ().disable ();
                        Steuer.Help.select ().disable ();
                        Steuer.Anwendung.select ().disable ();
                        Datei.zeige (Standard);
                        Datei.Dateien.disable ();
                        return false;
                case minimal :
                        Steuer.Runtime.select ().disable ();
                        Steuer.Help.deselect ().disable ();
                        Steuer.Anwendung.select ().disable ();
                        Datei.zeige (Minimal);
                        Datei.Dateien.disable ();
                        return false;
                case benutzer :
                        Steuer.Runtime.select ().enable ();
                        Steuer.Help.select ().enable ();
                        Steuer.Anwendung.select ().enable ();
                        Benutzer = Standard;
                        Datei.zeige (Benutzer);
                        Datei.Dateien.enable ();
                        return false;
                case runtime :
                case help :
                case anwendung :
                        Benutzer.removeAll ();
                        if ( Steuer.Runtime.isSelected () )
                        {
                                Benutzer.add (Laufzeitbibliothek);
                        }
                        if ( Steuer.Help.isSelected () )
                        {
                                Benutzer.add (Hilfedateien);
                        }
                        if ( Steuer.Anwendung.isSelected () )
                        {
                                Benutzer.add (Anwendung);
                        }
                        Datei.zeige (Benutzer);
                        return false;
        }
        return false;
}

Installation::Installation (unsigned long id, IWindow *o) :
        IFrameWindow (id, IWindow::desktopWindow (), o,
                IRectangle (), titleBar|dialogBorder|dialogBackground,
                "Installieren"),
        Client (FID_CLIENT, this, this),
        Notizbuch (10, &Client),
        Start (DID_OK, &Client, &Client),
        Abbruch (DID_CANCEL, &Client, &Client),
        Anzeige (20, o)
{
        Start.enableDefault ().
              setText ("~Start");
        Abbruch.setText ("~Abbruch");
        Client.setRowHeight (2, 0, true).
                setColumnWidth (5, 0, true).
                setRowHeight (5, IMultiCellCanvas::defaultCell ().height ()).
                setColumnWidth (6, IMultiCellCanvas::defaultCell ().width ()).
                addToCell (&Notizbuch, 2, 2, 4).
                addToCell (&Start, 2, 4).
                addToCell (&Abbruch, 4, 4);
        setClient (&Client);
        moveSizeToClient (IRectangle (o->position () + IPoint (30,30), 
                                Client.minimumSize ()));
        ICommandHandler::handleEventsFor (this);
}

Boolean Installation::command (ICommandEvent &e)
{
        switch ( e.commandId () )
        {
                case DID_OK :     dismiss (true);
                        IThread (new CopyThread (*this, 
                                Notizbuch.Datei.auswahl ()));
                        return true;
                case DID_CANCEL : dismiss (false);
                        return true;
        }
        return false;
}

void Installation::kopiere (const Paket &dateien)
{
        Paket::Cursor pc (dateien);
        int groesse = 100 / dateien.numberOfElements ();
        int prz = 0;
        Anzeige.show (); 
        forCursor (pc)
        {
                // simulation
                Anzeige.zeige (pc.element (), prz += groesse);
                DosSleep (4000);
                if ( Anzeige.istAbgebrochen () )
                        break;
        }
        Anzeige.hide ();
}

unsigned long Installation::showModally ()
{
        Notizbuch.turnToPage (Notizbuch.Seite1);
        Notizbuch.Pfade.Quelle.Laufwerk.setFocus ();
        Notizbuch.Steuer.Standard.click ();
        return IFrameWindow::showModally ();
}

Hauptfenster::Hauptfenster () :
        IFrameWindow ("Produkt v0.99z"),
        Menu (this),
        Client (FID_CLIENT, this, this),
        Install (10, this)
{
        Client.enableDataUpdate (false).
               importFromFile ("read.me");
        Client.setTop (1);
        Menu.addText (produkt, "~Produkt").
             addSubmenu (produkt);
        ISubmenu sm (Menu.menuItem (produkt).submenuHandle ());
        
        IMenuItem beenden (SC_CLOSE);
        beenden.setCommand (IMenuItem::systemCommand).
                setText ("~Beenden");
                
        sm.addText (installieren, "~Installieren ...").
           addText (entfernen, "~Entfernen ...").
           addSeparator (0).
           addItem (beenden);
        setClient (&Client);
        ICommandHandler::handleEventsFor (this);
}


Boolean Hauptfenster::command (ICommandEvent &e)
{
        if ( e.commandId () == installieren )
        {
                Install.showModally ();
        }
        return false;
}

main ()
{
        Hauptfenster f;
        f.show ();
        IApplication::current ().run ();
}
