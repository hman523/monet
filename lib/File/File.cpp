//
// Created by hunter on 7/31/19.
//

#include "File.h"
#include "../../include/Exception.h"
#include "../../include/Interpreter.h"
#include <cstring>
#include <fstream>
#include <vector>

File::File() {
  functions.insert({"file.read", "file.write", "file.exists", "file.getline",
                    "file.words", "file.split", "file.last", "file.append",
                    "file.count"});
}

std::set<std::string> File::getFunctions() { return functions; }

std::string File::eval(const std::string &expression) {
  std::vector<std::string> words = Interpreter::Instance()->split(expression);
  if (words[0] == "file.read") {
    return readfile(words);
  } else if (words[0] == "file.write") {
    return writefile(words);
  } else if (words[0] == "file.exists") {
    return fileexists(words);
  } else if (words[0] == "file.getline") {
    return getline(words);
  } else if (words[0] == "file.words") {
    return wordsfile(words);
  } else if (words[0] == "file.split") {
    return splitfile(words);
  } else if (words[0] == "file.last") {
    return getlastline(words);
  } else if (words[0] == "file.append") {
    return appendtofile(words);
  } else if (words[0] == "file.count") {
    return linecount(words);
  }
  throw Exception("Fatal implementation error in File::eval in File.cpp");
}

std::string File::evalstring(const std::string &val) {
  if (Interpreter::Instance()->isParens(val)) {
    return Interpreter::Instance()->eval(val);
  } else if (Interpreter::Instance()->isString(val)) {
    return Interpreter::Instance()->removequotes(val);
  } else {
    return Interpreter::Instance()->strtostr(val);
  }
}

std::string File::readfile(const std::vector<std::string> &vals) {
  if (vals.size() != 2) {
    throw Exception("Wrong number of parameters for file.read");
  }
  std::string filename = evalstring(vals[1]);
  std::ifstream file(filename);
  std::string text((std::istreambuf_iterator<char>(file)),
                   (std::istreambuf_iterator<char>()));
  file.close();
  return text;
}

std::string File::writefile(const std::vector<std::string> &vals) {
  if (vals.size() != 3) {
    throw Exception("Wrong number of parameters for file.write");
  }
  std::string filename = evalstring(vals[1]);
  std::string filecontents = evalstring(vals[2]);
  std::ofstream out(filename);
  out << filecontents;
  out.close();
  return "";
}

std::string File::getline(const std::vector<std::string> &vals) {
  if (vals.size() != 3) {
    throw Exception("Wrong number of parameters for file.getline");
  }
  std::string filename = evalstring(vals[1]);
  uint32_t linenum;
  if (Interpreter::Instance()->isParens(vals[2])) {
    linenum = (uint32_t)Interpreter::Instance()->strToNum(
        Interpreter::Instance()->eval(vals[2]));
  } else {
    linenum = (uint32_t)Interpreter::Instance()->strToNum(vals[2]);
  }
  std::ifstream file(filename);
  for (uint32_t x = 0; x < linenum; ++x) {
    file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  std::string text;
  std::getline(file, text);
  file.close();
  return text;
}

std::string File::wordsfile(const std::vector<std::string> &vals) {
  if (vals.size() != 2) {
    throw Exception("Wrong number of parameters for file.words");
  }
  std::string filename = evalstring(vals[1]);
  std::ifstream file(filename);
  std::string text((std::istreambuf_iterator<char>(file)),
                   (std::istreambuf_iterator<char>()));
  file.close();
  std::string delims = " \n\t";
  std::vector<std::string> words = split(text, delims);
  if (words.size() == 0) {
    // Case for empty file or file of just white space
    return "[]";
  }
  std::string wordslist = "[" + words[0];
  std::for_each(words.begin() + 1, words.end(), [&](std::string x) -> void {
    wordslist += (" \"" + x + "\"");
  });
  wordslist += "]";
  return wordslist;
}

std::string File::splitfile(const std::vector<std::string> &vals) {
  if (vals.size() < 3) {
    throw Exception("Wrong number of parameters for file.split");
  }
  std::string filename = evalstring(vals[1]);
  std::ifstream file(filename);
  std::string text((std::istreambuf_iterator<char>(file)),
                   (std::istreambuf_iterator<char>()));
  file.close();
  std::string delims;
  for (uint32_t j = 2; j < vals.size(); ++j) {
    delims += evalstring(vals[j]);
  }
  std::vector<std::string> words = split(text, delims);
  std::string wordslist = "[" + words[0];
  std::for_each(words.begin() + 1, words.end(), [&](std::string x) -> void {
    wordslist += (" \"" + x + "\"");
  });
  wordslist += "]";
  return wordslist;
}

std::string File::getlastline(const std::vector<std::string> &vals) {
  if (vals.size() != 2) {
    throw Exception("Wrong number of parameters for file.last");
  }
  std::string filename = evalstring(vals[1]);
  std::string contents;
  std::ifstream file(filename, std::ios::ate);
  int x = EOF;
  char curr;
  do {
    file.seekg(x, std::ios::end);
    file.get(curr);
    contents = curr + contents;
    --x;
  } while (curr != '\n' && curr != '\r');
  return contents.substr(1);
}

std::string File::appendtofile(const std::vector<std::string> &vals) {
  if (vals.size() != 3) {
    throw Exception("Wrong number of parameters for file.append");
  }
  std::string filename = evalstring(vals[1]);
  std::ofstream out;
  out.open(filename, std::ios::app);
  std::string filecontents;
  if (Interpreter::Instance()->isParens(vals[2])) {
    filecontents = Interpreter::Instance()->eval(vals[2]);
  } else if (Interpreter::Instance()->isString(vals[2])) {
    filecontents = Interpreter::Instance()->removequotes(vals[2]);
  } else {
    filecontents = Interpreter::Instance()->strtostr(vals[2]);
  }
  out << "\n" << filecontents;
  out.close();
  return "";
}

std::string File::fileexists(const std::vector<std::string> &vals) {
  if (vals.size() != 2) {
    throw Exception("Wrong number of parameters for file.readfile");
  }
  std::string filename = evalstring(vals[1]);
  std::ifstream file(filename);
  std::string exists = file.good() ? "true" : "false";
  file.close();
  return exists;
}

std::string File::linecount(const std::vector<std::string> &vals) {
  if (vals.size() != 2) {
    throw Exception("Wrong number of parameters for file.count");
  }
  uint32_t lines = 0;
  std::string filename = evalstring(vals[1]);
  std::string temp;
  std::ifstream file(filename);
  while (std::getline(file, temp)) {
    ++lines;
  }

  return std::to_string(lines);
}

std::vector<std::string> File::split(const std::string &text,
                                     const std::string &delims) {
  std::vector<std::string> words;
  std::string tempstring = "";
  for (uint32_t j = 0; j < text.length(); ++j) {
    bool foundDelim = false;
    for (uint32_t k = 0; k < delims.length(); ++k) {
      if (text.at(j) == delims.at(k)) {
        foundDelim = true;
        break;
      }
    }
    if (foundDelim) {
      if (tempstring != "") {
        words.push_back(tempstring);
        tempstring = "";
      }
    } else {
      tempstring += text.at(j);
    }
  }
  return words;
}
