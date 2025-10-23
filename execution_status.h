#ifndef EXECUTION_STATUS_H
#define EXECUTION_STATUS_H

enum class execution_status
{
  success,
  open_file,
  read_file,
  memory,
  bad_arg,
  small_divider,
  runtime_error

};

#endif