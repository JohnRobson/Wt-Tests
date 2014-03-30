#include <iostream>
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WBreak>

using namespace std;
using namespace Wt;

class WtApplication : public WApplication { public: WtApplication(const WEnvironment& env); };

WtApplication::WtApplication(const WEnvironment& env) : WApplication(env) {
	new WText("Hello World - Emails", root()); new WBreak(root());
}

WApplication *createApplication(const WEnvironment& env) { return new WtApplication(env); }

int main(int argc, char **argv) { return WRun(argc, argv, &createApplication); }
