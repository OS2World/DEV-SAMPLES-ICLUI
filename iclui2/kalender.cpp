#include <os2.h>

#include <ikeybag.h>
#include <isbsls.h>

#include <iapp.hpp>
#include <iframe.hpp>
#include <imcelcv.hpp>
#include <ioutlbox.hpp>
#include <istattxt.hpp>
#include <imle.hpp>
#include <ispinbt.hpp>
#include <ientryfd.hpp>
#include <iradiobt.hpp>
#include <icheckbx.hpp>
#include <ipushbut.hpp>
#include <imenubar.hpp>
#include <iaccel.hpp>
#include <iinfoa.hpp>
#include <icmdhdr.hpp>
#include <ifont.hpp>
#include <idate.hpp>
#include <itime.hpp>
#include <istring.hpp>

#include "kalender.hpp"

#include "kalender.h"

Terminliste::Terminliste (unsigned long id, IWindow *p, IWindow *o) :
        IMultiLineEdit (id, p, o)
{
        disableDataUpdate ();
        setColor (foreground, IColor::black);
        setColor (background, IColor::white);
}

Tag::Tag (unsigned long id, IWindow *p, IWindow *o) :
        IMultiCellCanvas (id, p, o),
        Trennlinie (10, this, this),
        Ueberschrift (20, this, this),
        Rahmen (10, &Ueberschrift, &Ueberschrift),
        Termine (30, this, this)
{
        addToCell (&Trennlinie, 1, 1);
        addToCell (&Ueberschrift, 1, 2);
        addToCell (&Termine, 1, 3);
        Trennlinie.setMinimumSize (ISize ());
        Termine.setMinimumSize (
                 ISize (IFont ().minTextWidth ("X") * 15,
                        IFont ().maxCharHeight () * 2));
        setColumnWidth (1, 0, true);
        setRowHeight (1, 2);
        setRowHeight (2, IFont ().maxCharHeight ());
        setRowHeight (3, 0, true);
        Rahmen.setColor (IOutlineBox::fillRegion, IColor::red);
}

void Tag::zeige (const IDate &datum)
{
        Ueberschrift.setText (IString (datum.dayOfMonth ()) + ". " + 
                                datum.dayName ());
        if ( datum.dayOfWeek () == 6 || datum.dayOfWeek () == 5 )
        {
                Ueberschrift.setColor (IStaticText::foreground, 
                        IColor::red);
        }
        if ( datum == IDate () )
        {
                Rahmen.sizeTo (Ueberschrift.displaySize ());
                Rahmen.show ();
        }
        else
        {
                Rahmen.hide ();
        }
        Tliste todo = Kalender::termineAmTag (datum);
        Tliste::Cursor tc (todo);
        IString aktivitaeten;
        for ( tc.setToFirst (); tc.isValid (); tc.setToNext (),
                                               aktivitaeten += "\n" )
        {
                aktivitaeten += tc.element ()->Zeit.asString ("%H:%M") + "\t" +
                                tc.element ()->Aktivitaet;
        }
        Termine.setText (aktivitaeten);
}

Seite::Seite (unsigned long id, IWindow *p, IWindow *o) :
        IMultiCellCanvas (id, p, o),
        SeitenkopfLinks (10, this, this),
        SeitenkopfRechts (20, this, this),
        Montag (30, this, this),
        Dienstag (40, this, this),
        Mittwoch (50, this, this),
        Donnerstag (60, this, this),
        Freitag (70, this, this),
        Samstag (80, this, this),
        Sonntag (90, this, this),
        Trenner (100, this, this)
{
        addToCell (&SeitenkopfLinks, 1, 1);
        addToCell (&SeitenkopfRechts, 3, 1);
        addToCell (&Montag, 1, 2);
        addToCell (&Dienstag, 1, 3);
        addToCell (&Mittwoch, 1, 4);
        addToCell (&Donnerstag, 1, 5);
        addToCell (&Freitag, 3, 2);
        addToCell (&Samstag, 3, 3);
        addToCell (&Sonntag, 3, 4);
        setRowHeight (1, IFont ().maxCharHeight ());
        for ( int zeile = 2; zeile <= 5; ++zeile )
                setRowHeight (zeile, 0, true);
        setColumnWidth (1, 0, true);
        setColumnWidth (3, 0, true);

        SeitenkopfRechts.setAlignment (IStaticText::topRight);

        Trenner.setMinimumSize (ISize ());
        addToCell (&Trenner, 2, 1, 1, 5);

        setColor (IGUIColor (IGUIColor::dialogBgnd));
}

