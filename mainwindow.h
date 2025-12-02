#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onAdd();
    void onExportPdf();
    void onUpdate();
    void onDelete();
    void onRefresh();
    void onSearchTextChanged(const QString &text);
    void onSendEmail();
    void onOpenChatAssistant();
    void onShowTypeStats();
    void onApplyTableEdits();
    void onSortChanged(int index);

private:
    Ui::MainWindow *ui;
    void refreshTable(const QString &filter = QString());
};

#endif // MAINWINDOW_H


