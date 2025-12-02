#include "mainwindow.h"
#include "ui_projet.h"
#include "projet.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QRegularExpressionValidator>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QDesktopServices>
#include <QUrl>
#include <QClipboard>
#include <QApplication>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPlainTextEdit>
#include <QDialogButtonBox>
#include <QDate>
#include <QRegularExpression>
#include <algorithm>
#include <QAbstractItemView>
#include <QPainter>
#include <QPair>
#include <QVector>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Input validation for ID field (lineEdit_43): non-space chars, max 20
    QRegularExpressionValidator *idValidator = new QRegularExpressionValidator(QRegularExpression("^\\S{0,20}$"), this);
    ui->lineEdit_43->setValidator(idValidator);
    ui->lineEdit_43->setMaxLength(20);

    // Input validation for Budget field (l2_17): numbers only (with decimal point)
    QDoubleValidator *budgetValidator = new QDoubleValidator(0.0, 999999999.99, 2, this);
    budgetValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->l2_17->setValidator(budgetValidator);

    // Wire buttons from .ui
    connect(ui->pushButton_60, &QPushButton::clicked, this, &MainWindow::onAdd);        // Ajouter
    connect(ui->pushButton_61, &QPushButton::clicked, this, &MainWindow::onExportPdf);  // Export PDF
    if (ui->pushButton_65) {
        connect(ui->pushButton_65, &QPushButton::clicked, this, &MainWindow::onSendEmail);
    }
    if (ui->pushButton_62) {
        connect(ui->pushButton_62, &QPushButton::clicked, this, &MainWindow::onOpenChatAssistant);
    }
    if (ui->l2_15) {
        connect(ui->l2_15, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);
    }
    if (ui->pushButton_66) {
        connect(ui->pushButton_66, &QPushButton::clicked, this, &MainWindow::onShowTypeStats);
    }
    if (ui->pushButton_63) {
        connect(ui->pushButton_63, &QPushButton::clicked, this, &MainWindow::onApplyTableEdits);
    }

    if (ui->tableWidget_2) {
        ui->tableWidget_2->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::SelectedClicked | QAbstractItemView::EditKeyPressed);
    }

    if (ui->comboBox_2) {
        ui->comboBox_2->clear();
        ui->comboBox_2->addItem("ID (A→Z)");
        ui->comboBox_2->addItem("ID (Z→A)");
        ui->comboBox_2->addItem("Email (A→Z)");
        ui->comboBox_2->addItem("Email (Z→A)");
        ui->comboBox_2->addItem("Budget (Low→High)");
        ui->comboBox_2->addItem("Budget (High→Low)");
        ui->comboBox_2->addItem("Deadline (Old→New)");
        ui->comboBox_2->addItem("Deadline (New→Old)");
        connect(ui->comboBox_2, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &MainWindow::onSortChanged);
    }

    // Optional handlers (if you add buttons later)
    // connect(ui->btnUpdate, &QPushButton::clicked, this, &MainWindow::onUpdate);
    // Wire Supprimer if present in the UI (supports multiple name variants)
    if (QPushButton* b = findChild<QPushButton*>("btnSupprimer")) {
        connect(b, &QPushButton::clicked, this, &MainWindow::onDelete);
    } else if (QPushButton* b2 = findChild<QPushButton*>("supprimer")) {
        connect(b2, &QPushButton::clicked, this, &MainWindow::onDelete);
    } else if (QPushButton* b3 = findChild<QPushButton*>("btnDelete")) {
        connect(b3, &QPushButton::clicked, this, &MainWindow::onDelete);
    } else if (QPushButton* b4 = findChild<QPushButton*>("pushButton_59")) {
        connect(b4, &QPushButton::clicked, this, &MainWindow::onDelete);
    }

    refreshTable();
}

