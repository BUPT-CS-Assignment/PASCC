//
// Created by jianxff on 2023/4/18.
//

#include "argparse.hpp"
#include "compiler.h"
#include "log.h"
#include "parser.h"
#include "parser.tab.h"
#include <algorithm>
using std::string;

/*  test command examples
 *  - arguments list
 *    usage: ${PASCC} [-i] [-o] [-t] [-r]
 *    optional arguments:
 *    tag  name      n-args  note
 *     -h, --help      -     show help message
 *     -i, --input    [?]    input pascal-s file
 *     -o, --output   [?]    output c file
 *     -s, --style    [?]    code style
 *     -d, --debug    [?]    debug mode
 *                           (0: QUIET, 1: ERROR, 2: WARN,
 *                            3: INFO, 4: DEBUG, 5: TRACE)
 *     -t, --test     [?]    test output c file, optional test args
 *     -r, --reserve  [?]    reserve cache files
 *
 *  - set in (out to screen), set code style and reserve cache files
 *      ${PASCC} -i input/input01.txt -s google -r
 *
 *  - set in & out, run c files generated and set running arguments
 *      ${PASCC} -i input/input01.txt -o output/output01.c -t "> test.out"
 *
 */

void dir_make(std::string&);
void file_make(std::string&);

int main(int argc, char** argv) {
  ArgumentParser parser;
  // add optional argument 'input json file' to set input json file
  parser.add_argument<string>("-i", "--input")
      .nArgs('?')
      .help("input pascal-s file")
      .default_("test.pas");
  // add optional argument 'output file' to set output c file
  parser.add_argument<string>("-o", "--output")
      .nArgs('?')
      .help("output c file")
      .default_("");
  // add optional argument 'code style' to set coding styles
  parser.add_argument<string>("-s", "--style")
      .nArgs('?')
      .help("code style")
      .choices({"google", "none", "llvm", "chromium", "mozilla", "webkit"});
  // add optional argument 'debug level' to set debug level
  parser.add_argument<int>("-d", "--debug")
      .nArgs('?')
      .help("debug level")
      .choices({0, 1, 2, 3, 4, 5})
      .default_(3);
  // add optional argument 'test' to run test and set test args
  parser.add_argument<string>("-t", "--test")
      .nArgs('?')
      .help("test output c file, optional test args")
      .default_("");
  // add optional argument 'remove cache' to auto-clear cache files
  parser.add_argument<bool>("-r", "--reserve").help("reserve cache files");

  // argument parse
  parser.parse_args(argc, argv);
  // get argument value
  // set debug level
  if (parser.is_call("d")) {
    int debug = parser.get_value<int>("d");
    log_set_level(5 - debug);
    yydebug_(debug);
  }

  // get input source file
  string in = parser.get_value<string>("i");
  dir_make(in);
  // get output destination
  string out = parser.get_value<string>("o");
  if (parser.is_call("t") && !parser.is_call("o")) {
    out = "_pascc_tmp_" + std::to_string(time(nullptr));
  }
  dir_make(out);
  file_make(out);
  // get code styles
  string style = parser.get_value<string>("s");

  string dir = string(argv[0]);
  Compiler compiler(dir);
  if (compiler.Compile(in, out, style) < 0) return 1;

  // optional test execute
  if (parser.is_call("t")) {
    string test_args = parser.get_value<string>("t");
    compiler.CodeExecute(out, test_args);
  }

  // optional reserve cache
  if (!parser.is_call("r")) {
    bool rm_all = parser.is_call("t") && (!parser.is_call("o"));
    compiler.Remove(out, rm_all);
  }

  return 0;
}

void dir_make(string& dir) {
#ifdef WIN32
  std::replace(dir.begin(), dir.end(), '/', '\\');
#else
  std::replace(dir.begin(), dir.end(), '\\', '/');
#endif
}

void file_make(string& file) {
  size_t len = file.length();
  if (file.length() > 2) {
    if (file[len - 1] == 'c' && file[len - 2] == '.') {
      file = file.substr(0, len - 2);
    }
  }
}