/* ============================================================================

* --------------------------------------------- *
*                   ____                        *
*                  | __  \                      *
*    __ _ _ __ __ _| |_) |__ _ _ __ ___  ___    *
*   / _` | '__/ _` |  __ / _` | '__/ __|/ _ \   *
*  | (_| | | | (_| | |  | (_| | |  \__ \  __/   *
*   \__,_|_|  \__, |_.   \__,_|_|  |___/\___|   *
*             |___/                             *
*                                               *
* --------------------------------------------- *

Argument Parser Based on C++ 17
https://github.com/Jianxff/argParse

Copyright 2023 Jianxff

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

============================================================================ */

#ifndef __ARGPARSE_HPP__
#define __ARGPARSE_HPP__

#include <cstring>
#include <iostream>
#include <set>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
class ArgumentParser;
class BasicArgument{
  friend class ArgumentParser;
public:
  enum class BaseType{
    TYPE_BOOL,
    TYPE_CHAR,
    TYPE_INT,
    TYPE_LONG_LONG,
    TYPE_DOUBLE,
    TYPE_STRING,
    TYPE_ERROR
  };

  class Exception : public std::exception{
    std::string _msg;
  public:
    Exception(BasicArgument* arg,std::string msg = ""){
      std::string name = (arg->name().length() > 0 ? arg->name() : arg->tag());
      _msg = "at " + name + "(" + arg->type_name() + "): " + std::string(msg);
    }

    Exception(std::string msg = ""){
      _msg = std::string(msg);
    }

    const char* what() const throw(){
      return _msg.c_str();
    }

  };
  virtual ~BasicArgument() = default;

protected:
  /**
   * @brief type assertion
   * @tparam T
   * @return
   */
  template <typename T>
  bool type_assert(){
    switch(_base_type){
      case BaseType::TYPE_BOOL: return std::is_same<bool,T>::value;
      case BaseType::TYPE_CHAR: return std::is_same<char,T>::value;
      case BaseType::TYPE_INT: return std::is_same<int,T>::value;
      case BaseType::TYPE_LONG_LONG: return std::is_same<long long,T>::value;
      case BaseType::TYPE_DOUBLE: return std::is_same<double,T>::value;
      case BaseType::TYPE_STRING: return std::is_same<std::string,T>::value;
      default: return false;
    }
  }

  /**
   * @brief value cast
   * @tparam T
   * @param str
   * @return
   */
  template<typename T> T value_cast(std::string str){
    if(std::is_same<bool,T>::value) {
      bool val = (str == "1" || str == "true" || str == "True" || str == "TRUE");
      return *(T*)(&val);
    }else if(std::is_same<char,T>::value){
      return *(T*)(&str[0]);
    }else if(std::is_same<std::string,T>::value){
      return *(T*)(&str);
    }
    std::stringstream ss(str);
    T val;
    ss >> val;
    return val;
  }

  /**
   * @brief init base_type
   * @tparam T
   */
  template <typename T>
  void init(){
    _base_type = std::is_same<bool,T>::value ? BaseType::TYPE_BOOL :
                 std::is_same<char,T>::value ? BaseType::TYPE_CHAR :
                 std::is_same<int,T>::value ? BaseType::TYPE_INT :
                 std::is_same<long long,T>::value ? BaseType::TYPE_LONG_LONG :
                 std::is_same<double,T>::value ? BaseType::TYPE_DOUBLE :
                 std::is_same<std::string,T>::value ? BaseType::TYPE_STRING :
                                                     BaseType::TYPE_ERROR;
    if(_base_type == BaseType::TYPE_ERROR)
      throw std::runtime_error("init(): unsupported type " + std::string(typeid(T).name()));
  }

  BaseType base_type() const{ return _base_type;}
  std::string type_name() {
    switch(_base_type){
    case BaseType::TYPE_BOOL: return "bool";
    case BaseType::TYPE_CHAR: return "char";
    case BaseType::TYPE_INT: return "int";
    case BaseType::TYPE_LONG_LONG: return "long long";
    case BaseType::TYPE_DOUBLE: return "double";
    case BaseType::TYPE_STRING: return "string";
    default: return "error";
    }
  }

  std::string tag() { return _tag;}
  std::string name() { return _name;}
  std::string help() { return _help;}
  char nargs() { return _n_args == 0 ? '*' : _n_args == 1 ? '?' : '+';}
  bool call_() { return _call;}
  std::set<std::string>& conflicts() { return _conflicts;}

  void set_tag(std::string tag) { _tag = tag;}
  void set_name(std::string name) { _name = name;}
  void set_help(std::string help) { _help = help;}
  void set_call(bool call) {_call = call;}
  void append_conflict(std::string str) { _conflicts.insert(str); }

