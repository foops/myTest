
#pragma once

#include "google/protobuf/util/json_util.h"
#include "fslib/fs/FileSystem.h"
#include <tuple>
#include <vector>
#include "autil/StringUtil.h"
#include <filesystem>

namespace fs = std::filesystem;
namespace suez {
  

class ProtobufUtils {

public:
  static bool protobufMessageToString(const google::protobuf::Message &message,
                                      std::string &outputStr) {
    return true;
  }

  static void printDirectory(std::string dir) {
    std::cout <<"list directoy, dir:"<<dir<<std::endl;
    for (const auto &entry : fs::directory_iterator(dir)) {
      // 判断是否为目录
      if (entry.is_directory()) {
        std::cout << "list directoy [DIR]  " << entry.path().filename()<< std::endl;
        printDirectory(entry.path());
      } else {
        std::cout << "list directoy [FILE] " << entry.path().filename()<< std::endl;
      }
    }
  }

  static bool fileToProtobufMessage(const std::string &fileName, google::protobuf::Message &message
    , const std::vector<std::pair<std::string,std::string>> & replacePair={}) {

    // std::cout << "load protobuf message from file: " << fileName << std::endl;
    std::string content;
    auto ec = fslib::fs::FileSystem::readFile(fileName, content);
    if (ec != fslib::EC_OK) {
      return false;
    }
    // std::cout <<"file content:"<<content<<std::endl;
    for(const auto & pair : replacePair){
        // std::cout <<" fileName:"<<fileName <<",replace, key="<<pair.first<<", value="<<pair.second<<std::endl;
        autil::StringUtil::replaceAll(content, pair.first, pair.second);
      } 

    google::protobuf::util::JsonParseOptions options;
    options.ignore_unknown_fields = true;
    auto status =
        google::protobuf::util::JsonStringToMessage(content, &message, options);
    return status.ok();
  }
};

} // namespace suez
