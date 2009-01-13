#ifndef QUERY_STATEMENT_DEFINED
#define QUERY_STATEMENT_DEFINED

#include <map>
#include <string>
#include <vector>
#include "script_datatypes.h"

#include <mysql.h>

using namespace std;

//-----------------------------------------------------------------------------

class Query_Statement : public Statement
{
  public:
    Query_Statement() {}
    virtual void set_attributes(const char **attr);
    virtual void add_statement(Statement* statement, string text);
    virtual string get_name() { return "query"; }
    virtual string get_result_name() { return output; }
    virtual void execute(MYSQL* mysql, map< string, Set >& maps);
    virtual ~Query_Statement() {}
    
  private:
    string output;
    unsigned int type;
    vector< pair< string, string > > key_values;
};

class Has_Key_Value_Statement : public Statement
{
  public:
    Has_Key_Value_Statement() {}
    virtual void set_attributes(const char **attr);
    virtual void add_statement(Statement* statement, string text);
    virtual string get_name() { return "has-kv"; }
    virtual string get_result_name() { return ""; }
    virtual void execute(MYSQL* mysql, map< string, Set >& maps) {}
    virtual ~Has_Key_Value_Statement() {}
    
    string get_key() { return key; }
    string get_value() { return value; }
    
  private:
    string key, value;
};

#endif