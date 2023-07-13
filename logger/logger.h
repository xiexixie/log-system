#pragma once

#include <string>
#include <fstream>
#include <memory>
#include <iostream>

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
      static std::ofstream m_fout;
      Level m_level;
      int m_max;
      int m_len;
      static const char *s_level[LEVEL_COUNT];
      static std::unique_ptr<logger> m_instance; // 单例模式无法调用析构函数，使用智能指针
      logger();
      class Close
      {
      public:
        ~Close()
        {
          m_fout.close();
          std::cout << "close";
        }
      };
      static Close close; // 静态成员最终结束会自动析构
      void rotate();
    };
  }
}