#ifndef STRUCTUREMENU_H
#define STRUCTUREMENU_H

#include <QMenu>

class StructureMenu : public QMenu
{
    Q_OBJECT

    public:
        explicit StructureMenu(QWidget *parent = 0);
        void setGotoVisible(bool b);

    signals:
        void gotoStartAction();
        void gotoEndAction();
        void exportAction();

    private:
        QMenu* _gotomenu;
        QAction* _actgotostart;
        QAction* _actgotoend;
        QAction* _actexport;
};

#endif // STRUCTUREMENU_H
