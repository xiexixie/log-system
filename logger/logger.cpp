#include "logger.h"
#include <time.h>
#include <stdarg.h>
#include <iostream>
#include <cstring>

using namespace xie::Log;

const char *logger::s_level[LEVEL_COUNT] = {
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "FATAL"};
std::unique_ptr<logger> logger::m_instance = nullptr;
std::ofstream logger::m_fout;
logger::Close logger::close;
logger *logger::instance()
{
  if (m_instance == nullptr)
    m_instance.reset(new logger);
  return m_instance.get();
}

logger::logger() : m_max(0), m_len(0), m_level(DEBUG)
{
  // std::cout << "creat" << std::endl;
}

void logger::open(const std::string &filename)
{
  m_filename = filename;
  m_fout.open(filename, std::ios::app);
  if (m_fout.fail())
  {
    throw new std::logic_error("open file failed" + filename);
  }
  m_fout.seekp(0, std::ios::end);
  m_len = m_fout.tellp();
}

void logger::log(Level level, const char *file, int line, const char *format, ...)
{
  if (m_level > level)
    return;
  if (m_fout.fail())
  {
    throw new std::logic_error("open file failed" + m_filename);
  }
  // 得到当前时间戳
  time_t t = time(nullptr);
  struct tm *ptn = localtime(&t);
  char timestamp[32];
  memset(timestamp, 0, sizeof(timestamp));
  strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", ptn);

  // 得到需要记录的信息的长度
  const char *fmt = "%s %s %s:%d";
  int size = snprintf(nullptr, 0, fmt, timestamp, s_level[level], file, line);
  // 将信息格式化
  if (size > 0)
  {
    char *buffer = new char[size + 1];
    snprintf(buffer, size + 1, fmt, timestamp, s_level[level], file, line);
    buffer[size] = '\0';
    m_fout << buffer;
    m_len += size;
    // std::cout << buffer << std::endl;
    delete buffer;
  }
  m_fout << " ";
  va_list va_ptr;
  va_start(va_ptr, format);
  size = vsnprintf(nullptr, 0, format, va_ptr);
  if (size > 0)
  {
    char *content = new char[size + 1];
    vsnprintf(content, size + 1, format, va_ptr);
    m_fout << content;
    m_len += size;
    delete content;
  }
  va_end(va_ptr);
  m_fout << '\n';
  m_fout.flush();
  if (m_len >= m_max && m_max > 0)
    rotate();
  // std::cout << timestamp << std::endl;
  // std::cout << file << std::endl;
  // std::cout << line << std::endl;
  // std::cout << format << std::endl;
}

void logger::rotate()
{
  m_fout.close();
  // 得到当前时间戳
  time_t t = time(nullptr);
  struct tm *ptn = localtime(&t);
  char timestamp[32];
  memset(timestamp, 0, sizeof(timestamp));
  strftime(timestamp, sizeof(timestamp), ".%Y-%m-%d_%H-%M-%S", ptn);

  // 将原log文件重命名
  std::string filename = m_filename + timestamp;
  if (rename(m_filename.c_str(), filename.c_str()) != 0)
    throw new std::logic_error("rename log failed");
  open(m_filename);
}