#ifndef QSPCODE_BUILDER_H
#define QSPCODE_BUILDER_H
#include "declarations.h"

class QspCodeBuilder {
public:
  QspCodeBuilder();
  ~QspCodeBuilder();
  int WriteIntVal(int val, QSP_BOOL isCode);
  int WriteVal(QSP_CHAR *val, QSP_BOOL isCode);

  QSP_CHAR* GetResult();
private:
  int AddText(QSP_CHAR* val, int valLen);
  int AddText(QSP_CHAR* val);

  qsp_string buffer;
};

#endif // QSPCODE_BUILDER_H