  virtual void add_value(std::string val_str = ""){}
  virtual void argument_assert(){}

protected:
  bool          _call = false;            // if called
  BaseType      _base_type;               // base type
  bool          _required   = false;      // required flag
  int           _n_args     = 1;          // arg-number tag (0: *, 1: ?, 2: +)
  std::string   _help = "";               // help-info string
  std::string   _tag = "", _name = "";    // tag-names
  std::set<std::string> _conflicts;       // conflict params

};


/**
* @brief Argument Type
*
*/
template <typename T>
class Argument : public BasicArgument{
  T               _default;     // default value
  std::vector<T>  _values;      // values from input(console)
  std::set<T>     _choices;     // relative choices

public:
  /**
    * @brief Construct a new Argument object
    *
   */
  Argument() {
    init<T>();
    _default = T();
  }


  /**
    * @brief set relatice choices
    *
    * @param choice_values
    * @return Argument&
   */
  Argument& choices(std::vector<T> choice_values){
    if(choice_values.size() == 0)
      throw BasicArgument::Exception("choices(): no elements");

    _choices.clear();
    for(auto& it:choice_values)
      _choices.insert(it);
    _default = choice_values[0];
    return *this;
  }

  /**
    * @brief set default value
    *
    * @param default_value
    * @return Argument&
   */
  Argument& default_(T default_value){
    _default = default_value;
    if(_choices.size() > 0){
      if(_choices.find(default_value) == _choices.end())
        throw BasicArgument::Exception(this,"default_(): not in choices");
    }
    return *this;
  }


  /**
    * @brief set required flag
    *
    * @param required_value
    * @return Argument&
   */
  Argument& required(bool required_value = true){
    _required = required_value;
    return *this;
  }

  /**
    * @brief set n-arg number-tag
    *
    * @param ch
    * @return Argument&
   */
  Argument& nArgs(const char ch){
    if(std::is_same<bool,T>::value && ch == '+')
      throw BasicArgument::Exception(this,"nArgs(): not available on bool args");

    if(ch == '*') _n_args = 0;
    else if(ch == '?') _n_args = 1;
    else if(ch == '+') _n_args = 2;
    else throw BasicArgument::Exception(this,"nArgs(): unknown tag '" + std::to_string(ch) + "'");

    return *this;
  }


  /**
    * @brief set help info string
    *
    * @param str
    * @return Argument&
   */
  Argument& help(std::string str){
    _help = str;
    return *this;
  }

  /**
   * @brief add conflict params
   * @param str
   * @return
   */
  Argument& conflict(std::string str) {
    append_conflict(str);
    return *this;
  }


  /**
    * @brief get one argument value (parameter)
    *
    * @param index
    * @return T
   */
  T value(int index = 0){
    if(_values.size() > 0){
      if(index < 0) index = _values.size() + index;
      if(index < 0 || index >= _values.size())
        throw std::out_of_range("value(): invalid index");
      return _values[index];
    }else{
      return _default;
    }
  }


  /**
    * @brief get argument values (parameters)
    *
    * @return std::vector<T>
   */
  std::vector<T> values(){
    if(_values.size() > 0){
      return _values;
    }else{
      return std::vector<T>(1,_default);
    }
  }

protected:
  /**
    * @brief assert value type
    * @param str
    * @return
   */
  bool value_assert(std::string str) {
    try{
      switch(_base_type){
        case BaseType::TYPE_BOOL: break;
        case BaseType::TYPE_CHAR: return str.length() == 1;
        case BaseType::TYPE_INT: {volatile int t = std::stoi(str); break;}
        case BaseType::TYPE_LONG_LONG: {volatile long long t = std::stoll(str); break;}
        case BaseType::TYPE_DOUBLE: {volatile double t = std::stod(str); break;}
        case BaseType::TYPE_STRING: break;
        default: return false;
      }
      return true;
    }catch (std::exception& e){
      return false;
    }
  }


  /**
    * @brief add value from argv (const char*)
    *
    * @param str
    * @return Argument&
   */
  void add_value(std::string str) override{
    if(std::is_same<bool,T>::value || str.length() == 0) {
      return;
    }

    if(_n_args == 1 && _values.size() >= 1)
      throw BasicArgument::Exception(this,"add_value(): too many arg values");
    std::string str_(str);
    if(!value_assert(str_))
      throw BasicArgument::Exception(this,"add_value(): mistyped value '" + str_ + "'");
    _values.emplace_back(value_cast<T>(str));
  }


