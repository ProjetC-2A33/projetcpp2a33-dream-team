#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>

// Forward declaration - must match your UI class name
namespace Ui {
    class mainwindow;  // This must match the <class> tag in .ui file
}

class MainWindow : public QDialog {
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
    Ui::mainwindow *ui;  // Pointer to UI class
    void refreshTable(const QString &filter = QString());
};

#endif // MAINWINDOW_H
