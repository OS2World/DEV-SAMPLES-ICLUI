#include "dm.hpp"

#include <iapp.hpp>
#include <ifont.hpp>
#include <ihandle.hpp>

Datei::Datei (IString const &n)
{
        FILESTATUS3 s;
        DosQueryPathInfo (n, FIL_STANDARD, &s, sizeof (s));
        FTIME t = s.ftimeLastWrite;
        FDATE d = s.fdateLastWrite;
        MD.day = d.day;
        MD.month = d.month;
        MD.year = d.year + 1980;
        MT.hours = t.hours;
        MT.minutes = t.minutes;
        MT.seconds = t.twosecs * 2;
        Groesse = s.cbFile;
        Dir = s.attrFile & FILE_DIRECTORY;

        setIcon (IPointerHandle (WinLoadFileIcon (n, FALSE)));
        Name = n;
        Name.remove (1, Name.lastIndexOf ('\\'));
        setIconText (Name);
}

Boolean DMDatei::sourceDiscard (IDMSourceDiscardEvent &e)
{
        e.setWhoDiscards (IDM::sourceDiscards);
        DosDelete (containerName () + "\\" + sourceName ());
        delete (Datei *)object ();
        return true;
}

Boolean DMDatei::sourcePrint (IDMSourcePrintEvent &e)
{
        e.setWhoPrints (IDM::targetPrints);
        return true;
}

Boolean DMDatei::sourceEnd (IDMSourceEndEvent &e)
{
	if ( e.wasTargetSuccessful () )
	{
                if ( sourceOperation ()->operation () == IDMOperation::move )
		{
			Datei *o = (Datei *)e.dragItem ()->object ();
			delete o;
		}
	}
	return true;
}

Boolean DMDatei::targetDrop (IDMTargetDropEvent &e)
{
        Verzeichnis *zv = (Verzeichnis *)e.object ();
        IDMTargetOperation *op = targetOperation ();

        IDMItem::Handle h = sourceItemFor (this);
        IDMItem *ci = (IDMItem *)h;
        IString Ziel = zv->path () + "\\" + ci->sourceName ();
        IString Quelle = ci->containerName () + "\\" + ci->sourceName ();

        if ( op->operation () == IDMOperation::copy )
        {
                DosCopy (Quelle, Ziel, 1);
        }
        if ( op->operation () == IDMOperation::move )
        {
                DosMove (Quelle, Ziel);
        }
        if ( op->operation () == IDMOperation::link )
        {
                // OS/2 Filesysteme kennen leider keine link
        }
        return true;
}

Boolean DMDatei::generateSourceItems (IDMSourceOperation *op)
{
        IDMCnrItem::generateSourceItems (op);
        for ( int i = 1; i <= op->numberOfItems (); ++i )
        {
                op->replaceItem (i, new DMDatei (op->item (i)));
        }
        return true;
}

DMDatei::DMDatei (const IDMItem::Handle &h) :
        IDMItem (h)
{
        if ( object () )
        {
		setTypes (IDM::plainText); // sollte aus EA ermitteln
                addType ("Datei");
		enableLink ();
		enableMove ();
		enableCopy ();
		setRMFs (rmfFrom (IDM::rmFile, IDM::rfText));
		addRMF (IDM::rmDiscard, IDM::rfUnknown);
		addRMF (IDM::rmPrint, IDM::rfUnknown);
                Datei *d = (Datei *)object ();
                setSourceName (d->iconText ());
                setContainerName (Struktur::aktiv ()->path ());
                setTargetName (sourceName ());
        }
}

