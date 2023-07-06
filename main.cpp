#include <iostream>
#include "logger.h"

using namespace xie::Log;

int main()
{
  logger::instance()->open("./test.log");
  logger::instance()->level(logger::WARN);
  logger::instance()->size(200);
  // logger::instance()->log(logger::DEBUG, __FILE__, __LINE__, "hello world");
  // logger::instance()->log(logger::DEBUG, __FILE__, __LINE__, "name:%s,age:%d", "jack", 18);

  debug("debug message");
  error("error message");
  warn("warn message");
  info("info message");
  fatal("fatal message");

  return 0;
}