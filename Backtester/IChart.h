#pragma once

class IChart {

};

enum class ChartEventType {
    CHART_START,
    CAND_CLOSE,
    CAND_OPEN,
    CHART_FINISH
};