Detail::Detail (unsigned long id, IWindow *o) :
        IContainerControl (id, o, o),
        Bild (IContainerColumn::isIcon),
        Name (offsetof (Datei, Name)),
        Groesse (offsetof (Datei, Groesse), 
                IContainerColumn::defaultHeadingStyle (),
                IContainerColumn::number),
        MD (offsetof (Datei, MD),
                IContainerColumn::defaultHeadingStyle (),
                IContainerColumn::date),
        MT (offsetof (Datei, MT),
                IContainerColumn::defaultHeadingStyle (),
                IContainerColumn::time)
{
        setDeleteObjectsOnClose ();

        unsigned long x = IFont (this).charWidth ('X');
        Name.setHeadingText ("Name").
                setDisplayWidth (256*x).
                showSeparators ();
        Groesse.setHeadingText ("Gr”áe").
                setDisplayWidth (8*x).
                showSeparators ();
        MD.setHeadingText ("Datum").
                setDisplayWidth (8*x).
                showSeparators ();
        MT.setHeadingText ("Zeit").
                setDisplayWidth (8*x).
                showSeparators ();
        addColumn (&Bild);
        addColumn (&Name);
        addColumn (&Groesse);
        addColumn (&MD);
        addColumn (&MT);
        setDetailsViewSplit (&Name, 15*x);
        showDetailsView ();
        setMultipleSelection ();
        ICnrMenuHandler::handleEventsFor (this);
        IDMHandler::defaultSourceHandler ().handleEventsFor (this);
        IDMHandler::defaultSourceHandler ().enableDragFrom (this);
        setItemProvider (this);
}

void Detail::zeige (IString const &d)
{
        disableUpdate ();
        deleteAllObjects ();
        HDIR h = HDIR_CREATE;
        FILEFINDBUF3 b;
        ULONG Attribute = FILE_DIRECTORY|FILE_SYSTEM|FILE_HIDDEN|FILE_READONLY;
        ULONG n = 1;
        APIRET e;

        e = DosFindFirst (d + "\\*.*", &h, Attribute, &b, sizeof (b), &n, FIL_STANDARD);
        while ( e == 0 )
        {
                addObject (new Datei (d + "\\" + b.achName));
                e = DosFindNext (h, &b, sizeof (b), &n);
        }
        DosFindClose (h);
        sortByIconText ();
        enableUpdate ();
}

Boolean Detail::makePopUpMenu (IMenuEvent &e)
{
        ICnrObjectSet context;

        if ( popupMenuObject () )
        {
                if ( isSelected (popupMenuObject ()) )
                {
                        IContainerControl::ObjectCursor cur (*this,
                                IContainerObject::selected);
                        forCursor (cur)
                                context.add (cur.current ());
                }
                else
                {
                        context.add (popupMenuObject ());
                }
        }
        IPopUpMenu *pm = 0;
        if ( pm )
        {
                pm->setAutoDeleteObject ();
                pm->show (e.mousePosition ());
        }
        return true;
}

Verzeichnis::Verzeichnis (IString const &n)
{
        Path = n;
        IString name = n;
        HPOINTER p = WinLoadFileIcon (n, FALSE);
        if ( p )
                setIcon (IPointerHandle (p));
        else
                setIcon (ISystemPointerHandle (ISystemPointerHandle::folder));
        setIconText (name.remove (1, Path.lastIndexOf ('\\')));
}

Struktur::Struktur (unsigned long id, IWindow *o) :
        IContainerControl (id, o, o)
{
        showTreeIconView ();
        unsigned long current, mask;
        DosQueryCurrentDisk (&current, &mask);
        unsigned long n = 0;
        for ( unsigned int bit = 1; n < 26; bit <<= 1, ++n )
        {
                if ( mask & bit )
                {
                        Verzeichnis *root = new Verzeichnis (IString ((char)('A' + n)) + ":");
                        if ( current == (n + 1) )
                                Aktiv = root;
                        addObject (root);
                        addObject (new Verzeichnis ("dummy"), root);
                }
        }
        IDMHandler::defaultTargetHandler ().handleEventsFor (this);
        IDMHandler::defaultTargetHandler ().enableDropOn (this);
        setItemProvider (this);
}