void Seite::zeige (const IDate &datum)
{
        SeitenkopfLinks.setText (datum.monthName () + " " + 
                                 IString (datum.year ()));
        SeitenkopfRechts.setText (SeitenkopfLinks.text ());
        Montag.zeige (datum);
        Dienstag.zeige (datum + 1);
        Mittwoch.zeige (datum + 2);
        Donnerstag.zeige (datum + 3);
        Freitag.zeige (datum + 4);
        Samstag.zeige (datum + 5);
        Sonntag.zeige (datum + 6);
}

Terminsammler Kalender::Termine;

Kalender::Kalender () :
        IFrameWindow ("Kalender"),
        Anzeige (0x8008, this, this),
        Menu (KALENDER, this),
        Accelerator (KALENDER, this),
        Info (this)
{       
        setClient (&Anzeige);
        sichtbar -= sichtbar.dayOfWeek ();
        Anzeige.zeige (sichtbar);
        setExtensionSize (&Info, (int)IFont ().maxCharHeight ());
        Info.setColor (IStaticText::background, IGUIColor (IGUIColor::dialogBgnd));
        Info.setColor (IStaticText::fill, IGUIColor (IGUIColor::dialogBgnd));
        handleEventsFor (this);
        show ();
        setFocus ();
}

Kalender::~Kalender ()
{
        Terminsammler::Cursor tc (Termine);
        forCursor (tc)
                delete tc.element ();
}

Boolean Kalender::command (ICommandEvent &e)
{
        switch ( e.commandId () )
        {
                case MI_VOR : Anzeige.zeige (sichtbar += 7);
                        return true;
                case MI_RUECK : Anzeige.zeige (sichtbar -= 7);
                        return true;
                case MI_NEU :
                        {
                            Erfassung dlg (this);
                            if ( dlg.showModally () )
                            {
                                Termine.add (dlg.termin ());
                                Anzeige.zeige (sichtbar);                               
                            }
                        }
                        return true;
        }
        return false;
}

Tliste Kalender::termineAmTag (const IDate &tag)
{
        Tliste liste;
        Terminsammler::Cursor tc (Termine);
        if ( Termine.locateElementWithKey (tag, tc) )
        {
                do
                {
                        liste.add (tc.element ());
                } while ( Termine.locateNextElementWithKey (tag, tc) );
        }
        return liste;
}

DateControl::DateControl (unsigned long id, IWindow *p, IWindow *o) :
        IMultiCellCanvas (id, p, o),
        Tag (10, this, this,
                IRectangle (),
                ISpinButton::servant|ISpinButton::rightAlign|
                ISpinButton::numericOnly|IWindow::visible),
        Monat (20, this, this,
                IRectangle (),
                ISpinButton::servant|ISpinButton::rightAlign|
                ISpinButton::numericOnly|IWindow::visible),
        Jahr (30, this, this,
                IRectangle (),
                ISpinButton::master|ISpinButton::rightAlign|
                ISpinButton::numericOnly|IWindow::visible)
{
        Tag.setMaster (Jahr).
            setRange (IRange (1, 31)).
            setLimit (2).
            enableTabStop ();
        Monat.setMaster (Jahr).
              setRange (IRange (1, 12)).
              setLimit (2).
              enableTabStop ();
        Jahr.setRange (IRange (1900, 2100)).
             setLimit (4).
             enableTabStop ();
        addToCell (&Tag, 1, 1);
        addToCell (&Monat, 2, 1);
        addToCell (&Jahr, 3, 1);
        setDate (IDate ());
}

void DateControl::setDate (const IDate &datum)
{
        Jahr.spinTo (datum.year ());
        Monat.spinTo (datum.monthOfYear ());
        Tag.spinTo (datum.dayOfMonth ());
}

IDate DateControl::date ()
{
        return IDate (Tag.value (), IDate::Month (Monat.value ()),
                         Jahr.value ());
}


