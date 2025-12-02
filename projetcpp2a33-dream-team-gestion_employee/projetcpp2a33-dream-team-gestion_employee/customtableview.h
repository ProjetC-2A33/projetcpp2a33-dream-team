#ifndef CUSTOMTABLEVIEW_H
#define CUSTOMTABLEVIEW_H

#include <QTableWidget>

class CustomTableView : public QTableWidget
{
    Q_OBJECT

public:
    explicit CustomTableView(QWidget *parent = nullptr);
};

#endif // CUSTOMTABLEVIEW_H