void MainWindow::onApplyTableEdits() {
    QTableWidget *table = ui->tableWidget_2;
    int r = table ? table->currentRow() : -1;
    if (r < 0) {
        QMessageBox::warning(this, "Modifier", "Sélectionnez une ligne à modifier.");
        return;
    }
    auto idItem = table->item(r,0);
    QString originalId = idItem ? idItem->data(Qt::UserRole).toString() : QString();
    QString displayId = idItem ? idItem->text().trimmed() : QString();
    QString id = !originalId.isEmpty() ? originalId : displayId;
    auto email = table->item(r,1) ? table->item(r,1)->text().trimmed() : QString();
    auto deadlineStr = table->item(r,2) ? table->item(r,2)->text().trimmed() : QString();
    auto budgetStr = table->item(r,3) ? table->item(r,3)->text().trimmed() : QString();
    auto type = table->item(r,4) ? table->item(r,4)->text().trimmed() : QString();

    if (id.isEmpty()) {
        QMessageBox::warning(this, "Modifier", "ID_P est requis.");
        return;
    }
    QDate deadline = QDate::fromString(deadlineStr, "yyyy-MM-dd");
    if (!deadline.isValid()) {
        deadline = QDate::fromString(deadlineStr, "dd/MM/yyyy");
    }
    bool okBudget=false; double budget = budgetStr.toDouble(&okBudget);
    if (!okBudget) {
        QMessageBox::warning(this, "Modifier", "Budget invalide.");
        return;
    }
    if (!deadline.isValid()) {
        QMessageBox::warning(this, "Modifier", "Deadline invalide (yyyy-MM-dd ou dd/MM/yyyy).");
        return;
    }
    Projet p;
    p.setId(displayId);
    p.setEmail(email);
    p.setDeadline(deadline.isValid() ? deadline : QDate());
    p.setBudget(budget);
    p.setTypeProjet(type);
    bool ok = originalId.isEmpty() ? Projet::update(p) : Projet::updateById(originalId, p);
    if (ok) {
        refreshTable();
        QMessageBox::information(this, "Modifier", "Projet modifié.");
    } else {
        QMessageBox::warning(this, "Modifier", QString("Modification impossible: %1").arg(Projet::lastError()));
    }
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::refreshTable(const QString &filter) {
    Projet::fillTable(ui->tableWidget_2, Projet::fetchAll(filter));
}

void MainWindow::onAdd() {
    Projet p;
    p.setId(ui->lineEdit_43->text());              // ID_P
    p.setEmail(ui->l2_13->text());                 // E_MAIL
    p.setDeadline(ui->dateTimeEdit_13->date());    // DEADLINE
    p.setBudget(ui->l2_17->text().toDouble());     // BUDGET
    p.setTypeProjet(Projet::typeProjetFromGroupBox(ui->groupBox)); // TYPE_PROJET

    if (Projet::add(p)) {
        refreshTable();
        QMessageBox::information(this, "Succès", "Projet ajouté.");
    } else {
        QMessageBox::warning(this, "Échec", "Ajout impossible (vérifiez les champs).");
    }
}

void MainWindow::onUpdate() {
    if (ui->tableWidget_2->currentRow() < 0) return;
    auto itemId = ui->tableWidget_2->item(ui->tableWidget_2->currentRow(), 0);
    if (!itemId) return;

    Projet p;
    p.setId(itemId->text());
    p.setEmail(ui->l2_13->text());
    p.setDeadline(ui->dateTimeEdit_13->date());
    p.setBudget(ui->l2_17->text().toDouble());
    p.setTypeProjet(Projet::typeProjetFromGroupBox(ui->groupBox));

    if (Projet::update(p)) {
        refreshTable();
        QMessageBox::information(this, "Succès", "Projet modifié.");
    } else {
        QMessageBox::warning(this, "Échec", "Modification impossible.");
    }
}

void MainWindow::onDelete() {
    QString id = ui->lineEdit_43->text().trimmed();
    if (id.isEmpty()) {
        int r = ui->tableWidget_2->currentRow();
        if (r >= 0) {
            auto item = ui->tableWidget_2->item(r, 0);
            if (item) id = item->text();
        }
    }

    if (id.isEmpty()) {
        QMessageBox::warning(this, "Supprimer", "Veuillez saisir/sélectionner un ID_P.");
        return;
    }

    if (QMessageBox::question(this, "Confirmer", "Supprimer le projet ID_P = " + id + " ?") != QMessageBox::Yes) {
        return;
    }

    if (Projet::removeById(id)) {
        refreshTable();
        QMessageBox::information(this, "Supprimer", "Projet supprimé.");
    } else {
        QMessageBox::warning(this, "Supprimer", "Aucun projet trouvé avec cet ID.");
    }
}

void MainWindow::onExportPdf() {
    QString path = QFileDialog::getSaveFileName(this, "Exporter PDF", "projets.pdf", "PDF (*.pdf)");
    if (!path.isEmpty()) {
        if (Projet::exportTableToPdf(ui->tableWidget_2, path)) {
            QMessageBox::information(this, "PDF", "Export réussi.");
        } else {
            QMessageBox::warning(this, "PDF", "Échec de l'export PDF.");
        }
    }
}

void MainWindow::onRefresh() {
    refreshTable();
}

void MainWindow::onSortChanged(int index) {
    QTableWidget *t = ui->tableWidget_2;
    if (!t) return;
    int rows = t->rowCount();
    QList<int> idx; for (int r=0;r<rows;++r) idx<<r;
    auto cell = [t](int r, int c){ return t->item(r,c) ? t->item(r,c)->text() : QString(); };
    auto moveByOrder = [&](const QList<int>& order){
        struct RowData { QStringList cells; QVariant idRole; };
        QVector<RowData> data(order.size());
        for (int i=0;i<order.size();++i){
            int r=order[i];
            QStringList row;
            for (int c=0;c<t->columnCount();++c){ row<<cell(r,c);} 
            RowData rd; rd.cells=row; rd.idRole = t->item(r,0) ? t->item(r,0)->data(Qt::UserRole) : QVariant();
            data[i]=rd;
        }
        t->setRowCount(0);
        for (int i=0;i<data.size();++i){
            t->insertRow(i);
            for (int c=0;c<t->columnCount();++c){
                QTableWidgetItem *it = new QTableWidgetItem(data[i].cells[c]);
                if (c==0) { it->setData(Qt::UserRole, data[i].idRole); it->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled); }
                t->setItem(i,c,it);
            }
        }
    };
    switch (index) {
    case 0: std::sort(idx.begin(), idx.end(), [&](int a,int b){ return cell(a,0).toLower() < cell(b,0).toLower();}); break;
    case 1: std::sort(idx.begin(), idx.end(), [&](int a,int b){ return cell(a,0).toLower() > cell(b,0).toLower();}); break;
    case 2: std::sort(idx.begin(), idx.end(), [&](int a,int b){ return cell(a,1).toLower() < cell(b,1).toLower();}); break;
    case 3: std::sort(idx.begin(), idx.end(), [&](int a,int b){ return cell(a,1).toLower() > cell(b,1).toLower();}); break;
    case 4: std::sort(idx.begin(), idx.end(), [&](int a,int b){ return cell(a,3).toDouble() < cell(b,3).toDouble();}); break;
    case 5: std::sort(idx.begin(), idx.end(), [&](int a,int b){ return cell(a,3).toDouble() > cell(b,3).toDouble();}); break;
    case 6: std::sort(idx.begin(), idx.end(), [&](int a,int b){ return QDate::fromString(cell(a,2), "yyyy-MM-dd") < QDate::fromString(cell(b,2), "yyyy-MM-dd");}); break;
    case 7: std::sort(idx.begin(), idx.end(), [&](int a,int b){ return QDate::fromString(cell(a,2), "yyyy-MM-dd") > QDate::fromString(cell(b,2), "yyyy-MM-dd");}); break;
    default: return;
    }
    moveByOrder(idx);
}

