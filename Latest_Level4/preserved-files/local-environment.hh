
/*********************************************************************************************

                                sclp : A C Language Processor
                                --------------------------------

           About:

           Originally implemented in 2012 by Tanu Kanvar and Uday Khedker 
           (http://www.cse.iitb.ac.in/~uday) for the courses cs302+cs316
           Language Processors (theory and lab) at IIT Bombay.

           Initial release date Jan 15, 2013.

           Updated by N. Venkatesh in Jan 2019.

           Updated by Uday Khedker with the help of Nisha Biju and Saari
           Rajan in Jan 2020.

           Copyrights reserved  by Uday Khedker. This  implemenation has
           been made available purely  for academic purposes without any
           warranty of any kind.


***********************************************************************************************/

#ifndef LOCAL_ENVIRONMENT_HH
#define LOCAL_ENVIRONMENT_HH

#include<string>
#include<map>

#define VAR_SPACE "         "

using namespace std;

typedef enum
{
	int_result,
	void_result,
	double_result
} Result_Enum;

class Eval_Result;
class Local_Environment;

class Eval_Result 
{
protected:
	Result_Enum result_type;

public:
	virtual int get_int_value();
	virtual void set_value(int value);
	virtual double get_double_value();
	virtual void set_value(double value);
	
	virtual bool is_variable_defined();
	virtual void set_variable_status(bool def);

	virtual void set_result_enum(Result_Enum) = 0;
	virtual Result_Enum get_result_enum() = 0;
};

class Eval_Result_Value:public Eval_Result
{
public:
	virtual void set_value(int number);
	virtual void set_value(double number);
	virtual int get_int_value();
	virtual double get_double_value();

	virtual bool is_variable_defined() = 0;
	virtual void set_variable_status(bool def) = 0;

	virtual void set_result_enum(Result_Enum res) = 0;
	virtual Result_Enum get_result_enum() = 0;
};

class Eval_Result_Value_Int:public Eval_Result_Value
{
	int value;
	bool defined;
public:
	Eval_Result_Value_Int();
	~Eval_Result_Value_Int();

	void set_value(int number);
	void set_value(double number);
	int get_int_value();

	void set_variable_status(bool def);
	bool is_variable_defined();

	void set_result_enum(Result_Enum res);
	Result_Enum get_result_enum();
};

class Eval_Result_Value_Double:public Eval_Result_Value
{
	double value;
	bool defined;
public:
	Eval_Result_Value_Double();
	~Eval_Result_Value_Double();

	void set_value(double number);
	void set_value(int number);
	double get_double_value();

	void set_variable_status(bool def);
	bool is_variable_defined();

	void set_result_enum(Result_Enum res);
	Result_Enum get_result_enum();
};

class Local_Environment
{
	map<string, Eval_Result *> variable_table;

public:
	Local_Environment();
	~Local_Environment();

	void print(ostream & file_buffer);
	bool is_variable_defined(string name);
	Eval_Result * get_variable_value(string name);
	void put_variable_value(Eval_Result & value, string name);
	bool does_variable_exist(string name);
};

extern Local_Environment interpreter_global_table;

#endif
