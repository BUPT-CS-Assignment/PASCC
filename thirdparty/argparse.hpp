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

#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <any>
#include <cstring>
#include <string>
#include <typeinfo>
#include <type_traits>


/**
 * @brief Argument Type
 * 
 */
class Argument{
    bool                    _required   = false;                // required flag
    char                    _n_args     = '?';                  // arg-number tag
    std::string             _help = "";                         // help-info string
    std::string             _short_tag = "", _full_tag = "";    // tag-names

    std::any                _default;                           // default values
    std::vector<std::any>   _values;                            // values from input(console)
    std::vector<std::any>   _choices;                           // relative choices
    
    // helper function for compare two std::any vars
    bool _is_equal(std::any& e1, std::any& e2){
        if(_default.type() == typeid(bool))
            return std::any_cast<bool>(e1) == std::any_cast<bool>(e2);
        else if(_default.type() == typeid(int))
            return std::any_cast<int>(e1) == std::any_cast<int>(e2);
        else if(_default.type() == typeid(long long))
            return std::any_cast<long long>(e1) == std::any_cast<long long>(e2);
        else if(_default.type() == typeid(double))
            return std::any_cast<double>(e1) == std::any_cast<double>(e2);
        else if(_default.type() == typeid(char))
            return std::any_cast<char>(e1) == std::any_cast<char>(e2);
        else if(_default.type() == typeid(std::string))
            return std::any_cast<std::string>(e1) == std::any_cast<std::string>(e2);
        return false;
    }

public:
    /**
     * @brief Argument Exception Type
     * 
     */
    class Exception : public std::exception{
        std::string _msg = "";
    public:
        Exception(Argument* arg,std::string msg = ""){
            _msg = arg->tag() + "(" + arg->arg_typename() + ") : " + std::string(msg);
        }

        Exception(std::string msg = ""){
            _msg = std::string(msg);
        }

        const char* what() const throw(){
            return _msg.c_str();
        }

    };


    /**
     * @brief Construct a new Argument object
     * 
     * @tparam T type of argument
     */
    template<typename T>
    Argument& init(){
        if(std::is_same<bool,T>::value      ||
           std::is_same<int,T>::value       || 
           std::is_same<long long,T>::value ||
           std::is_same<double,T>::value    || 
           std::is_same<char,T>::value      ||
           std::is_same<std::string,T>::value
        ){
            _default.emplace<T>();  // set default type
        }else
            throw Exception("unsupported type " + std::string(typeid(T).name()));   
        return *this;
    }


    /**
     * @brief set relatice choices
     * 
     * @tparam T 
     * @param choice_values 
     * @return Argument& 
     */
    template<typename T>
    Argument& choices(std::vector<T> choice_values){
        if(_default.type() != typeid(T))
            throw Exception(this,"mismatched type in choices") ;
        if(choice_values.size() == 0)
            throw Exception("no choices given");

        _choices.clear();
        for(auto& it:choice_values)
            _choices.emplace_back(it);
        _default.emplace<T>(choice_values[0]);
        return *this;
    }
    

    /**
     * @brief set default value
     * 
     * @tparam T 
     * @param default_value 
     * @return Argument& 
     */
    template<typename T>
    Argument& default_(T default_value){
        _default.emplace<T>(default_value);
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
        if(_default.type() == typeid(bool) && ch == '+')
            throw Exception(this,"n-arg funciton is not available on bool args");

        if(ch == '?' || ch == '+' || ch == '*')
            _n_args = ch;
        else
            throw Exception(this,"unknown n-arg tag '" + std::to_string(ch) + "'");
        
        
        return *this;
    }


    /**
     * @brief set help info string
     * 
     * @param str 
     * @return Argument& 
     */
    Argument& help(const char* str){
        _help = std::string(str);
        return *this;
    }


    /**
     * @brief get one argument value (parameter)
     * 
     * @tparam T 
     * @param index 
     * @return T 
     */
    template<typename T>
    T value(int index = 0){
        if(_values.size() > 0){
            if(index < 0)
                index = _values.size() + index;
            return std::any_cast<T>(_values[index]);
        }else{
            if(index == 0)
                return std::any_cast<T>(_default);
        }
        throw std::out_of_range("invalid index of values");
    }