void MainWindow::onSearchTextChanged(const QString &text) {
    refreshTable(text);
}

void MainWindow::onSendEmail() {
    int r = ui->tableWidget_2->currentRow();
    if (r < 0) {
        QMessageBox::warning(this, "Email", "Sélectionnez un projet dans le tableau.");
        return;
    }

    auto idItem = ui->tableWidget_2->item(r, 0);
    auto emailItem = ui->tableWidget_2->item(r, 1);
    auto deadlineItem = ui->tableWidget_2->item(r, 2);
    auto budgetItem = ui->tableWidget_2->item(r, 3);
    auto typeItem = ui->tableWidget_2->item(r, 4);
    if (!emailItem) {
        QMessageBox::warning(this, "Email", "Aucune adresse email pour ce projet.");
        return;
    }

    QString to = emailItem->text().trimmed();
    QString subject = QString("Projet %1").arg(idItem ? idItem->text() : QString());
    QString body = QString(
        "Bonjour,\n\n"
        "Voici les détails du projet:\n"
        "ID: %1\n"
        "Deadline: %2\n"
        "Budget: %3\n"
        "Type: %4\n\n"
        "Cordialement,")
        .arg(idItem ? idItem->text() : QString())
        .arg(deadlineItem ? deadlineItem->text() : QString())
        .arg(budgetItem ? budgetItem->text() : QString())
        .arg(typeItem ? typeItem->text() : QString());

    QString url = QString("https://mail.google.com/mail/?view=cm&to=%1&su=%2&body=%3")
        .arg(QString::fromLatin1(QUrl::toPercentEncoding(to)))
        .arg(QString::fromLatin1(QUrl::toPercentEncoding(subject)))
        .arg(QString::fromLatin1(QUrl::toPercentEncoding(body)));

    QDesktopServices::openUrl(QUrl(url));
}

