/**
 * This file is part of the "FnordMetric" project
 *   Copyright (c) 2014 Paul Asmuth, Google Inc.
 *
 * Licensed under the MIT license (see LICENSE).
 */
#ifndef _FNORDMETRIC_CSVFILE_H
#define _FNORDMETRIC_CSVFILE_H
#include <string>
#include <vector>
#include <memory>
#include "../../util/unittest.h"

namespace fnordmetric {
namespace csv_backend {

class CSVFile {
  friend class fnordmetric::util::UnitTest::TestCase;
public:

  enum ErrorCodes {
    ERR_CSV_CANNOT_OPEN_FILE = 4000,
    ERR_CSV_READ_ERROR = 4001
  };

  /**
   * Open a new csv file from the provided file path. Throws an exception if
   * the file cannot be opened.
   *
   * @param file_path the path to the csv file
   */
  static std::unique_ptr<CSVFile> openFile(
      const std::string& file_path,
      char column_seperator = ',',
      char row_seperator = '\n',
      char quote_char = '"');

  CSVFile(const CSVFile& other) = delete;
  CSVFile& operator=(const CSVFile& other) = delete;

  /**
   * Create a new CSVFile instance with the provided filedescriptor. The csv
   * file takes ownership of the fd and will close() it when destructed.
   *
   * @param fd a valid an opened fd, transfers ownership and closes on destruct
   */
  explicit CSVFile(
      int fd,
      char column_seperator = ',',
      char row_seperator = '\n',
      char quote_char = '"');

  /**
   * Read the next row from the csv file
   */
  void readNextRow(std::vector<std::string>* target);

protected:

  /**
   * Read the next column from the csv file
   */
  std::string readNextColumn();


  /**
   * Read the next chunk from disk. Returns true on success and false on EOF.
   * Raises an exception on error.
   */
  bool readNextChunk();


  int fd_;
  char buf_[4];
  size_t buf_len_;
  size_t buf_pos_;
};

}
}
#endif