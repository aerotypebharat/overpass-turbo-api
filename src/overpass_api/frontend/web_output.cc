#include "web_output.h"

#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

void Web_Output::add_encoding_error(const string& error)
{
  if (log_level != Error_Output::QUIET)
  {
    ostringstream out;
    out<<"encoding error: "<<error;
    display_error(out.str());
  }
  encoding_errors = true;
}

void Web_Output::add_parse_error(const string& error, int line_number)
{
  if (log_level != Error_Output::QUIET)
  {
    ostringstream out;
    out<<"line "<<line_number<<": parse error: "<<error;
    display_error(out.str());
  }
  parse_errors = true;
}

void Web_Output::add_static_error(const string& error, int line_number)
{
  if (log_level != Error_Output::QUIET)
  {
    ostringstream out;
    out<<"line "<<line_number<<": static error: "<<error;
    display_error(out.str());
  }
  static_errors = true;
}

void Web_Output::add_encoding_remark(const string& error)
{
  if (log_level == Error_Output::VERBOSE)
  {
    ostringstream out;
    out<<"encoding remark: "<<error;
    display_remark(out.str());
  }
}

void Web_Output::add_parse_remark(const string& error, int line_number)
{
  if (log_level == Error_Output::VERBOSE)
  {
    ostringstream out;
    out<<"line "<<line_number<<": parse remark: "<<error;
    display_remark(out.str());
  }
}

void Web_Output::add_static_remark(const string& error, int line_number)
{
  if (log_level == Error_Output::VERBOSE)
  {
    ostringstream out;
    out<<"line "<<line_number<<": static remark: "<<error;
    display_remark(out.str());
  }
}

void Web_Output::runtime_error(const string& error)
{
  if (log_level != Error_Output::QUIET)
  {
    ostringstream out;
    out<<"runtime error: "<<error;
    display_error(out.str());
  }
}

void Web_Output::runtime_remark(const string& error)
{
  if (log_level == Error_Output::VERBOSE)
  {
    ostringstream out;
    out<<"runtime remark: "<<error;
    display_remark(out.str());
  }
}

void Web_Output::display_statement_stopwatch
    (const string& name,
     const vector< double >& stopwatches,
     const vector< uint >& read_counts)
{
  if (log_level != Error_Output::VERBOSE)
    return;
  ostringstream out;
  out<<"Stopwatch "<<name;
  vector< uint >::const_iterator rit(read_counts.begin());
  for (vector< double >::const_iterator it(stopwatches.begin());
      it != stopwatches.end(); ++it)
  {
    out<<setprecision(3)<<fixed<<'\t'<<*it<<' '<<*rit;
    ++rit;
  }
  display_remark(out.str());
}

void Web_Output::enforce_header()
{
  if (header_written == not_yet)
    write_html_header();
}

void Web_Output::write_html_header
    (const string& timestamp, const string& area_timestamp)
{
  if (header_written != not_yet)
    return;    
  header_written = html;
  
  cout<<
  "Content-Type: text/html; charset=utf-8\n\n";
  cout<<
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
  "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\"\n"
  "    \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n"
  "<html xmlns=\"http://www.w3.org/1999/xhtml\" xml:lang=\"en\" lang=\"en\">"
  "<head>\n"
  "  <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\" lang=\"en\"/>\n"
  "  <title>OSM3S Response</title>\n"
  "</head>\n"
  "<body>\n\n"
  "<p>The data included in this document is from www.openstreetmap.org. "
  "It has there been collected by a large group of contributors. For individual "
  "attribution of each item please refer to "
  "http://www.openstreetmap.org/api/0.6/[node|way|relation]/#id/history </p>\n";
  if (timestamp != "")
  {
    cout<<"<p>Data included until: "<<timestamp;
    if (area_timestamp != "")
      cout<<"<br/>Areas based on data until: "<<area_timestamp;
    cout<<"</p>\n";
  }
}

void Web_Output::write_xml_header
    (const string& timestamp, const string& area_timestamp)
{
  if (header_written != not_yet)
    return;    
  header_written = xml;
  
  cout<<
  "Content-type: application/osm3s\n\n";
  cout<<
  "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<osm-derived>\n"
  "<note>The data included in this document is from www.openstreetmap.org. "
  "It has there been collected by a large group of contributors. For individual "
  "attribution of each item please refer to "
  "http://www.openstreetmap.org/api/0.6/[node|way|relation]/#id/history </note>\n";
  cout<<"<meta osm_base=\""<<timestamp<<'\"';
  if (area_timestamp != "")
    cout<<" areas=\""<<area_timestamp<<"\"";
  cout<<"/>\n\n";
}

void Web_Output::write_footer()
{
  if (header_written == xml)
    cout<<"\n</osm-derived>\n";
  else if (header_written == html)
    cout<<"\n</body>\n</html>\n";
  header_written = final;
}

void Web_Output::display_remark(const string& text)
{
  enforce_header();
  if (header_written == xml)
    cout<<"<remark> "<<text<<" </remark>\n";
  else if (header_written == html)
    cout<<"<p><strong style=\"color:#00BB00\">Remark</strong>: "
        <<text<<" </p>\n";
}

void Web_Output::display_error(const string& text)
{
  enforce_header();
  if (header_written == xml)
    cout<<"<remark> "<<text<<" </remark>\n";
  else if (header_written == html)
    cout<<"<p><strong style=\"color:#FF0000\">Error</strong>: "
        <<text<<" </p>\n";
}