  /**
    * @brief assert read parameters
    *
   */
  void argument_assert() override{
    /* assert require */
    if(_required && _values.size() == 0)
      throw BasicArgument::Exception(this,"argument required");

    /* assert numbers */
    if(_n_args == 2 && _values.size() == 0)
      throw BasicArgument::Exception(this,"too few parameters");

    if(_n_args == 1 && _values.size() > 1)
      throw BasicArgument::Exception(this,"too many parameters");

    /* assert choices */
    if(_choices.size() > 0){
      for(int i = 0; i < _values.size(); i++){
        if(_choices.find(_values[i]) == _choices.end())
          throw BasicArgument::Exception(this,"not found in choices");
      }
    }
  }
};






/**
* @brief Argument Parser Type
*
*/
class ArgumentParser{
  std::string                         _file_name = "command";     // filename
  bool                                _positional_flag = true;    // available flag for adding positional args
  int                                 _positional_num = 0;        // number of positional args
  std::unordered_map<std::string,int> _tag_map;                   // mapping short string to pos of argument-structure
  std::unordered_map<std::string,int> _name_map;                  // mapping long string to pos of argument-structure
  std::vector<BasicArgument*>         _args;                      // argument-structure list

  // filter prefix of '-'
  int _filter(std::string input, std::string& output){
    output = "";
    if(input.size() == 0) return -1;
    int p;
    for(p = 0; p < input.length() && input[p] == '-'; p++);
    output = (p > 2 ? "" : input.substr(p));
    return p;
  }

  // calc prefix of '-'
  int _argname_assert(std::string name){
    if(name.length() == 0) return -1;
    int p;
    for(p = 0;p < name.length() && name[p] == '-';p++);
    return p <= 2 ? p : -1;
  }

  /**
    * @brief Get the argument object pointer from argument list
    *
    * @param index
    * @return Argument*
   */
  BasicArgument* _get_argument(std::string index){
    std::string name;
    int p = _filter(index,name);
    if(p != 1 && p != 2) return nullptr;
    int pos = p == 1 ? (_tag_map.find(name) == _tag_map.end() ? -1 : _tag_map[name]) :
                     (_name_map.find(name) == _name_map.end() ? -1 : _name_map[name]);
    return pos == -1 ? nullptr : _args[pos];
  }

public:
  /**
   * @brief Destroy the Argument Parser object
   */
  ~ArgumentParser(){
    for(int i = 0; i < _args.size(); i++){
      delete _args[i];
    }
  }

  /**
    * @brief Add new argument structure
    *
    * @tparam T
    * @param name_or_flag
    * @param full_argname
    * @return Argument&
   */
  template<typename T>
  Argument<T>& add_argument(std::string tag, std::string name = ""){
    /* assertion */
    int tag_type[2] = {_argname_assert(tag), _argname_assert(name)};
    if((tag_type[0] != 0 && tag_type[0] != 1)           ||
        (name.length() > 0 && tag_type[1] != 2)  ||
        (tag_type[0] == 0 && tag_type[1] > 0))
    {
      throw std::runtime_error("add_argument(): invalid argument format");
    }

    /* consturct new argument and initialize */
    /* tag assertion */
    std::string tag_f, name_f;
    _filter(tag,tag_f);
    _filter(name,name_f);

    if(tag_f == "h" || tag_f == "help" || name_f == "help")
      throw BasicArgument::Exception("add_argument(): reserved keyword");

    Argument<T>* new_arg = new Argument<T>();
    new_arg->set_tag(tag_f);
    new_arg->set_name(name_f);
    _args.emplace_back(new_arg);

    if(tag_type[0] == 0){
      /* positional arguments */
      if(!_positional_flag)
        throw BasicArgument::Exception("add_argument(): positional args should be set before optional args");
      _positional_num++;
      (*new_arg).required().nArgs('?'); // set positional arg
    }else{
      /* optional arguments */
      _positional_flag = false;
    }

    /* insert into parser */
    if(_tag_map.find(tag_f) != _tag_map.end() ||
        _name_map.find(name_f) != _name_map.end())
      throw BasicArgument::Exception("add_argument(): duplicated argument");

    _tag_map[tag_f] = _args.size() - 1;
    if(name.length() > 0)
      _name_map[name_f] = _args.size() - 1;

    return *new_arg;
  }


  /**
    * @brief Get one argument value by tag name
    *
    * @tparam T
    * @param index
    * @return T
   */
  template<typename T>
  T get_value(const char* index){
    auto iter = _tag_map.find(index);
    if(iter == _tag_map.end()) iter = _name_map.find(index);
    if(iter == _tag_map.end() || iter == _name_map.end())
      throw BasicArgument::Exception("get_value(): argument not found: " + std::string(index));

    BasicArgument* arg = _args[(*iter).second];
    if(!arg->type_assert<T>())
      throw BasicArgument::Exception(arg,"get_value(): invalid cast <" + std::string(typeid(T).name()) + ">");
    return (dynamic_cast<Argument<T>*>(arg))->value();
  }


