//
// Created by jianxff on 2023/4/18.
//

#include "compiler.h"
#include "parser.h"
#include "parser.tab.h"
#include "argparse.hpp"
#include "log.h"
using std::string;

/*  test command examples
 *  - arguments list
 *    usage: ${PASCC} [-i] [-o] [-t] [-r]
 *    optional arguments:
 *    tag  name      n-args  note
 *     -h, --help      -     show help message
 *     -i, --input    [?]    input json file
 *     -o, --output   [?]    output c file
 *     -s, --style    [?]    code style
 *     -d, --debug    [?]    debug mode (0: ERROR, 1: WARN, 2: INFO, 3: DEBUG)
 *     -t, --test     [?]    test output c file, optional test args
 *     -r, --reserve  [?]    reserve cache files
 *
 *  - set input and print result to screen, set code style and reserve cache files
 *      ${PASCC} -i input/input01.txt -s google -r
 *
 *  - set input and output destination, run c files generated and set running arguments
 *      ${PASCC} -i input/input01.txt -o output/output01.c -t "> test.out"
 *
 */

int main(int argc, char** argv){
  ArgumentParser parser;
  // add optional argument 'input json file' to set input json file
  parser.add_argument<string>("-i","--input").nArgs('?').help("input json file")
        .default_("test.pas");
  // add optional argument 'output file' to set output c file
  parser.add_argument<string>("-o","--output").nArgs('?').help("output c file")
        .default_("");
  // add optional argument 'code style' to set coding styles
  parser.add_argument<string>("-s","--style").nArgs('?').help("code style")
        .choices({"google","llvm","chromium","mozilla","webkit"});
  // add optional argument 'debug level' to set debug level
  parser.add_argument<int>("-d","--debug").nArgs('?').help("debug level")
        .choices({0,1,2,3}).default_(2);
  // add optional argument 'test' to run test and set test args
  parser.add_argument<string>("-t","--test").nArgs('?').help("test output c file, optional test args")
        .default_("");
  // add optional argument 'remove cache' to auto-clear cache files
  parser.add_argument<bool>("-r","--reserve").help("reserve cache files");

  // argument parse
  parser.parse_args(argc, argv);
  // get argument value
  // set debug level
  log_set_level(2);
  if(parser.is_call("d")){
    int debug = parser.get_value<int>("d");
    log_set_level(debug == 0 ? 4 : debug == 1 ? 3 : debug == 2 ? 2 : 1);
    yydebug_(debug);
  }

  // get input source file
  string in = parser.get_value<string>("i");
  // get output destination
  string out = parser.get_value<string>("o");
  // get code styles
  string style = parser.get_value<string>("s");

  Compiler compiler;
  int p = compiler.Compile(in,out,style);


  // optional test execute
  if(parser.is_call("t")) {
    string test_args = parser.get_value<string>("t");
    compiler.CodeExecute(compiler.tmp_file(p),test_args);
  }

  // optional reserve cache
  if(!parser.is_call("r")){
    compiler.Clear();
  }

  return 0;
}