void MainWindow::onOpenChatAssistant() {
    QTableWidget *table = ui->tableWidget_2;
    if (!table) return;

    QString csv;
    int rows = table->rowCount();
    int cols = table->columnCount();
    QStringList headers;
    for (int c = 0; c < cols; ++c) {
        headers << (table->horizontalHeaderItem(c) ? table->horizontalHeaderItem(c)->text() : QString("Col%1").arg(c+1));
    }
    csv += headers.join(',') + "\n";
    for (int r = 0; r < rows; ++r) {
        QStringList cells;
        for (int c = 0; c < cols; ++c) {
            QString t = table->item(r, c) ? table->item(r, c)->text() : QString();
            QString escaped = t;
            escaped.replace('"', "\"");
            if (escaped.contains(',') || escaped.contains('\n')) {
                escaped = '"' + escaped + '"';
            }
            cells << escaped;
        }
        csv += cells.join(',') + "\n";
    }
    QApplication::clipboard()->setText(csv);

    QDialog dlg(this);
    dlg.setWindowTitle("Chat Assistant");
    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    QPlainTextEdit *history = new QPlainTextEdit(&dlg);
    history->setReadOnly(true);
    layout->addWidget(history);
    QHBoxLayout *inputRow = new QHBoxLayout();
    QLineEdit *input = new QLineEdit(&dlg);
    input->setPlaceholderText("Posez une question (ex: budget > 1000)");
    QPushButton *send = new QPushButton("Envoyer", &dlg);
    inputRow->addWidget(input);
    inputRow->addWidget(send);
    layout->addLayout(inputRow);

    auto answerFromTable = [table](const QString &q) {
        QString qt = q.trimmed().toLower();
        QStringList results;
        int rows = table->rowCount();
        auto cell = [table](int r, int c){ return table->item(r,c) ? table->item(r,c)->text() : QString(); };
        QString qtNoQ = qt;
        qtNoQ.replace('?', ' ');
        qtNoQ = qtNoQ.trimmed();
        auto intsFrom = [](const QString &s){
            QRegularExpression re("(\\d+)");
            auto it = re.globalMatch(s);
            QList<int> ns; while (it.hasNext()) { ns << it.next().captured(1).toInt(); } return ns; };
        auto doublesFrom = [](const QString &s){
            QRegularExpression re("(\\d+(?:\\.\\d+)?)");
            auto it = re.globalMatch(s);
            QList<double> ns; while (it.hasNext()) { ns << it.next().captured(1).toDouble(); } return ns; };
        auto dateFromAny = [](const QString &s){ QDate d = QDate::fromString(s, "yyyy-MM-dd"); if (!d.isValid()) d = QDate::fromString(s, "dd/MM/yyyy"); return d; };
        auto cmpBudgetAsc = [cell](int a, int b){ return cell(a,3).toDouble() < cell(b,3).toDouble(); };
        auto cmpBudgetDesc = [cell](int a, int b){ return cell(a,3).toDouble() > cell(b,3).toDouble(); };
        if (qt.startsWith("budget >")) {
            bool ok=false; double v = qt.mid(8).trimmed().toDouble(&ok);
            if (ok) {
                for (int r=0;r<rows;++r) {
                    double b = cell(r,3).toDouble();
                    if (b>v) results << QString("ID %1, Budget %2").arg(cell(r,0)).arg(b);
                }
            }
        } else if (qt.startsWith("budget <") || qtNoQ.contains("under") || qtNoQ.contains("below") || qtNoQ.contains("moins de") || qtNoQ.contains("inférieur à") || qtNoQ.contains("inferieur a") || qtNoQ.contains("budjet") || qtNoQ.contains("bugdet") || qtNoQ.contains("budg")) {
            bool ok=false; double v = qt.mid(8).trimmed().toDouble(&ok);
            if (!ok) {
                auto nums = doublesFrom(qtNoQ); if (!nums.isEmpty()) { v = nums.first(); ok = true; }
            }
            if (ok) {
                for (int r=0;r<rows;++r) {
                    double b = cell(r,3).toDouble();
                    if (b<v) {
                        bool wantIds = qtNoQ.contains("id") || qtNoQ.contains("ids") || qtNoQ.contains("liste");
                        if (wantIds) results << cell(r,0); else results << QString("ID %1, Budget %2").arg(cell(r,0)).arg(b);
                    }
                }
            }
        } else if (qtNoQ.contains("budget") && (qtNoQ.contains("between") || qtNoQ.contains("entre"))) {
            QRegularExpression re("(\\d+(?:\\.\\d+)?)");
            auto it = re.globalMatch(qtNoQ);
            QList<double> nums;
            while (it.hasNext()) {
                auto m = it.next();
                nums << m.captured(1).toDouble();
            }
            if (nums.size() >= 2) {
                double a = qMin(nums[0], nums[1]);
                double b2 = qMax(nums[0], nums[1]);
                bool wantIds = qtNoQ.contains("id") || qtNoQ.contains("ids");
                if (wantIds) {
                    for (int r=0;r<rows;++r) {
                        double b = cell(r,3).toDouble();
                        if (b>=a && b<=b2) results << cell(r,0);
                    }
                } else {
                    int count = 0;
                    for (int r=0;r<rows;++r) {
                        double b = cell(r,3).toDouble();
                        if (b>=a && b<=b2) count++;
                    }
                    results << QString::number(count);
                }
            }
        } else if (qt.startsWith("deadline before")) {
            QString d = qt.mid(QString("deadline before").length()).trimmed();
            QDate limit = QDate::fromString(d, "yyyy-MM-dd");
            if (limit.isValid()) {
                for (int r=0;r<rows;++r) {
                    QDate dd = QDate::fromString(cell(r,2), "yyyy-MM-dd");
                    if (dd.isValid() && dd<limit) results << QString("ID %1, Deadline %2").arg(cell(r,0)).arg(dd.toString("yyyy-MM-dd"));
                }
            }
        } else if (qt.startsWith("deadline after")) {
            QString d = qt.mid(QString("deadline after").length()).trimmed();
            QDate limit = QDate::fromString(d, "yyyy-MM-dd");
            if (limit.isValid()) {
                for (int r=0;r<rows;++r) {
                    QDate dd = QDate::fromString(cell(r,2), "yyyy-MM-dd");
                    if (dd.isValid() && dd>limit) results << QString("ID %1, Deadline %2").arg(cell(r,0)).arg(dd.toString("yyyy-MM-dd"));
                }
            }
        } else if (((qtNoQ.contains("how many") || qtNoQ.contains("ids") || qtNoQ.contains("id")) && qtNoQ.contains("deadline") && qtNoQ.contains("this year")) ||
                   (qtNoQ.contains("deadline") && (qtNoQ.contains("cette annee") || qtNoQ.contains("cette année")))) {
            int year = QDate::currentDate().year();
            bool wantIds = qtNoQ.contains("id") || qtNoQ.contains("ids") || qtNoQ.contains("liste");
            if (wantIds) {
                for (int r=0;r<rows;++r) {
                    QDate dd = QDate::fromString(cell(r,2), "yyyy-MM-dd");
                    if (dd.isValid() && dd.year()==year) results << cell(r,0);
                }
            } else {
                int count = 0;
                for (int r=0;r<rows;++r) {
                    QDate dd = QDate::fromString(cell(r,2), "yyyy-MM-dd");
                    if (dd.isValid() && dd.year()==year) count++;
                }
                results << QString::number(count);
            }
        } else if ((qtNoQ.contains("how many") && (qtNoQ.contains("deadline")||qtNoQ.contains("end")) && qtNoQ.contains("this month")) ||
                   (qtNoQ.contains("combien") && (qtNoQ.contains("deadline")||qtNoQ.contains("fin")) && (qtNoQ.contains("ce mois")))) {
            QDate now = QDate::currentDate();
            int count = 0;
            for (int r=0;r<rows;++r) {
                QDate dd = QDate::fromString(cell(r,2), "yyyy-MM-dd");
                if (dd.isValid() && dd.year()==now.year() && dd.month()==now.month()) count++;
            }
            results << QString::number(count);
        } else if (qtNoQ.contains("email") || qtNoQ.contains("emil") || qtNoQ.contains('@')) {
            QRegularExpression reAny("([A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+)");
            auto m = reAny.match(qtNoQ);
            if (m.hasMatch()) {
                QString target = m.captured(1).toLower();
                for (int r=0;r<rows;++r) {
                    QString e = cell(r,1).toLower();
                    if (e == target) results << cell(r,0);
                }
            }
        } else if ((qtNoQ.contains("deadline") || qtNoQ.contains("end") || qtNoQ.contains("finish")) && (qtNoQ.contains("on") || qtNoQ.contains("equals") || qtNoQ.contains("= ") || qtNoQ.contains("le "))) {
            QRegularExpression dr("(\\d{4}-\\d{2}-\\d{2}|\\d{2}/\\d{2}/\\d{4})");
            auto m = dr.match(qtNoQ);
            if (m.hasMatch()) {
                QDate d = QDate::fromString(m.captured(1), "yyyy-MM-dd");
                if (!d.isValid()) d = QDate::fromString(m.captured(1), "dd/MM/yyyy");
                for (int r=0;r<rows;++r) {
                    QDate dd = QDate::fromString(cell(r,2), "yyyy-MM-dd");
                    if (dd.isValid() && dd == d) results << cell(r,0);
                }
            }
        } else if ((qtNoQ.contains("how many") && qtNoQ.contains("type")) ||
                   (qtNoQ.contains("combien") && qtNoQ.contains("type"))) {
            QString key;
            if (qtNoQ.contains("type")) {
                int idx = qtNoQ.indexOf("type");
                key = qtNoQ.mid(idx+4).trimmed();
            }
            int count = 0;
            for (int r=0;r<rows;++r) {
                QString t = cell(r,4).toLower();
                if (key.isEmpty()) {
                    if (!t.isEmpty()) count++;
                } else if (t.contains(key)) {
                    count++;
                }
            }
            results << QString::number(count);
        } else if (qtNoQ.contains("sum") && qtNoQ.contains("budget")) {
            double s = 0.0;
            for (int r=0;r<rows;++r) s += cell(r,3).toDouble();
            results << QString::number(s, 'f', 2);
        } else if ((qtNoQ.contains("average") && qtNoQ.contains("budget")) ||
                   (qtNoQ.contains("moyenne") && qtNoQ.contains("budget"))) {
            double s = 0.0; int n=0;
            for (int r=0;r<rows;++r) { s += cell(r,3).toDouble(); n++; }
            double avg = n? s/n : 0.0;
            results << QString::number(avg, 'f', 2);
        } else if (qtNoQ.contains("overdue") || qtNoQ.contains("en retard")) {
            QDate today = QDate::currentDate();
            for (int r=0;r<rows;++r) {
                QDate dd = QDate::fromString(cell(r,2), "yyyy-MM-dd");
                if (dd.isValid() && dd < today) results << QString("ID %1, Deadline %2").arg(cell(r,0)).arg(dd.toString("yyyy-MM-dd"));
            }
        } else if (qtNoQ.contains("upcoming") || qtNoQ.contains("dans") && qtNoQ.contains("jours")) {
            int n = 7; auto ns = intsFrom(qtNoQ); if (!ns.isEmpty()) n = ns[0];
            QDate today = QDate::currentDate(); QDate end = today.addDays(n);
            for (int r=0;r<rows;++r) {
                QDate dd = QDate::fromString(cell(r,2), "yyyy-MM-dd");
                if (dd.isValid() && dd >= today && dd <= end) results << QString("ID %1, Deadline %2").arg(cell(r,0)).arg(dd.toString("yyyy-MM-dd"));
            }
        } else if (qtNoQ.contains("next deadline") || qtNoQ.contains("prochaine deadline")) {
            QDate today = QDate::currentDate(); QDate best; QString bestId;
            for (int r=0;r<rows;++r) {
                QDate dd = QDate::fromString(cell(r,2), "yyyy-MM-dd");
                if (dd.isValid() && dd >= today && (!best.isValid() || dd < best)) { best = dd; bestId = cell(r,0); }
            }
            if (best.isValid()) results << QString("ID %1, Deadline %2").arg(bestId).arg(best.toString("yyyy-MM-dd"));
        } else if (qtNoQ.contains("top") && qtNoQ.contains("budget")) {
            int n = 3; auto ns = intsFrom(qtNoQ); if (!ns.isEmpty()) n = ns[0];
            QList<int> idx; for (int r=0;r<rows;++r) idx<<r; std::sort(idx.begin(), idx.end(), cmpBudgetDesc);
            for (int i=0;i<qMin(n, idx.size()); ++i) { int r=idx[i]; results<<QString("ID %1, Budget %2").arg(cell(r,0)).arg(cell(r,3)); }
        } else if (qtNoQ.contains("bottom") && qtNoQ.contains("budget") || qtNoQ.contains("plus petits budgets")) {
            int n = 3; auto ns = intsFrom(qtNoQ); if (!ns.isEmpty()) n = ns[0];
            QList<int> idx; for (int r=0;r<rows;++r) idx<<r; std::sort(idx.begin(), idx.end(), cmpBudgetAsc);
            for (int i=0;i<qMin(n, idx.size()); ++i) { int r=idx[i]; results<<QString("ID %1, Budget %2").arg(cell(r,0)).arg(cell(r,3)); }
        } else if (qtNoQ.contains("min budget") || qtNoQ.contains("minimum budget")) {
            double minV = std::numeric_limits<double>::infinity(); QString minId;
            for (int r=0;r<rows;++r){ double b=cell(r,3).toDouble(); if (b<minV){minV=b;minId=cell(r,0);} }
            if (std::isfinite(minV)) results<<QString("ID %1, Budget %2").arg(minId).arg(minV);
        } else if (qtNoQ.contains("max budget") || qtNoQ.contains("maximum budget")) {
            double maxV = -std::numeric_limits<double>::infinity(); QString maxId;
            for (int r=0;r<rows;++r){ double b=cell(r,3).toDouble(); if (b>maxV){maxV=b;maxId=cell(r,0);} }
            if (std::isfinite(maxV)) results<<QString("ID %1, Budget %2").arg(maxId).arg(maxV);
        } else if (qtNoQ.contains("median budget") || qtNoQ.contains("médiane budget") || qtNoQ.contains("mediane budget")) {
            QList<double> bs; for (int r=0;r<rows;++r) bs<<cell(r,3).toDouble(); std::sort(bs.begin(), bs.end());
            double med=0.0; if (!bs.isEmpty()){int n=bs.size(); med = n%2? bs[n/2] : (bs[n/2-1]+bs[n/2])/2.0;} results<<QString::number(med,'f',2);
        } else if (qtNoQ.contains("list") && qtNoQ.contains("budgets") && qtNoQ.contains("ascending")) {
            QList<int> idx; for (int r=0;r<rows;++r) idx<<r; std::sort(idx.begin(), idx.end(), cmpBudgetAsc);
            for (int r: idx) results<<QString("ID %1, Budget %2").arg(cell(r,0)).arg(cell(r,3));
        } else if (qtNoQ.contains("list") && qtNoQ.contains("budgets") && qtNoQ.contains("descending")) {
            QList<int> idx; for (int r=0;r<rows;++r) idx<<r; std::sort(idx.begin(), idx.end(), cmpBudgetDesc);
            for (int r: idx) results<<QString("ID %1, Budget %2").arg(cell(r,0)).arg(cell(r,3));
        } else if (qtNoQ.contains("deadline between") || qtNoQ.contains("deadline entre")) {
            auto ds = doublesFrom(qtNoQ); Q_UNUSED(ds);
            QRegularExpression dr("(\\d{4}-\\d{2}-\\d{2}|\\d{2}/\\d{2}/\\d{4})"); auto it=dr.globalMatch(qtNoQ);
            QList<QDate> dates; while (it.hasNext()){ dates<<dateFromAny(it.next().captured(1)); }
            if (dates.size()>=2){ QDate a=dates[0]; QDate b=dates[1]; if (a>b) std::swap(a,b);
                for (int r=0;r<rows;++r){ QDate d=dateFromAny(cell(r,2)); if (d.isValid() && d>=a && d<=b) results<<QString("ID %1, Deadline %2").arg(cell(r,0)).arg(d.toString("yyyy-MM-dd")); }
            }
        } else if (qt.startsWith("id contains")) {
            QString k = qt.mid(QString("id contains").length()).trimmed();
            for (int r=0;r<rows;++r) {
                if (cell(r,0).toLower().contains(k)) results << QString("ID %1").arg(cell(r,0));
            }
        } else if (qt.startsWith("email contains")) {
            QString k = qt.mid(QString("email contains").length()).trimmed();
            for (int r=0;r<rows;++r) {
                if (cell(r,1).toLower().contains(k)) results << QString("ID %1, Email %2").arg(cell(r,0)).arg(cell(r,1));
            }
        } else if (qtNoQ.contains("count by domain") || qtNoQ.contains("par domaine")) {
            QMap<QString,int> m; for (int r=0;r<rows;++r){ QString e=cell(r,1); int at=e.indexOf('@'); if (at!=-1){ QString d=e.mid(at+1).toLower(); m[d]++; } }
            for (auto it=m.constBegin(); it!=m.constEnd(); ++it) results<<QString("%1: %2").arg(it.key()).arg(it.value());
        } else if (qtNoQ.contains("list type") || qtNoQ.contains("liste type")) {
            QString key; {
                int idx = qtNoQ.indexOf("type"); if (idx!=-1) key = qtNoQ.mid(idx+4).trimmed();
            }
            for (int r=0;r<rows;++r) {
                QString t = cell(r,4).toLower(); if (key.isEmpty() || t.contains(key)) results<<QString("ID %1, Type %2").arg(cell(r,0)).arg(cell(r,4));
            }
        } else {
            for (int r=0;r<rows;++r) {
                results << QString("ID %1, Email %2, Deadline %3, Budget %4, Type %5")
                            .arg(cell(r,0)).arg(cell(r,1)).arg(cell(r,2)).arg(cell(r,3)).arg(cell(r,4));
            }
        }
        return results.isEmpty() ? QString("Aucun résultat.") : results.join("\n");
    };

    QObject::connect(send, &QPushButton::clicked, &dlg, [this, table, history, input, answerFromTable]() {
        QString q = input->text().trimmed();
        if (q.isEmpty()) return;
        history->appendPlainText("Vous: " + q);
        QString ans = answerFromTable(q);
        history->appendPlainText("Assistant: \n" + ans + "\n");
        QString ql = q.trimmed().toLower();
        if (ql.contains("sort") || ql.contains("trier")) {
            if (ql.contains("id") && ql.contains("asc")) onSortChanged(0);
            else if (ql.contains("id") && (ql.contains("desc") || ql.contains("decroissant") || ql.contains("décroissant"))) onSortChanged(1);
            else if (ql.contains("email") && ql.contains("asc")) onSortChanged(2);
            else if (ql.contains("email") && (ql.contains("desc") || ql.contains("decroissant") || ql.contains("décroissant"))) onSortChanged(3);
            else if ((ql.contains("budget") && (ql.contains("low") || ql.contains("croissant")))) onSortChanged(4);
            else if ((ql.contains("budget") && (ql.contains("high") || ql.contains("decroissant") || ql.contains("décroissant")))) onSortChanged(5);
            else if ((ql.contains("deadline") && (ql.contains("old") || ql.contains("asc") || ql.contains("ancien")))) onSortChanged(6);
            else if ((ql.contains("deadline") && (ql.contains("new") || ql.contains("desc") || ql.contains("récent") || ql.contains("recent")))) onSortChanged(7);
        }
        input->clear();
    });

    dlg.resize(600, 400);
    dlg.exec();
}

