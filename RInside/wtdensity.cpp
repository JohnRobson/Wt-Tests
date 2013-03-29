#include <iostream>
#include <cstdio>

#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WImage>
#include <Wt/WSpinBox>
#include <Wt/WGroupBox>
#include <Wt/WButtonGroup>
#include <Wt/WRadioButton>
#include <Wt/WHBoxLayout>
#include <Wt/WEnvironment>
#include <Wt/WFileResource>

#include <RInside.h>

using namespace std;
using namespace Wt;

class DensityApp : public WApplication {
public:
  DensityApp(const WEnvironment& env, RInside& R);

private:
  WLineEdit* codeEdit_; // to edit the RNG draw expression
  WButtonGroup* group_; // holds the radiobuttons
  WSpinBox* spin_; // selects the density bandwidth
  WImage* img_; // displays the image
  WFileResource* imgfile_; // controls the file resources
  WText* greeting_; // text label for status message

  void reportButton(); // called when new button selected
  void reportEdit(); // called when RNG expression edited
  void reportSpinner(); // called when bandwidth changed
  void plot(); // to call R for new plot

  enum Kernel { Gaussian = 0, Epanechnikov = 1, Rectangular = 2, Triangular = 3, Cosine = 4 };

  RInside& R_;  // reference to embedded R instance

  string tempfile_; // name of file used by R for plots
  int bw_, kernel_; // parameters used to estimate the density
  string cmd_; // random draw command string
  Rcpp::NumericVector Yvec_; // the random draw
};

// The env argument contains information about the new session, and the initial request.
// It must be passed to the WApplication // constructor so it is typically also an argument
// for your custom application constructor.
DensityApp::DensityApp(const WEnvironment& env, RInside& R) : WApplication(env), R_(R) {
  setTitle("Witty WebApp With RInside"); // application title

  setCssTheme("polished");
  messageResourceBundle().use(appRoot() + "wtdensity");

  new WText(WString::tr("overview"), root());

  string tfcmd = "tfile <- tempfile(pattern=\"img\", tmpdir=\"/tmp\", fileext=\".png\")";
  tempfile_ = Rcpp::as<string>(R_.parseEval(tfcmd)); // assign to 'tfile' in R, and report back
  bw_ = 100;
  kernel_ = 0; // parameters used to estimate the density
  cmd_ = "c(rnorm(100,0,1), rnorm(50,5,1))"; // random draw command string

  new WText(WString::tr("user input"), root());
  WContainerWidget* wc = new WContainerWidget(root());
  wc->setStyleClass("box");

  WHBoxLayout* layout = new WHBoxLayout();
  WContainerWidget* midbox = new WContainerWidget(root());
  layout->addWidget(midbox);
  WContainerWidget* container = new WContainerWidget(root());
  layout->addWidget(container);

  wc->setLayout(layout, AlignTop | AlignJustify);

  midbox->addWidget(new WText("Density estimation scale factor (div. by 100)"));
  midbox->addWidget(new WBreak()); // insert a line break
  spin_ = new WSpinBox(midbox);
  spin_->setRange(5, 200);
  spin_->setValue(bw_);
  spin_->valueChanged().connect(this, &DensityApp::reportSpinner);

  midbox->addWidget(new WBreak()); // insert a line break
  midbox->addWidget(new WText("R Command for data generation")); // show some text
  midbox->addWidget(new WBreak()); // insert a line break
  codeEdit_ = new WLineEdit(midbox); // allow text input
  codeEdit_->setTextSize(30);
  codeEdit_->setText(cmd_);
  codeEdit_->setFocus(); // give focus
  codeEdit_->enterPressed().connect(this, &DensityApp::reportEdit);

  group_ = new WButtonGroup(container); // use button group to arrange radio buttons

  WRadioButton* button;
  button = new WRadioButton("Gaussian", container);
  new WBreak(container);
  group_->addButton(button, Gaussian);

  button = new WRadioButton("Epanechnikov", container);
  new WBreak(container);
  group_->addButton(button, Epanechnikov);

  button = new WRadioButton("Rectangular", container);
  new WBreak(container);
  group_->addButton(button, Rectangular);

  button = new WRadioButton("Triangular", container);
  new WBreak(container);
  group_->addButton(button, Triangular);

  button = new WRadioButton("Cosine", container);
  new WBreak(container);
  group_->addButton(button, Cosine);

  group_->setCheckedButton(group_->button(kernel_));
  group_->checkedChanged().connect(this, &DensityApp::reportButton);

  new WText(WString::tr("r result"), root());
  WContainerWidget* botbox = new WContainerWidget(root());
  botbox->setStyleClass("box");
  imgfile_ = new WFileResource("image/png", tempfile_);
  imgfile_->suggestFileName("density.png"); // name the clients sees of datafile
  img_ = new WImage(imgfile_, "PNG version", botbox);

  new WText(WString::tr("browser info"), root());
  WContainerWidget* stbox = new WContainerWidget(root());
  stbox->setStyleClass("box");
  greeting_ = new WText(stbox); // empty text
  greeting_->setText("Setting up...");

  useStyleSheet("wtdensity.css"); // set our style sheet last

  reportEdit(); // create a new RNG draw in Yvec_
  plot(); // and draw a new density plot
}

