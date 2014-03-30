/*
 * LER:  http://stackoverflow.com/questions/347949/convert-stdstring-to-const-char-or-char
 */

#include <iostream>
#include <fstream>
//#include <boost/property_tree/detail/rapidxml.hpp>
#include "rapidxml/rapidxml.hpp"
#include "rapidxml/rapidxml_print.hpp"
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WText>
#include <Wt/WBreak>

using namespace std;
using namespace Wt;
using namespace rapidxml;

class WtApplication : public WApplication {
	private:
		void test1();
		void test2();
		void test3();
		void test4();
		void test5();
		void test6();
		void test7();
		xml_node<>* get_child(xml_node<> *inputNode, string sNodeFilter);

	public:
		WtApplication(const WEnvironment& env);
};

void WtApplication::test1() {
	string str;
	char xml[] = "<?xml version=\"1.0\" encoding=\"latin-1\"?><book></book>";

	//Parse the original document
	xml_document<> doc;
	doc.parse<0>(xml);

	cout << "Name of my first node is: " << doc.first_node()->name() << "\n";
	str.append("Name of my first node is: ").append(doc.first_node()->name());
	new WText(str, root());
	new WBreak(root());

	//Insert something
	xml_node<> *node = doc.allocate_node(node_element, "author", "John Doe");
	doc.first_node()->append_node(node);

	stringstream ss;
	ss << *doc.first_node();
	str.assign(ss.str());
	new WText(str, root());

	string result_xml = ss.str();
	cout << result_xml << endl;
}

void WtApplication::test2() {
	string r, s;
	ifstream file("/home/user/projects/wt/tests/RapidXML/Info_BMFBOVESPA.xml", ios::in);
	while (getline(file, r)) { s.append(r); }

	char* a = new char[s.size()+1];
	a[s.size()] = 0;
	memcpy(a,s.c_str(),s.size());

	xml_document<> doc;
	doc.parse<0>(a);
	cout << "Name of my first node is: " << doc.first_node()->name() << "\n";
}

void WtApplication::test3() {
	// http://www.cplusplus.com/reference/clibrary/cstdio/fread/
	FILE *fp = fopen("/home/user/projects/wt/tests/RapidXML/Info_BMFBOVESPA.xml","r");
	//if (fp==NULL) {fputs ("File error",stderr); exit (1);}
	//if (fp.fail()) return 1;

	fseek(fp,0,SEEK_END); //go to end
	long len = ftell(fp); //get position at end (length)
	rewind(fp);
	fseek(fp,0,SEEK_SET); //go to beg
	char *buf = (char*) malloc(sizeof(char)*len); //malloc buffer
	//if (buf == NULL) {fputs ("Memory error",stderr); exit (2);}

	size_t result = fread(buf,1,len,fp); //read into buffer
	//if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

	fclose(fp);

	xml_document<> doc;
	doc.parse<0>(buf);
	free(buf);
	cout << "Name of my first node is: " << doc.first_node()->name() << "\n";
	xml_node<> *node = doc.first_node();

	cout << "Node foobar has value " << node->value() << "\n";
	for (xml_attribute<> *attr = node->first_attribute(); attr; attr = attr->next_attribute()) {
		cout << "Node foobar has attribute " << attr->name() << " ";
		cout << "with value " << attr->value() << "\n";
	}
}

void WtApplication::test4() {
	ifstream fl("/home/user/projects/wt/tests/RapidXML/Info_BMFBOVESPA.xml", ios::in | ios::binary);
	//if (fl.fail()) return 1;
	fl.seekg(0, ios::end);
	size_t len = fl.tellg();
	fl.seekg(0, ios::beg);

	char* buf = new char[len + 1];
	fl.read(buf, len);
	fl.close();
	buf[len] = '\0';

	xml_document<> doc;
	doc.parse<0>(buf);
	delete [] buf;

	cout << "The first node is '" << doc.first_node()->name() << "'\n";
	xml_node<> *statuses, *status_c, *status_cc;
	statuses = doc.first_node();

	for (status_c = statuses->first_node("status"); status_c; status_c = status_c->next_sibling()) {
		status_cc = status_c->first_node("text");
		cout << status_cc->name() << endl;
		cout << status_cc->value() << endl;
	}
}

vector<xml_node<> *> childElements(xml_node<> *element, const char *tagName) {
	vector<xml_node<> *> result;
	xml_node<> *r;

	for (r = element->first_node(tagName); r; r = r->next_sibling(tagName)) { result.push_back(r); }

	return result;
}

void WtApplication::test5() {
	ifstream fl("/home/user/projects/wt/tests/RapidXML/Info_BMFBOVESPA.xml", ios::in | ios::binary);
	fl.seekg(0, ios::end);
	size_t len = fl.tellg();
	fl.seekg(0, ios::beg);

	boost::scoped_array<char> buf(new char[len + 1]);
	fl.read(buf.get(), len);
	fl.close();
	buf[len] = '\0';

	xml_document<> doc;
	doc.parse<parse_normalize_whitespace | parse_trim_whitespace | parse_validate_closing_tags>(buf.get());
	buf.reset();
	xml_node<> *root, *app;
	root= doc.first_node();
	vector<xml_node<> *> applications = childElements(root, "status");

	for (unsigned i = 0; i < applications.size(); ++i) {
		app = applications[i]->first_node("text");
		cout << app->value() << endl;
	}
}

void WtApplication::test6() {
	ifstream fl("/home/user/projects/wt/tests/RapidXML/Info_BMFBOVESPA.xml");
	//if (fin.fail()) return 1;

	fl.seekg(0, ios::end);
	size_t len = fl.tellg();
	fl.seekg(0, ios::beg);
	char* buf = new char[len + 1];
	fl.read(buf, len);
	buf[len] = '\0';
	fl.close();

	xml_document<> doc;
	doc.parse<0>(buf);
	delete [] buf;

	xml_node<> *statuses = doc.first_node();

	get_child(statuses, "screen_name");
}

xml_node<>* WtApplication::get_child(xml_node<> *inputNode, string sNodeFilter) {
	for (xml_node<> *nodeChild = inputNode->first_node(); nodeChild; nodeChild = nodeChild->next_sibling()) {
		if (nodeChild->name() == sNodeFilter) {
			cout << "node name " << nodeChild->name() << "\n";
			cout << "nodeChild " << nodeChild << endl;
			// returns the desired child
			return nodeChild;
		}
		xml_node<> * x = get_child(nodeChild, sNodeFilter);
		if (x) return x;
	}
	return 0;
}

WtApplication::WtApplication(const WEnvironment& env) : WApplication(env) {
	//test1();
	//test2();
	//test3();  // nem sempre funciona
	//test4(); // OK em C++
	test5(); // OK usando Boost
	//test6();
}

WApplication *createApplication(const WEnvironment& env) { return new WtApplication(env); }

int main(int argc, char **argv) { return WRun(argc, argv, &createApplication); }