class PieWidget : public QWidget {
public:
    explicit PieWidget(const QVector<QPair<QString,double>> &data, QWidget *parent=nullptr)
        : QWidget(parent), m_data(data) {}
protected:
    void paintEvent(QPaintEvent *) override {
        QPainter p(this);
        p.setRenderHint(QPainter::Antialiasing, true);
        QRectF rect(20, 20, width()-200, height()-40);
        double total = 0.0;
        for (auto &kv : m_data) total += kv.second;
        if (total <= 0.0) {
            p.drawText(rect, Qt::AlignCenter, "Aucune donnée");
            return;
        }
        QList<QColor> colors = {QColor(255,99,132), QColor(54,162,235), QColor(255,206,86), QColor(75,192,192), QColor(153,102,255), QColor(255,159,64)};
        int ci = 0;
        int start = 0;
        int legendY = 20;
        for (auto &kv : m_data) {
            double spanDeg = 360.0 * (kv.second / total);
            int span = int(spanDeg * 16);
            QColor col = colors[ci % colors.size()];
            p.setBrush(col);
            p.setPen(Qt::NoPen);
            p.drawPie(rect, start, span);
            p.setPen(Qt::black);
            p.setBrush(Qt::NoBrush);
            p.fillRect(width()-160, legendY, 20, 20, col);
            p.drawRect(width()-160, legendY, 20, 20);
            p.drawText(width()-135, legendY+15, QString("%1 (%2)").arg(kv.first).arg(kv.second));
            legendY += 26;
            start += span;
            ci++;
        }
    }
private:
    QVector<QPair<QString,double>> m_data;
};

void MainWindow::onShowTypeStats() {
    QTableWidget *table = ui->tableWidget_2;
    if (!table) return;
    QMap<QString,int> counts;
    int rows = table->rowCount();
    for (int r=0; r<rows; ++r) {
        QString t = table->item(r,4) ? table->item(r,4)->text() : QString();
        for (const QString &part : t.split(',', Qt::SkipEmptyParts)) {
            QString k = part.trimmed();
            if (!k.isEmpty()) counts[k]++;
        }
    }
    QVector<QPair<QString,double>> data;
    for (auto it = counts.constBegin(); it != counts.constEnd(); ++it) {
        data.append(qMakePair(it.key(), double(it.value())));
    }
    QDialog dlg(this);
    dlg.setWindowTitle("Statistiques Type de Projet");
    QVBoxLayout *layout = new QVBoxLayout(&dlg);
    PieWidget *pie = new PieWidget(data, &dlg);
    layout->addWidget(pie);
    dlg.resize(700, 450);
    dlg.exec();
}
