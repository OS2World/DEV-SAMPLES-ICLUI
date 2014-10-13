
class Termin
{
        public :
                IDate Datum;
                ITime Zeit;
                IString Aktivitaet;
                int Prioritaet;
                Boolean Erledigt;
};

inline IDate const &key (Termin* const &e)
{
        return e->Datum;
}

inline unsigned long hash (IDate const &d, unsigned long n)
{
        return d.julianDate () % n;
}

inline long compare (Termin * const &a, Termin * const &b)
{
        if ( a->Datum > b->Datum )
                return 1;
        if ( a->Datum < b->Datum )
                return -1;
        if ( a->Zeit > b->Zeit )
                return 1;
        if ( a->Zeit < b->Zeit )
                return -1;
        return 0;
}

class Terminliste : public IMultiLineEdit
{
        public :
                Terminliste (unsigned long id, IWindow *p, IWindow *o);
};

class Tag : public IMultiCellCanvas
{
        private :
                IOutlineBox Trennlinie;
                IStaticText Ueberschrift;
                IOutlineBox Rahmen;
                Terminliste Termine;
        public :
                Tag (unsigned long id, IWindow *p, IWindow *o);

                void zeige (const IDate &);
};

class Seite : public IMultiCellCanvas
{
        private :
                IOutlineBox Trenner;
                IStaticText SeitenkopfLinks;
                IStaticText SeitenkopfRechts;
                Tag Montag;
                Tag Dienstag;
                Tag Mittwoch;
                Tag Donnerstag;
                Tag Freitag;
                Tag Samstag;
                Tag Sonntag;
        public :
                Seite (unsigned long id, IWindow *p, IWindow *o);
                void zeige (const IDate &);
};

typedef IKeyBag<Termin *, IDate> Terminsammler;
typedef ISortedBagOnSortedLinkedSequence<Termin *> Tliste;

class Kalender : public IFrameWindow,
                 private ICommandHandler
{
        private :
                static Terminsammler Termine;
                Boolean command (ICommandEvent &);
                Seite Anzeige;
                IMenuBar Menu;
                IAccelerator Accelerator;
                IInfoArea Info;
                IDate sichtbar;
        public :

                static Tliste termineAmTag (const IDate &);

                Kalender ();
                ~Kalender ();
};

class DateControl : public IMultiCellCanvas
{
        private :
                ISpinButton Tag;
                ISpinButton Monat;
                ISpinButton Jahr;
        public :
                DateControl (unsigned long id, IWindow *p, IWindow *o);

                void setDate (const IDate &);
                IDate date ();
};

class TimeControl : public IMultiCellCanvas
{
        private :
                ISpinButton Stunde;
                ISpinButton Minute;
        public :
                TimeControl (unsigned long id, IWindow *p, IWindow *o);

                void setTime (const ITime &);
                ITime time ();
};

class Erfassung : public IFrameWindow,
                  private ICommandHandler
{
        private :
                Boolean command (ICommandEvent &);
                IMultiCellCanvas Client;
                IStaticText DatumText;
                IStaticText ZeitText;
                IStaticText AktivitaetText;
                DateControl Datum;
                TimeControl Zeit;
                IEntryField Aktivitaet;
                IRadioButton Prioritaet1;
                IRadioButton Prioritaet2;
                IRadioButton Prioritaet3;
                ICheckBox Erledigt;
                IPushButton OK;
                IPushButton Abbruch;
       public :
                Erfassung (IWindow *owner);

                Termin *termin ();
};

