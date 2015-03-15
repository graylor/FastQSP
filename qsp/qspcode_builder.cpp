#include "qspcode_builder.h"
#include "coding.h"
#include "text.h"
#include <cassert>

QspCodeBuilder::QspCodeBuilder()
  : buffer() {
  buffer.reserve(15000);
}

QspCodeBuilder::~QspCodeBuilder() {

}

int QspCodeBuilder::WriteIntVal(int val, QSP_BOOL isCode) {
  QSP_CHAR buf[12];
  QSP_CHAR* temp;
  qspNumToStr(buf, val);
  if (isCode) {
    temp = qspCodeReCode(buf, QSP_TRUE);
    AddText(temp);
    free(temp);
  } else {
    AddText(buf);
  }
  return AddText(QSP_STRSDELIM, QSP_LEN(QSP_STRSDELIM));
}

int QspCodeBuilder::WriteVal(QSP_CHAR *val, QSP_BOOL isCode) {
  QSP_CHAR *temp;
  if (val) {
    if (isCode) {
      temp = qspCodeReCode(val, QSP_TRUE);
      AddText(temp);
      free(temp);
    } else {
      AddText(val);
    }
  }
  return AddText(QSP_STRSDELIM, QSP_LEN(QSP_STRSDELIM));
}

size_t QspCodeBuilder::BufferLength()
{
  return buffer.length() + 1;
}

QSP_CHAR* QspCodeBuilder::CloneBuffer() {
  std::size_t len = buffer.length() + 1;
  QSP_CHAR* result = reinterpret_cast<QSP_CHAR*>(malloc(len*sizeof(QSP_CHAR)));
  qspStrNCopy(result, buffer.c_str(), len);
  return result;
}

int QspCodeBuilder::AddText(QSP_CHAR* val, int valLen)
{
  qsp_string value(val, valLen);
  buffer += value;
  return buffer.length();
}

int QspCodeBuilder::AddText(QSP_CHAR* val) {
  qsp_string value(val);
  buffer += value;
  return buffer.length();
}