  /**
    * @brief Get one argument value by index
    *
    * @tparam T
    * @param index
    * @return T
   */
  template<typename T>
  T get_value(int index){
    if(index < 0) index += _args.size();
    BasicArgument* arg = _args[index];
    if(!arg->type_assert<T>())
      throw BasicArgument::Exception(arg,"get_value(): invalid cast <" + std::string(typeid(T).name()) + ">");
    return dynamic_cast<Argument<T>*>(arg)->value();
  }


  /**
    * @brief Get all argument values by tag name
    *
    * @tparam T
    * @param index
    * @return std::vector<T>
   */
  template<typename T>
  std::vector<T> get_values(std::string index){
    auto iter = _tag_map.find(index);
    if(iter == _tag_map.end()) iter = _name_map.find(index);
    if(iter == _tag_map.end() || iter == _name_map.end())
      throw BasicArgument::Exception("get_value(): argument not found: " + std::string(index));

    BasicArgument* arg = _args[(*iter).second];
    if(!arg->type_assert<T>())
      throw BasicArgument::Exception(arg,"get_value(): invalid cast <" + std::string(typeid(T).name()) + ">");
    return (dynamic_cast<Argument<T>*>(arg))->values();
  }


  /**
    * @brief Get all argument values by index
    *
    * @tparam T
    * @param index
    * @return std::vector<T>
   */
  template<typename T>
  std::vector<T> get_values(int index){
    if(index < 0) index += _args.size();
    BasicArgument* arg = _args[index];
    if(!arg->type_assert<T>())
      throw BasicArgument::Exception(arg,"get_value(): invalid cast <" + std::string(typeid(T).name()) + ">");
    return dynamic_cast<Argument<T>*>(arg)->values();
  }

  bool is_call(std::string index){
    auto iter = _tag_map.find(index);
    if(iter == _tag_map.end()) iter = _name_map.find(index);
    if(iter == _tag_map.end() || iter == _name_map.end())  return false;
    BasicArgument* arg = _args[(*iter).second];
    return arg->call_();
  }

  bool is_call(int index){
    if(index < 0) index += _args.size();
    BasicArgument* arg = _args[index];
    return arg->call_();
  }


  /**
    * @brief start parsing args with argc and argv
    *
    * @param argc
    * @param argv
   */
  void parse_args(int argc, char** argv){
    if(argc <= 0 || argv == nullptr)
      throw std::runtime_error("parse_args(): no args");

    /* help info */
    for(int i = 1; i < argc; i++){
      if(std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help"){
        show_help();
        exit(0);
      }
    }

    _file_name = std::string(argv[0]);
    int ptr = 1;
    /* read positional args */
    for(int i = 0; ptr < argc && i < _positional_num; i++){
      _args[i]->add_value(argv[ptr++]);
    }

    /* process optional args */
    while(ptr < argc){
      std::string arg_str(argv[ptr++]);
      BasicArgument* arg_ptr = _get_argument(arg_str);
      if(arg_ptr == nullptr)
        throw std::runtime_error("parse_args(): unknown argument '" + arg_str + "'");
      arg_ptr->set_call(true);
      while(ptr < argc && _argname_assert(std::string(argv[ptr])) == 0){
        arg_ptr->add_value(argv[ptr++]);
      }
    }

    /* assertion */
    for(auto it:_args) {
      it->argument_assert();
      if(it->call_()){
        auto clist = it->conflicts();
        for(auto it2 = clist.begin(); it2 != clist.end(); it2++){
          if(is_call(*it2))
            throw BasicArgument::Exception(it,"conflicts(): conflict against '" + *it2 + "'");
        }
      }
    }
  }


  /**
    * @brief print help info
    *
   */
  void show_help(){
    std::cout << "usage: " << _file_name << " ";
    for(int i = 0; i < _positional_num; i++)
      std::cout << _args[i]->tag() << " ";
    for(int i = _positional_num; i < _args.size(); i++)
      std::cout << "[-" << _args[i]->tag() << "] ";

    /* positional */
    std::cout << "\n\npositional arguments:\n";
    for(int i = 0; i < _positional_num; i++)
      std::cout << "  " << _args[i]->tag() << "\t" << _args[i]->help() << std::endl;

    /* optional */
    std::cout << "\noptional arguments:\n";
    std::cout << " tag  name\tn-args\tnote\n";
    std::cout << "  -h, --help\t  - \tshow help message\n";
    for(int i = _positional_num; i < _args.size(); i++){
      std::cout << "  -" << _args[i]->tag();
      if(_args[i]->name().length() > 0)
        std::cout << ", --" << _args[i]->name();
      else std::cout << "\t";
      std::cout << "\t [" << _args[i]->nargs() << "] ";
      std::cout << "\t" << _args[i]->help() << std::endl;
    }
  }

};


#endif