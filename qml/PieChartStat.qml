import QtQuick 2.15
import QtQuick.Controls 2.15
import QtCharts 2.15

Item {
    id: root
    anchors.fill: parent
    property var platformStatsModel: []
    // Mixed purple palette ranging from light lavender to deep plum
    property var purplePalette: [
        "#F4E3FF", "#E0BBE4", "#C79FDA", "#AD82CE",
        "#8F5FBF", "#7030A0", "#5A1A88", "#3C0D5C"
    ]

    function refreshSeries(newData) {
        platformStatsModel = newData
        rebuildSeries()
    }

    function rebuildSeries() {
        pieSeries.clear()
        var total = 0
        for (var j = 0; j < platformStatsModel.length; ++j) {
            total += Number(platformStatsModel[j].value) || 0
        }

        for (var i = 0; i < platformStatsModel.length; ++i) {
            var entry = platformStatsModel[i]
            var value = Number(entry.value) || 0
            var slice = pieSeries.append(entry.label || "N/A", value)
            var percentage = total > 0 ? Math.round((value / total) * 1000) / 10 : 0
            slice.label = (entry.label || "N/A") + " (" + value + " | " + percentage + "%)"
            slice.color = purplePalette[i % purplePalette.length]
            slice.labelPosition = PieSlice.LabelOutside
            slice.labelVisible = true
        }
    }

    ChartView {
        id: chartView
        anchors.fill: parent
        antialiasing: true
        legend.alignment: Qt.AlignBottom
        title: qsTr("Créateurs par plateforme")

        PieSeries {
            id: pieSeries
        }
    }

    Component.onCompleted: rebuildSeries()
}