    /**
     * @brief get argument values (parameters)
     * 
     * @tparam T 
     * @return std::vector<T> 
     */
    template<typename T>
    std::vector<T> values(){
        if(_values.size() > 0){
            std::vector<T> cast_values;
            for(auto& it:_values)
                cast_values.emplace_back(std::any_cast<T>(it));
            return cast_values;
        }else{
            return std::vector<T>(std::any_cast<T>(_default));
        }
    }


    /**
     * @brief add value from argv (const char*)
     * 
     * @param str 
     * @return Argument& 
     */
    Argument& add_value(const char* str = nullptr){
        if(_default.type() == typeid(bool))
            _values.resize(1,true);

        if(str == nullptr)
            return *this;
        
        if(_n_args == '?' && _values.size() >= 1)
            throw Exception(this,"too many arg values");

        else if(_default.type() == typeid(int))
            _values.emplace_back(std::stoi(str));
        else if(_default.type() == typeid(long long))
            _values.emplace_back(std::stoll(str));
        else if(_default.type() == typeid(double))
            _values.emplace_back(std::stod(str));
        else if(_default.type() == typeid(char)){
            if(strlen(str) > 1)
                throw Exception(this,"too many characters");
            _values.emplace_back(str[0]);
        }
        else if(_default.type() == typeid(std::string))
            _values.emplace_back(std::string(str));
        else
            throw std::bad_typeid();
        return *this;
    }

    
    /**
     * @brief assert read parameters
     * 
     */
    void argument_assert(){
        std::string tag = _full_tag.length() > 0 ? _full_tag : _short_tag;
        /* assert require */
        if(_required && _values.size() == 0)
            throw Exception(this,"argument required");

        /* assert numbers */
        if(_n_args == '+' && _values.size() == 0)
            throw Exception(this,"too few parameters");

        if(_n_args == '?' && _values.size() > 1)        
            throw Exception(this,"too many parameters");
        
        /* assert choices */
        if(_choices.size() > 0){
            for(auto& it:_values){
                bool match = false;
                for(auto& it2:_choices)
                    if(_is_equal(it,it2))
                        {match = true; break;}
                if(!match)  throw Exception(this,"not found in choices");    
            }
        }
    }


    /**
     * @brief Set the Tag object
     * 
     * @param name 
     * @return Argument& 
     */
    Argument& setTag(std::string name){
        if(name.length() > 0)
            _short_tag = std::string(name);
        return *this;
    }

    /**
     * @brief Set the Name object
     * 
     * @param name 
     * @return Argument& 
     */
    Argument& setName(std::string name){
        if(name.length() > 0)
            _full_tag = std::string(name);
        return *this;
    }

    /**
     * @brief get tag (shot-tag)
     * 
     * @return std::string 
     */
    std::string tag(){
        return _short_tag;
    }

    /**
     * @brief get name (full-tag)
     * 
     * @return std::string 
     */
    std::string name(){
        return _full_tag;
    }

    /**
     * @brief get argument type name
     * 
     * @return std::string 
     */
    std::string arg_typename(){
        return std::string(_default.type().name());
    }