void Struktur::deleteTree (IContainerObject *o)
{
        ICnrObjectSet set = immediateDescendentsOf (o);
        ICnrObjectSet::Cursor cur (set);
        forCursor (cur)
        {
                deleteTree (cur.element ());
                delete cur.element ();
        }
}

Boolean Struktur::provideEnterSupport (IDMTargetEnterEvent &e)
{
        e.setDropIndicator (IDM::notOk);
        if ( e.object () )
        {
                IDMTargetOperation::Handle top = IDMTargetOperation::instanceFor (e);
                if ( top->item (1)->hasType ("Datei") )
                {
                        e.setDropIndicator (IDM::ok);
                }
        }
        return true;
}

DateiManager::DateiManager () :
        IFrameWindow ("Datei Manager"),
        Client (0x8008, this, this),
        Links (10, &Client),
        Rechts (20, &Client)
{
        setClient (&Client);
        Client.setSplitWindowPercentage (&Links, 30);
        Client.setSplitWindowPercentage (&Rechts, 70);
        ICnrHandler::handleEventsFor (&Links);
        ICnrHandler::handleEventsFor (&Rechts);
        Links.setSelected (Links.aktiv ());
}

Boolean DateiManager::selectedChanged (ICnrEmphasisEvent &e)
{
        IContainerControl *c = (IContainerControl *)e.controlWindow ();
        if ( c == &Links )
        {
                Verzeichnis *v = (Verzeichnis *)e.object ();
                if ( c->isSelected (v) )
                {
                        Rechts.zeige (v->path ());
                        Aktuell = v;
                }
                return true;
        }
        return false;
}

Boolean DateiManager::treeExpanded (IContainerObject *o, IContainerControl *c)
{
        disableUpdate ();
        Links.deleteTree (o);

        HDIR h = HDIR_CREATE;
        FILEFINDBUF3 b;
        ULONG Attribute = MUST_HAVE_DIRECTORY;
        ULONG n = 1;
        APIRET e;
        Verzeichnis *v = (Verzeichnis *)o;

        e = DosFindFirst (v->path () + "\\*.*", &h, Attribute, &b, sizeof (b), &n, FIL_STANDARD);
        while ( e == 0 )
        {
                if ( IString (b.achName) != "." && IString (b.achName) != ".." )
                {
                        Verzeichnis *neu = new Verzeichnis (v->path () + "\\" + b.achName);
                        c->addObject (neu, o);
                        c->addObject (new Verzeichnis ("dummy"), neu);
                }
                e = DosFindNext (h, &b, sizeof (b), &n);
        }
        DosFindClose (h);
        enableUpdate ();
        return true;
}

Boolean DateiManager::treeCollapsed (IContainerObject *o, IContainerControl *c)
{
        disableUpdate ();
        Links.deleteTree (o);
        Links.addObject (new Verzeichnis ("dummy"), o);
        enableUpdate ();
        return true;
}

Boolean DateiManager::enter (ICnrEnterEvent &e)
{
        IContainerControl *c = (IContainerControl *)e.controlWindow ();
        if ( c == &Rechts )
        {
                Datei *d = (Datei *)e.object ();
                if ( d->iconText () == "." )
                        return true;
                if ( d->iconText () == ".." )
                {
                        IContainerObject *p = Links.parentObject (Aktuell);
                        if ( p ) Links.setSelected (p);
                }
                if ( d->isDirectory () )
                {
                        Links.expand (Aktuell);
                        ICnrObjectSet set = Links.immediateDescendentsOf (Aktuell);
                        ICnrObjectSet::Cursor cur (set);
                        forCursor (cur)
                        {
                                if ( cur.element ()->iconText () == d->iconText () )
                                {
                                        Links.setSelected (cur.element ());
                                        break;
                                }
                        }
                }
        }
        return true;
}

Verzeichnis *Struktur::Aktiv = 0;

main ()
{
        DateiManager m;
        m.show ().setFocus ();
        IApplication::current ().run ();
}