void DensityApp::reportButton() {
  kernel_ = group_->checkedId(); // get id of selected kernel
  plot();
}

void DensityApp::reportEdit() {
  cmd_ = codeEdit_->text().toUTF8(); // get text written in box, as UTF-8, assigned to string
  string rng = "y2 <- " + cmd_ + "; y <- y2";
  R_.parseEvalQNT(rng); // evaluates expression, assigns to 'y'
  Yvec_ = R_["y"]; // cache the y vector
  plot();
}

void DensityApp::reportSpinner() {
  bw_ = spin_->value(); // get the value of the spin selector
  plot();
}

void DensityApp::plot() {
  const char* kernelstr[] = { "gaussian", "epanechnikov", "rectangular", "triangular", "cosine" };
  greeting_->setText("Starting R call");
  R_["tfile"] = tempfile_;
  R_["bw"] = bw_;
  R_["kernel"] = kernelstr[kernel_]; // passes the string to R
  R_["y"] = Yvec_;

  //string cmd0 = "png(filename=tfile,width=600,height=400); plot(density(y, bw=bw/100, kernel=kernel), xlim=range(y)+c(-2,2), main=\"Kernel: ";
  //string cmd1 = "\"); points(y, rep(0, length(y)), pch=16, col=rgb(0,0,0,1/4)); dev.off()";
  //string cmd = cmd0 + kernelstr[kernel_] + cmd1; // stick the selected kernel in the middle



  string cmd0 = "png(filename=tfile,width=600,height=400); plot(density(y, bw=bw/100, kernel=kernel), xlim=range(y)+c(-2,2), main=\"Kernel: ";
  string cmd1 = "\"); points(y, rep(0, length(y)), pch=16, col=rgb(0,0,0,1/4)); dev.off()";

  string cmd = cmd0 + kernelstr[kernel_] + cmd1; // stick the selected kernel in the middle




  R_.parseEvalQ(cmd); // evaluate command -- generates new density plot

  imgfile_->setChanged(); // important: tells consumer that image has changed, forces refresh
  greeting_->setText("Finished request from " + this->environment().clientAddress() + " using " + this->environment().userAgent()) ;
}

WApplication* createApplication(const WEnvironment& env) {
// You could read information from the environment to decide whether
// the user has permission to start a new application
//
// We grab an instance of the embedded R. Note we can start only one,
// so resource conflicts have to be managed (eg add mutexes etc)
//
  return new DensityApp(env, RInside::instance());
}

int main(int argc, char** argv) {
  RInside R(argc, argv); // create the one embedded R instance

// Your main method may set up some shared resources, but should then
// start the server application (FastCGI or httpd) that starts listening
// for requests, and handles all of the application life cycles.
//
// The last argument to WRun specifies the function that will instantiate
// new application objects. That function is executed when a new user surfs
// to the Wt application, and after the library has negotiated browser
// support. The function should return a newly instantiated application
// object.
  return WRun(argc, argv, createApplication);
}

/*
// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; tab-width: 8; -*-

rm wtdensity; g++-4.7 -Wall -I/usr/share/R/include -I/usr/local/lib/R/site-library/Rcpp/include -I/usr/local/lib/R/site-library/RInside/include -pipe -O0 -g3 -ggdb3 -DDEBUG -D_DEBUG=9 -o wtdensity wtdensity.cpp -L/usr/lib/R/lib -lR -lblas -llapack -L/usr/local/lib/R/site-library/Rcpp/lib -lRcpp -Wl,-rpath,/usr/local/lib/R/site-library/Rcpp/lib -L/usr/local/lib/R/site-library/RInside/lib -lRInside -Wl,-rpath,/usr/local/lib/R/site-library/RInside/lib -lwt -lwthttp -lboost_signals

./wtdensity --approot . --docroot . --http-addr 0.0.0.0 --http-port 10100

gdb --args wtdensity --approot . --docroot . --http-addr 0.0.0.0 --http-port 10100
*/
