
class Fortschrittsanzeige : public IFrameWindow,
                            public ICommandHandler
{
        private :
                IMultiCellCanvas Client;
                IStaticText Ueberschrift;
                IStaticText Datei;
                IProgressIndicator Fortschritt;
                IPushButton Abbruch;
                Boolean abgebrochen;

                Boolean command (ICommandEvent &);
        public :
                void zeige (const IString &datei, int prozent);
                Boolean istAbgebrochen () const { return abgebrochen; }

                Fortschrittsanzeige (unsigned long id, IWindow *o);
};

typedef ISequence<IString> Paket;
typedef ISequence<Paket> Konfiguration;


class PfadSeite : public IMultiCellCanvas
{
        private :
                class PfadControl : public IMultiCellCanvas
                {
                        public :
                                IStaticText Ueberschrift;
                                ISpinButton Laufwerk;
                                IEntryField Verzeichnis;

                                PfadControl (unsigned long id, IWindow *p,
                                        const IString &text);
                                IString pfad () const;
                };
        public :
                PfadControl Quelle;
                PfadControl Ziel;
                PfadSeite (unsigned long id, IWindow *p);
};

class SteuerSeite : public IMultiCellCanvas
{
        public :
                IGroupBox Grp1;
                IRadioButton Standard;
                IRadioButton Minimal;
                IRadioButton Benutzer;
                IGroupBox Grp2;
                ICheckBox Runtime;
                ICheckBox Help;
                ICheckBox Anwendung;
                SteuerSeite (unsigned long id, IWindow *p);
};

class DateiSeite : public IMultiCellCanvas
{
        public :
                IListBox Dateien;
                DateiSeite (unsigned long id, IWindow *p);

                void zeige (const Konfiguration &);
                Paket auswahl ();
};

class Optionen : public INotebook,
                 public ISelectHandler
{
                Boolean selected (IControlEvent &);
        public :
                enum Id
                {
                        standard = 10,
                        minimal,
                        benutzer,
                        runtime,
                        help,
                        anwendung,
                        grp1,
                        grp2
                };
                PfadSeite Pfade;
                SteuerSeite Steuer;
                DateiSeite Datei;
                IPageHandle Seite1;
                IPageHandle Seite2;
                IPageHandle Seite3;

                Paket Laufzeitbibliothek;
                Paket Hilfedateien;
                Paket Anwendung;
                Konfiguration Standard;
                Konfiguration Minimal;
                Konfiguration Benutzer;

                Optionen (unsigned long id, IWindow *p);
};

class Installation : public IFrameWindow,
                     public ICommandHandler
{
        private :
                IMultiCellCanvas Client;
                Optionen Notizbuch;
                IPushButton Start;
                IPushButton Abbruch;
                Fortschrittsanzeige Anzeige;
                class CopyThread : public IThreadFn
                {
                        private :
                                Installation &inst;
                                Paket dateien;
                        public :
                                CopyThread (Installation &i, 
                                        const Paket &p) : 
                                        inst (i) { dateien = p; }
                                void run ()
                                { inst.kopiere (dateien); }
                };
                friend CopyThread;

                void kopiere (const Paket &dateien);
                Boolean command (ICommandEvent &);
        public :
                Installation (unsigned long id, IWindow *o);

                unsigned long showModally ();
};

class Hauptfenster : public IFrameWindow,
                     public ICommandHandler
{
        private :
                enum kommandos
                {
                        produkt = 1000,
                        installieren,
                        entfernen
                };
                IMenuBar Menu;
                IMultiLineEdit Client;
                Installation Install;

                Boolean command (ICommandEvent &);
        public:
                Hauptfenster ();
};

