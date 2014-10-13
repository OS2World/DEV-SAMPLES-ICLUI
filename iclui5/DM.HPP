#define INCL_PM
#define INCL_DOS
#include <os2.h>

#include <iframe.hpp>
#include <isplitcv.hpp>

#include <icnrobj.hpp>
#include <icnrctl.hpp>
#include <icnrcol.hpp>
#include <icnrhdr.hpp>
#include <icnrolst.hpp>
#include <icnrmhdr.hpp>
#include <ipopmenu.hpp>

class Detail;

class Datei : public IContainerObject
{
        public :
                Datei (IString const &);
                Boolean isDirectory () { return Dir; }
        private :
                IString Name;
                unsigned long Groesse;
                CDATE   MD;
                CTIME   MT;
                Boolean Dir;

                friend Detail;
};

class Detail : public IContainerControl,
                public ICnrMenuHandler
{
        public :
                Detail (unsigned long id, IWindow *);

                void zeige (IString const &);
        private :
                IContainerColumn Bild;
                IContainerColumn Name;
                IContainerColumn Groesse;
                IContainerColumn MD;
                IContainerColumn MT;

                Boolean makePopUpMenu (IMenuEvent &);
};

class Verzeichnis : public IContainerObject
{
        public :
                Verzeichnis (IString const &);

                IString const &path () const { return Path; }
        private :
                IString Path;
};

class Struktur : public IContainerControl
{
        public :
                Struktur (unsigned long id, IWindow *);
                void deleteTree (IContainerObject *);

                Verzeichnis *aktiv () const { return Aktiv; }
        private :
                Verzeichnis *Aktiv;
};

class DateiManager : public IFrameWindow,
                     public ICnrHandler
{
        public :
                DateiManager ();
        private :
                ISplitCanvas Client;
                Struktur Links;
                Detail Rechts;
                Verzeichnis *Aktuell;

                Boolean selectedChanged (ICnrEmphasisEvent &);
                Boolean treeExpanded (IContainerObject *, IContainerControl *);
                Boolean treeCollapsed (IContainerObject *, IContainerControl *);
                Boolean enter (ICnrEnterEvent &);
};
