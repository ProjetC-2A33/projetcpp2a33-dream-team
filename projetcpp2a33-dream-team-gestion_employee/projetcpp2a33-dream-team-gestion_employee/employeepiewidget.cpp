#include "employeepiewidget.h"

#include <QPainter>

EmployeePieWidget::EmployeePieWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumSize(260, 200);
}

void EmployeePieWidget::setData(const QVector<QPair<QString, int>> &data)
{
    m_data = data;
    update();
}

void EmployeePieWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    if (m_data.isEmpty()) {
        p.setPen(Qt::white);
        p.drawText(rect(), Qt::AlignCenter, tr("Aucune donnée"));
        return;
    }

    int total = 0;
    for (const auto &item : m_data)
        total += item.second;

    if (total <= 0) {
        p.setPen(Qt::white);
        p.drawText(rect(), Qt::AlignCenter, tr("Aucune donnée"));
        return;
    }

    // Palette de couleurs
    const QVector<QColor> colors = {
        QColor(0x4E, 0x79, 0xA7), // bleu
        QColor(0xA7, 0x4E, 0x79), // rose
        QColor(0x4E, 0xA7, 0x79), // vert
        QColor(0xA7, 0xA7, 0x4E), // jaune
        QColor(0x79, 0x4E, 0xA7), // violet
        QColor(0xA7, 0x79, 0x4E)  // orange
    };

    // Zone du camembert (en haut)
    int size = qMin(width() - 40, height() / 2 - 30);
    QRectF pieRect(10, 25, size, size);
    double startAngle = 0.0;

    // Titre
    p.setPen(Qt::white);
    p.drawText(QRectF(0, 0, width(), 20), Qt::AlignCenter,
               tr("Types d'employés"));

    // Légende sous le camembert
    int legendX = 10;
    int legendY = static_cast<int>(pieRect.bottom()) + 25;

    for (int i = 0; i < m_data.size(); ++i) {
        const auto &item = m_data.at(i);
        const QString &label = item.first;
        int value = item.second;

        double angleSpan = 360.0 * static_cast<double>(value) / static_cast<double>(total);

        QColor color = colors.at(i % colors.size());
        p.setBrush(color);
        p.setPen(Qt::NoPen);
        p.drawPie(pieRect, static_cast<int>(startAngle * 16),
                  static_cast<int>(angleSpan * 16));

        // Légende : Nom : N (xx.x %)
        double percent = (100.0 * value) / static_cast<double>(total);

        p.setBrush(Qt::NoBrush);
        p.setPen(Qt::white);

        QRect colorRect(legendX, legendY - 10, 14, 14);
        p.fillRect(colorRect, color);
        p.drawRect(colorRect);

        QString legendText = QString("%1 : %2 (%3%)")
                                 .arg(label)
                                 .arg(value)
                                 .arg(QString::number(percent, 'f', 1));

        QRect textRect(legendX + 20, legendY - 10, width() - legendX - 30, 20);
        p.drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter, legendText);

        legendY += 22;
        startAngle += angleSpan;
    }
}
