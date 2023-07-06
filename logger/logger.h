#pragma once

#include <string>
#include <fstream>

namespace xie
{
  namespace Log
  {
#define debug(format, ...) logger::instance()->log(logger::DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__);
#define info(format, ...) logger::instance()->log(logger::INFO, __FILE__, __LINE__, format, ##__VA_ARGS__);
#define warn(format, ...) logger::instance()->log(logger::WARN, __FILE__, __LINE__, format, ##__VA_ARGS__);
#define error(format, ...) logger::instance()->log(logger::ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__);
#define fatal(format, ...) logger::instance()->log(logger::FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__);

    class logger
    {
    public:
      enum Level
      {
        DEBUG,
        INFO,
        WARN,
        ERROR,
        FATAL,
        LEVEL_COUNT
      };
      void log(Level level, const char *file, int line, const char *format, ...);
      static logger *instance();
      void open(const std::string &filename);
      void close();
      void level(Level level)
      {
        m_level = level;
      }
      void size(int bytes)
      {
        m_max = bytes;
      }

    private:
      std::string m_filename;
      std::ofstream m_fout;
      Level m_level;
      int m_max;
      int m_len;
      static const char *s_level[LEVEL_COUNT];
      static logger *m_instance;
      logger();
      ~logger();
      void rotate();
    };
  }
}