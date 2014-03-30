#include <Wt/WApplication>
#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WException>
#include <Wt/WLogger>
#include <Wt/WServer>

class HelloApplication : public Wt::WApplication
{
public:
    HelloApplication(const Wt::WEnvironment& env, const std::string& title);

private:
    Wt::WLineEdit *nameEdit_;
    Wt::WText *greeting_;

    void greet();
};

HelloApplication::HelloApplication(const Wt::WEnvironment& env, const std::string& title)
    : Wt::WApplication(env)
{
    setTitle(title);

    root()->addWidget(new Wt::WText("Your name, please ? "));
    nameEdit_ = new Wt::WLineEdit(root());
    Wt::WPushButton *button = new Wt::WPushButton("Greet me.", root());
    root()->addWidget(new Wt::WBreak());
    greeting_ = new Wt::WText(root());
    button->clicked().connect(this, &HelloApplication::greet);
}

void HelloApplication::greet()
{    greeting_->setText("Hello there, " + nameEdit_->text());
}
class GoodbyeApplication : public Wt::WApplication{
public:
    GoodbyeApplication(const Wt::WEnvironment& env, const std::string& title);

private:    Wt::WLineEdit *nameEdit_;
    Wt::WText *greeting_;


    void greet();
};

GoodbyeApplication::GoodbyeApplication(const Wt::WEnvironment& env, const std::string& title)
    : Wt::WApplication(env)
{
    setTitle(title);

    root()->addWidget(new Wt::WText("Your name, please ? "));
    nameEdit_ = new Wt::WLineEdit(root());
    Wt::WPushButton *button = new Wt::WPushButton("Say goodbye.", root());
    root()->addWidget(new Wt::WBreak());
    greeting_ = new Wt::WText(root());
    button->clicked().connect(this, &GoodbyeApplication::greet);
}

void GoodbyeApplication::greet()
{
    greeting_->setText("Goodbye, " + nameEdit_->text());
}

Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
    return new HelloApplication(env, "First app");
}

Wt::WApplication *createSecondApplication(const Wt::WEnvironment& env)
{
    return new GoodbyeApplication(env, "Second app");
}

int YourWRun(int argc, ctwo_helloworldshar *argv[], Wt::ApplicationCreator createApplication, Wt::ApplicationCreator createSecondApplication)
{
  try {
    // use argv[0] as the application name to match a suitable entry
    // in the Wt configuration file, and use the default configuration
    // file (which defaults to /etc/wt/wt_config.xml unless the environment
    // variable WT_CONFIG_XML is set)
    Wt::WServer server(argv[0],"");

    // WTHTTP_CONFIGURATION is e.g. "/etc/wt/wthttpd"
    server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);

    // add a single entry point, at the default location (as determined
    // by the server configuration's deploy-path)
    server.addEntryPoint(Wt::Application, createApplication);
    server.addEntryPoint(Wt::Application, createSecondApplication,"/second");
    if (server.start()) {
      int sig = Wt::WServer::waitForShutdown(argv[0]);

      std::cerr << "Shutdown (signal = " << sig << ")" << std::endl;
      server.stop();

      /*
      if (sig == SIGHUP)
        WServer::restart(argc, argv, environ);
      */
      }
  } catch (Wt::WServer::Exception& e) {
    std::cerr << e.what() << "\n";
    return 1;
  } catch (std::exception& e) {
    std::cerr << "exception: " << e.what() << "\n";
    return 1;
  }
}

int main(int argc, char **argv)
{
    return YourWRun(argc, argv, &createApplication, &createSecondApplication);
}

/*
http://stackoverflow.com/questions/15956947/can-multiple-wt-applications-run-on-same-port

Compile it with

g++ -g -o two_helloworlds two_helloworlds.cc -I/usr/local/include -L/usr/local/lib -lwthttp -lwt -lboost_random -lboost_regex -lboost_signals -lboost_system -lboost_thread -lboost_filesystem -lboost_program_options -lboost_date_time

and execute with

./two_helloworlds --docroot . --http-address 0.0.0.0 --http-port 8080

on localhost:8080 you will access one of the applications and on localhost:8080/second you will access the other.
*/
