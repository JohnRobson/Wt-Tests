#include <iostream>
#include <cmath>
#include <fstream>
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WBreak>

#include <Wt/WDate>
#include <Wt/WEnvironment>
#include <Wt/WItemDelegate>
#include <Wt/WStandardItem>
#include <Wt/WStandardItemModel>
#include <Wt/WText>

#include <Wt/WBorderLayout>
#include <Wt/WFitLayout>

#include <Wt/WTableView>

#include <Wt/Chart/WCartesianChart>
#include <Wt/Chart/WPieChart>

#include "ChartConfig.h"
#include "CsvUtil.h"

using namespace std;
using namespace Wt;
using namespace Wt::Chart;

//namespace {
WAbstractItemModel* readCsvFile(const string& fname, WContainerWidget* parent) {
  WStandardItemModel* model = new WStandardItemModel(0, 0, parent);
  ifstream f(fname.c_str());

  if(f) {
    createModel(f, model);

    for(int row = 0; row < model->rowCount(); ++row) {
      for(int col = 0; col < model->columnCount(); ++col) {
        model->item(row, col)->setFlags(ItemIsSelectable | ItemIsEditable);
      }
    }

    return model;
  } else {
    WString error(WString::tr("error-missing-data"));
    error.arg(fname, UTF8);
    new WText(error, parent);
    return 0;
  }
}
//}

void TimeSeriesExample(Wt::WContainerWidget*& parent, string rootSite) {
  WAbstractItemModel* model = readCsvFile(rootSite + "Chart/timeseries.csv", parent);

  if(!model)
    return;

  for(int i = 0; i < model->rowCount(); ++i) {
    WString s = asString(model->data(i, 0));
    WDate d = WDate::fromString(s, "dd/MM/yy");
    model->setData(i, 0, d);
  }

  // Show a view that allows editing of the model.
  /*
  WContainerWidget* w = new WContainerWidget(parent);
  WTableView* table = new WTableView(w);

  table->setMargin(10, Top | Bottom);
  table->setMargin(WLength::Auto, Left | Right);

  table->setModel(model);
  table->setSortingEnabled(false); // Does not make much sense for time series
  table->setColumnResizeEnabled(true);
  table->setSelectionMode(NoSelection);
  table->setAlternatingRowColors(true);
  table->setColumnAlignment(0, AlignCenter);
  table->setHeaderAlignment(0, AlignCenter);
  table->setRowHeight(22);

  // Editing does not really work without Ajax, it would require an
  // additional button somewhere to confirm the edited value.
  if(WApplication::instance()->environment().ajax()) {
    table->resize(800, 20 + 5 * 22);
    table->setEditTriggers(WAbstractItemView::SingleClicked);
  } else {
    table->resize(800, 20 + 5 * 22 + 25);
    table->setEditTriggers(WAbstractItemView::NoEditTrigger);
  }

  WItemDelegate* delegate = new WItemDelegate(parent);
  delegate->setTextFormat("%.1f");
  table->setItemDelegate(delegate);
  table->setItemDelegateForColumn(0, new WItemDelegate(parent));

  table->setColumnWidth(0, 80);

  for(int i = 1; i < model->columnCount(); ++i)
    table->setColumnWidth(i, 90);
  */

  WCartesianChart* chart = new WCartesianChart(parent);
  //chart->setPreferredMethod(WPaintedWidget::PngImage);
  //chart->setBackground(gray);
  chart->setModel(model);        // set the model
  chart->setXSeriesColumn(0);    // set the column that holds the X data
  chart->setLegendEnabled(true); // enable the legend
  //chart->setLegendLocation(Chart::LegendOutside, Wt::Right, Wt::AlignMiddle);
  chart->setLegendLocation(Chart::LegendInside, Wt::Right, Wt::AlignMiddle);

  chart->setType(ScatterPlot);            // set type to ScatterPlot
  chart->axis(XAxis).setScale(DateScale); // set scale of X axis to DateScale
  //chart->axis(XAxis).setVisible(false);

  // Provide space for the X and Y axis and title.
  chart->setPlotAreaPadding(40, Left);
  chart->setPlotAreaPadding(20, Top | Bottom);

  for(int i = 1; i < 8; ++i) {
    WDataSeries s(i, LineSeries);
    s.setShadow(WShadow(3, 3, WColor(0, 0, 0, 127), 3));
    chart->addSeries(s);
  }

  chart->resize(600, 300); // WPaintedWidget must be given explicit size

  chart->setMargin(10, Top | Bottom);            // add margin vertically
  chart->setMargin(WLength::Auto, Left | Right); // center horizontally

  //new ChartConfig(chart, parent);
}

class WtApplication : public WApplication {
public:
  WtApplication(const WEnvironment& env);
};

WtApplication::WtApplication(const WEnvironment& env) : WApplication(env) {
  string rootSite = appRoot();
  setTitle("Charts example");
  setCssTheme("polished");
  root()->setPadding(10);
  root()->resize(WLength::Auto, WLength::Auto);
  useStyleSheet("./Chart/charts.css");

  new WText("Charts", root());
  //new WBreak(root());

  WContainerWidget* wc = new WContainerWidget();
  TimeSeriesExample(wc, rootSite);
  root()->addWidget(wc);
}

WApplication* createApplication(const WEnvironment& env) {
  return new WtApplication(env);
}

int main(int argc, char** argv) {
  return WRun(argc, argv, &createApplication);
}
