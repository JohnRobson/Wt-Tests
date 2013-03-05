/*
 * Copyright (C) 2008 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <stdlib.h>
#include <boost/tokenizer.hpp>

#include <Wt/WAbstractItemModel>
#include <Wt/WString>
#include <vector>

#include "CsvUtil.h"

using namespace std;

void readFile(istream& f, vector<string>& content) {
  string r;

  while(getline(f, r)) {
    content.push_back(r);
  }
}

void createModel(istream& f, Wt::WAbstractItemModel* model, int numRows, bool firstLineIsHeaders) {
  int csvRow = 0;
  typedef boost::tokenizer<boost::escaped_list_separator<char> > CsvTokenizer;

  vector<string> content;
  readFile(f, content);

for(vector<string>::iterator it = content.begin(); it != content.end(); ++it) {
    CsvTokenizer tok(*it);

    int col = 0;
    for(CsvTokenizer::iterator i = tok.begin(); i != tok.end(); ++i, ++col) {

      if(col >= model->columnCount())
        model->insertColumns(model->columnCount(), col + 1 - model->columnCount());

      if(firstLineIsHeaders && csvRow == 0)
        model->setHeaderData(col, boost::any(Wt::WString::fromUTF8(*i)));
      else {
        int dataRow = firstLineIsHeaders ? csvRow - 1 : csvRow;

        if(numRows != -1 && dataRow >= numRows)
          return;

        if(dataRow >= model->rowCount())
          model->insertRows(model->rowCount(), dataRow + 1 - model->rowCount());

        boost::any data;
        string s = *i;
        char* endptr;

        if(s.empty())
          data = boost::any();
        else {
          double d = strtod(s.c_str(), &endptr);

          if(*endptr == 0)
            data = boost::any(d);
          else
            data = boost::any(Wt::WString::fromUTF8(s));
        }
        model->setData(dataRow, col, data);
      }
    }
    ++csvRow;
  }
}