    /**
     * @brief Get the help info
     * 
     * @return std::string 
     */
    std::string get_help(){
        return _help;
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
    int                                 _size = 0;                  // size of arguments
    std::unordered_map<std::string,int> _arg_map_s;                 // mapping short string to pos of argument-structure
    std::unordered_map<std::string,int> _arg_map_l;                 // mapping long string to pos of argument-structure
    std::vector<Argument>               _args;                      // argument-structure list

    // filter prefix of '-'
    int _filter(std::string& name){
        if(name.size() == 0)
            return -1;
        int p;
        for(p = 0;p < name.length() && name[p] == '-';p++);
        name = (p > 2 ? "" : name.substr(p));
        return p;
    }

    // calc prefix of '-'
    int _argname_assert(std::string name){
        if(name.length() == 0 || name.find(' ') != std::string::npos)
            return -1;
        
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
    Argument* _get_argument(std::string index, bool is_short = true){
        int pos = is_short ? (_arg_map_s.find(index) == _arg_map_s.end() ? -1 : _arg_map_s[index])
                           : (_arg_map_l.find(index) == _arg_map_l.end() ? -1 : _arg_map_l[index]);
        return pos == -1 ? nullptr : &_args[pos];
    }

public:

    /**
     * @brief Add new argument structure
     * 
     * @tparam T 
     * @param name_or_flag 
     * @param full_argname 
     * @return Argument& 
     */
    template<typename T>
    Argument& add_argument(std::string name_or_flag, std::string full_argname = ""){
        /* assertion */
        int tag_type[2] = {_argname_assert(name_or_flag), _argname_assert(full_argname)};
        if((tag_type[0] != 0 && tag_type[0] != 1)           || 
           (full_argname.length() > 0 && tag_type[1] != 2)  ||
           (tag_type[0] == 0 && tag_type[1] > 0))
        {
            throw Argument::Exception("invalid argument format");
        }

        _size++;

        /* consturct new argument and initialize */
        _args.emplace_back(Argument());
        Argument* new_arg = &_args[_args.size() - 1];
        (*new_arg).init<T>();

        /* do tag assertion*/
        _filter(name_or_flag);
        _filter(full_argname);
        std::string tag = name_or_flag, name = full_argname;
        if(tag == "h" || tag == "help")
            throw Argument::Exception("reserved keyword");
        
        (*new_arg).setTag(tag).setName(name);

        if(tag_type[0] == 0){
            /* positional arguments */
            if(!_positional_flag)
                throw Argument::Exception("positional args should be set before optional args");
            _positional_num++;
            (*new_arg).required().nArgs('?'); // set positional arg
        }else{
            /* optional arguments */
            _positional_flag = false;
        }
        
        /* insert into parser */
        if(_arg_map_s.find(tag) != _arg_map_s.end() || _arg_map_l.find(name) != _arg_map_l.end())
            throw Argument::Exception("duplicated argument");
            
        _arg_map_s[tag] = _args.size() - 1;
        if(full_argname.length() > 0)
            _arg_map_l[name] = _args.size() - 1;
        
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
        auto iter = _arg_map_s.find(index);
        if(iter == _arg_map_s.end()) iter = _arg_map_l.find(index);
        if(iter == _arg_map_s.end() || iter == _arg_map_l.end())  return T();
        return _args[(*iter).second].value<T>();
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
        return _args[index].value<T>();
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
        auto iter = _arg_map_s.find(index);
        if(iter == _arg_map_s.end()) iter = _arg_map_l.find(index);
        if(iter == _arg_map_s.end() || iter == _arg_map_l.end())  return std::vector<T>();
            
        return _args[(*iter).second].values<T>();
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
        return _args[index].values<T>();
    }


    /**
     * @brief start parsing args with argc and argv
     * 
     * @param argc 
     * @param argv 
     */
    void parse_args(int argc, char** argv){
        if(argc <= 0 || argv == nullptr)
            throw Argument::Exception("no args");

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
            _args[i].add_value(argv[ptr++]);
        }

        /* process optional args */
        while(ptr < argc){
            Argument* arg_ptr = nullptr;
            std::string arg_name(argv[ptr++]);
            std::string arg_name_origin = arg_name;
            int type = _filter(arg_name);
            if(type == 1 || type == 2){
                arg_ptr = _get_argument(arg_name, type == 1);
            }
            if(arg_ptr == nullptr)
                throw Argument::Exception("unknown argument '" + arg_name_origin + "'");
            
            arg_ptr->add_value();
            while(ptr < argc && _argname_assert(std::string(argv[ptr])) == 0){
                arg_ptr->add_value(argv[ptr++]);
            }
        }

        /* assertion */
        for(auto& it:_args)
            it.argument_assert();
    }


    /**
     * @brief print help info
     * 
     */
    void show_help(){
        std::cout << "usage: " << _file_name << " ";
        for(int i = 0; i < _positional_num; i++)
            std::cout << _args[i].tag() << " ";
        for(int i = _positional_num; i < _size; i++)
            std::cout << "[-" << _args[i].tag() << "] ";

        /* positional */
        std::cout << "\n\npositional arguments:\n";
        for(int i = 0; i < _positional_num; i++)
            std::cout << "  " << _args[i].tag() << "\t" << _args[i].get_help() << std::endl;
        
        /* optional */
        std::cout << "\noptional arguments:\n";
        std::cout << "  -h, --help\tshow help message\n";
        for(int i = _positional_num; i < _size; i++){
            std::cout << "  -" << _args[i].tag();
            if(_args[i].name().length() > 0)
                std::cout << ", --" << _args[i].name();
            else std::cout << "\t";
            std::cout << "\t" << _args[i].get_help() << std::endl;
        }
    }

};


#endif