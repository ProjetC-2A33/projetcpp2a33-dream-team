#ifndef EMPLOYEEPIEWIDGET_H
#define EMPLOYEEPIEWIDGET_H

#include <QWidget>
#include <QVector>
#include <QPair>
#include <QString>

class EmployeePieWidget : public QWidget
{
    Q_OBJECT
public:
    explicit EmployeePieWidget(QWidget *parent = nullptr);

    void setData(const QVector<QPair<QString, int>> &data);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QVector<QPair<QString, int>> m_data;
};

#endif // EMPLOYEEPIEWIDGET_H