TimeControl::TimeControl (unsigned long id, IWindow *p, IWindow *o) :
        IMultiCellCanvas (id, p, o),
        Stunde (10, this, this,
                IRectangle (),
                ISpinButton::master|ISpinButton::rightAlign|
                ISpinButton::numericOnly|IWindow::visible),
        Minute (20, this, this,
                IRectangle (),
                ISpinButton::master|ISpinButton::rightAlign|
                ISpinButton::numericOnly|IWindow::visible)
{
        Minute.setRange (IRange (0, 59)).
               setLimit (2).
               enableTabStop ();
        Stunde.setRange (IRange (0, 23)).
                setLimit (2).
                enableTabStop ();
        addToCell (&Stunde, 1, 1);
        addToCell (&Minute, 2, 1);
        setTime (ITime ());
}

void TimeControl::setTime (const ITime &zeit)
{
        Minute.spinTo (zeit.minutes ());
        Stunde.spinTo (zeit.hours ());
}

ITime TimeControl::time ()
{
        return ITime (Stunde.value (), Minute.value ());
}

Erfassung::Erfassung (IWindow *owner) :
        IFrameWindow (0, IWindow::desktopWindow (), owner,
                IRectangle (),
                dialogBackground|dialogBorder|titleBar,
                "Termin erfassen"),
        Client (0x8008, this, this),
        DatumText (10, &Client, &Client),
        ZeitText (20, &Client, &Client),
        AktivitaetText (30, &Client, &Client),
        Datum (40, &Client, &Client),
        Zeit (50, &Client, &Client),
        Aktivitaet (60, &Client, &Client),
        Prioritaet1 (70, &Client, &Client),
        Prioritaet2 (80, &Client, &Client),
        Prioritaet3 (90, &Client, &Client),
        Erledigt (100, &Client, &Client),
        OK (DID_OK, &Client, &Client),
        Abbruch (DID_CANCEL, &Client, &Client)
{
        handleEventsFor (this);
        DatumText.setText ("Datum");
        ZeitText.setText ("Zeit");
        Aktivitaet.enableTabStop ();
        AktivitaetText.setText ("Aktivit„t");
        Prioritaet1.setText ("Priorit„t 1").
               enableTabStop ().
               enableGroup ();
        Prioritaet2.setText ("Priorit„t 2").
               enableTabStop ();
        Prioritaet3.setText ("Priorit„t 3").
                enableTabStop ();
        Erledigt.setText ("Erledigt").
                enableTabStop ();
        OK.enableDefault ().
                setText ("~OK").
                enableTabStop ();
        Abbruch.setText ("~Abbruch").
                enableTabStop ();
        Client.setColumnWidth (4, 0, true).
               setColumnWidth (7, IMultiCellCanvas::defaultCell ().width ()).
               setRowHeight (7, IMultiCellCanvas::defaultCell ().height ()).
               setColor (IGUIColor (IGUIColor::dialogBgnd)).
               addToCell (&DatumText, 2, 2).
               addToCell (&Datum, 3, 2, 2).
               addToCell (&ZeitText, 2, 3).
               addToCell (&Zeit, 3, 3, 2).
               addToCell (&AktivitaetText, 2, 4).
               addToCell (&Aktivitaet, 3, 4, 2).
               addToCell (&Prioritaet1, 6, 2).
               addToCell (&Prioritaet2, 6, 3).
               addToCell (&Prioritaet3, 6, 4).
               addToCell (&Erledigt, 3, 5, 2).
               addToCell (&OK, 2, 6).
               addToCell (&Abbruch, 3, 6);
        Prioritaet1.select ();
        Aktivitaet.setFocus ();

        moveSizeToClient (IRectangle (
                                owner->position () + IPoint (30,30), 
                                Client.minimumSize ()));
}

Boolean Erfassung::command (ICommandEvent &e)
{
        IFrameWindow *f = (IFrameWindow *)e.window ();
        switch ( e.commandId () )
        {
                case DID_OK : f->dismiss (true);
                        return true;
                case DID_CANCEL : f->dismiss (false);
                        return true;
        }
        return false;
}

Termin *Erfassung::termin ()
{
        Termin *t = new Termin;
        t->Datum = Datum.date ();
        t->Zeit = Zeit.time ();
        t->Aktivitaet = Aktivitaet.text ();
        t->Prioritaet = Prioritaet1.selectedIndex () + 1;
        t->Erledigt = Erledigt.isSelected ();
        return t;
}

main ()
{
        Kalender kalender;
        IApplication::current ().run ();
}

