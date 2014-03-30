/**
  Wt-Tests - Testes and Error Simulation

  1) Create Folder:
  $ mkdir /tmp/wt/; cd /tmp/wt/

  2) Clone Repository:
  $ git clone git://github.com/JohnRobson/Wt-Tests.git  [or]  git clone https://github.com/JohnRobson/Wt-Tests.git

  3) Compiling:
  $ cd Wt-Tests/<SomeProject>/

  $ cmake -std=c++11 -pedantic -O0 -ggdb3 -Wall -Wextra

  $ make -j2

  4) Running:
  $ ./test.wt --docroot . --http-addr 0.0.0.0 --http-port 10100
*/

#include <iostream>
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WBreak>

using namespace std;
using namespace Wt;

class WtApplication : public WApplication { public: WtApplication(const WEnvironment& env); };

WtApplication::WtApplication(const WEnvironment& env) : WApplication(env) {
	new WText("Hello World - Simple", root()); new WBreak(root());
}

WApplication *createApplication(const WEnvironment& env) { return new WtApplication(env); }

int main(int argc, char **argv) { return WRun(argc, argv, &createApplication); }


/*
  Paste code:   http://paste.ubuntu.com/
  Paste image:  http://imagebin.org/
*/
